//#include <stdlib.h>
//#include <time.h>

#include "postgres.h"
#include "fmgr.h"
#include "common/base64.h"
#include "utils/builtins.h"
#include "aes.h"
#include "pg_function.h"


/*
*	AES:https://github.com/kokke/tiny-AES-c
*	RSA:https://github.com/andrewkiluk/RSA-Library
*/

PG_FUNCTION_INFO_V1(generate_aes_key);
PG_FUNCTION_INFO_V1(generate_aes_iv);
PG_FUNCTION_INFO_V1(aes_encrypt);
PG_FUNCTION_INFO_V1(aes_decrypt);


//安全释放分配的内存
#define SE_free(ptr_)\
do {\
	if (NULL !=ptr_) {\
		pfree(ptr_); ptr_ = NULL;\
	}\
} while (0);

/*生成密钥*/
static uint8_t * generate_key(int *key_len) {
	unsigned char* key;
#if defined(AES192)
	*key_len = 24;
#elif defined (AES256)
	*key_len = 32;
#else
	*key_len = 16;
#endif
	key = palloc(*key_len);
	for (int i = 0; i < (*key_len); i++)
		key[i] = random() % 0xFF;
	return key;
}

/*base64编码*/
static bool base64_encrypt(const char *data, int32_t data_len, text **ptr) {
	text *result = NULL;
	int32_t rc = 0, b64_len = 0;

	if (NULL == data || data_len < 1)
		goto SE_ERROR_CLEAR;

	b64_len = pg_b64_enc_len(data_len);
	result = (text *)palloc(b64_len + VARHDRSZ);
	rc = pg_b64_encode(data, data_len, VARDATA(result));
	if (-1 == rc)
		goto SE_ERROR_CLEAR;
	SET_VARSIZE(result, rc + VARHDRSZ);
	*ptr = result;
	return true;
SE_ERROR_CLEAR:
	SE_free(result);
	(*ptr) = NULL;
	ereport(ERROR, (errcode(ERRCODE_DATA_EXCEPTION), errmsg("base64 encode fail!")));
	return false;
}

/*base64解码*/
static bool base64_decrypt(const char *base64, int32_t b64_len, text **ptr) {
	text *result = NULL;
	int32_t rc = 0, bin_len = 0;

	if (NULL == base64 || b64_len < 1)
		goto SE_ERROR_CLEAR;

	bin_len = pg_b64_dec_len(b64_len);
	result = (text *)palloc(bin_len + VARHDRSZ);
	rc = pg_b64_decode(base64, b64_len, VARDATA(result));
	if (-1 == rc)
		goto SE_ERROR_CLEAR;
	SET_VARSIZE(result, rc + VARHDRSZ);
	(*ptr) = result;
	return true;
SE_ERROR_CLEAR:
	SE_free(result);
	(*ptr) = NULL;
	ereport(ERROR, (errcode(ERRCODE_DATA_EXCEPTION), errmsg("base64 decode fail!")));
	return false;
}

/*生成AES密钥或向量*/
static bool generate_aes(text **ptr) {
	int bin_key_len;
	char* bin_key = NULL;
	bin_key = generate_key(&bin_key_len);
	if (!base64_encrypt(bin_key, bin_key_len, ptr))
		goto SE_ERROR_CLEAR;
	return true;
SE_ERROR_CLEAR:
	return false;
}

/*生成AES密钥*/
Datum generate_aes_key(PG_FUNCTION_ARGS) {
	text *result = NULL;
	if (generate_aes(&result))
		PG_RETURN_TEXT_P(PointerGetDatum(result));
	else
		PG_RETURN_NULL();
}

/*生成AES向量*/
Datum generate_aes_iv(PG_FUNCTION_ARGS) {
	text *result = NULL;
	if (generate_aes(&result))
		PG_RETURN_TEXT_P(PointerGetDatum(result));
	else
		PG_RETURN_NULL();
}


#define aes_encrypt_free(bin_key,bin_iv,bin_encrypt) do {\
	if (NULL != bin_key) {\
		pfree(bin_key); bin_key = NULL;\
	}\
	if (NULL != bin_iv) {\
		pfree(bin_iv); bin_iv = NULL;\
	}\
	if (NULL != bin_encrypt) {\
		pfree(bin_encrypt); bin_encrypt = NULL;\
	}\
} while (0);

/*加密字符串*/
Datum aes_encrypt(PG_FUNCTION_ARGS) {
	text *b64_key, *b64_iv, *bin_data;
	text *bin_key = NULL, *bin_iv = NULL, *result;
	char *bin_encrypt = NULL;
	int   bin_data_len, bin_encrypt_len;
	struct AES_ctx ctx;
	char *ptr_tmp;

	/* must cast away the const, unfortunately 去掉const*/
	b64_key = pg_detoast_datum_packed((struct varlena *)(PG_GETARG_TEXT_P(0)));
	b64_iv = pg_detoast_datum_packed((struct varlena *)(PG_GETARG_TEXT_P(1)));
	bin_data = pg_detoast_datum_packed((struct varlena *)(PG_GETARG_TEXT_P(2)));

	if (!base64_decrypt(VARDATA_ANY(b64_key), VARSIZE_ANY_EXHDR(b64_key), &bin_key))
		goto SE_ERROR_CLEAR;
	if (!base64_decrypt(VARDATA_ANY(b64_iv), VARSIZE_ANY_EXHDR(b64_iv), &bin_iv))
		goto SE_ERROR_CLEAR;

	bin_data_len = VARSIZE_ANY_EXHDR(bin_data); //VARSIZE_ANY_EXHDR获取的长度包含结束符0
	//计算补位后的字节长度,asc数据大小要求16的倍数
	bin_encrypt_len = (bin_data_len / AES_BLOCKLEN * AES_BLOCKLEN) + (0 == (bin_data_len % AES_BLOCKLEN) ? 0 : AES_BLOCKLEN);
	bin_encrypt = (char *)palloc(bin_encrypt_len);
	memcpy(bin_encrypt, VARDATA_ANY(bin_data), bin_data_len);
	//bin_encrypt[bin_data_len] = '\0';
	//补位的字节设置为0	
	if ((bin_encrypt_len - bin_data_len) > 0) {
		ptr_tmp = bin_encrypt + bin_data_len;
		memset(ptr_tmp, 0, bin_encrypt_len - bin_data_len);
	}
	//初始化密钥和向量后开始加密
	AES_init_ctx_iv(&ctx, VARDATA_ANY(bin_key), VARDATA_ANY(bin_iv));
	AES_CBC_encrypt_buffer(&ctx, (uint8_t *)bin_encrypt, bin_encrypt_len);
	if (!base64_encrypt(bin_encrypt, bin_encrypt_len, &result))
		goto SE_ERROR_CLEAR;
	aes_encrypt_free(bin_key, bin_iv, bin_encrypt);
	PG_RETURN_TEXT_P(PointerGetDatum(result));
SE_ERROR_CLEAR:
	aes_encrypt_free(bin_key, bin_iv, bin_encrypt);
	PG_RETURN_NULL();
}

/*解密字符串*/

#define aes_decrypt_free(bin_key,bin_iv) do {\
	if (NULL != bin_key) {\
		pfree(bin_key); bin_key = NULL;\
	}\
	if (NULL != bin_iv) {\
		pfree(bin_iv); bin_iv = NULL;\
	}\
} while (0);

Datum aes_decrypt(PG_FUNCTION_ARGS) {
	text *b64_key, *b64_iv, *b64_data;
	text *bin_key = NULL, *bin_iv = NULL,*result;
	struct AES_ctx ctx;
	/* must cast away the const, unfortunately 去掉const*/
	b64_key = pg_detoast_datum_packed((struct varlena *)(PG_GETARG_TEXT_P(0)));
	b64_iv = pg_detoast_datum_packed((struct varlena *)(PG_GETARG_TEXT_P(1)));
	b64_data = pg_detoast_datum_packed((struct varlena *)(PG_GETARG_TEXT_P(2)));

	if (!base64_decrypt(VARDATA_ANY(b64_key), VARSIZE_ANY_EXHDR(b64_key), &bin_key))
		goto SE_ERROR_CLEAR;
	if (!base64_decrypt(VARDATA_ANY(b64_iv), VARSIZE_ANY_EXHDR(b64_iv), &bin_iv))
		goto SE_ERROR_CLEAR;
	if (!base64_decrypt(VARDATA_ANY(b64_data), VARSIZE_ANY_EXHDR(b64_data), &result))
		goto SE_ERROR_CLEAR;
	if (0 != (VARSIZE_ANY_EXHDR(result) % AES_BLOCKLEN))
		goto SE_ERROR_CLEAR;
	
	AES_init_ctx_iv(&ctx, VARDATA_ANY(bin_key), VARDATA_ANY(bin_iv));
	AES_CBC_decrypt_buffer(&ctx, (uint8_t *)VARDATA_ANY(result), VARSIZE_ANY_EXHDR(result) );
	SET_VARSIZE(result, strlen(VARDATA_ANY(result)) + VARHDRSZ);
	aes_decrypt_free(bin_key, bin_iv);
	PG_RETURN_TEXT_P(PointerGetDatum(result));
SE_ERROR_CLEAR:
	aes_decrypt_free(bin_key, bin_iv);
	PG_RETURN_NULL();
}