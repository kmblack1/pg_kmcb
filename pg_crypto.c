//#include <stdlib.h>
//#include <time.h>

#include "postgres.h"
#include "fmgr.h"
#include "common/base64.h"
#include "utils/builtins.h"
#include "aes.h"
#include "pg_function.h"

#define AES_BLOCKLEN (16) //Block length in bytes AES is 128b block only

/*
*	AES:https://github.com/kokke/tiny-AES-c
*	RSA:https://github.com/andrewkiluk/RSA-Library
*/

PG_FUNCTION_INFO_V1(generate_aes_key);
PG_FUNCTION_INFO_V1(generate_aes_iv);
PG_FUNCTION_INFO_V1(aes_encrypt);
PG_FUNCTION_INFO_V1(aes_decrypt);

/*生成密钥*/
static uint8_t * generate_key(int *key_len) {
	unsigned char* key;
#if defined(AES192)
	(*key_len) = 24;
#elif defined (AES256)
	(*key_len) = 32;
#else
	(*key_len) = 16;
#endif
	key = palloc((*key_len));
	for (int i = 0; i < (*key_len); i++)
		key[i] = random() % 255;
	return key;
}

/*解析BASE64编码的KEY和IV*/
static uint8_t *decode_aes(const char *p_key) {
	int key_dec_len, rc, key_len;
	uint8_t *dec_key = NULL, *key = NULL;

	key_len = (int)strlen(p_key);
	key_dec_len = pg_b64_dec_len(key_len);
	dec_key = (uint8_t *)palloc(key_dec_len);
	rc = pg_b64_decode(p_key, key_len, (char *)dec_key);
	if (-1 == rc) {
		pfree(dec_key);
		return NULL;
	}
#if defined(AES192)
	key = (uint8_t *)palloc(24);
	memcpy(key, dec_key, 24);
#elif defined (AES256)
	key = (uint8_t *)palloc(32);
	memcpy(key, dec_key, 32);
#else
	key = (uint8_t *)palloc(16);
	memcpy(key, dec_key, 16);
#endif	
	pfree(dec_key);
	return key;
}

/*生成AES密钥或向量*/
static text * generate_aes() {
	int key_len, enc_len, rc;
	unsigned char* key = NULL;
	char *base64_dst = NULL;
	text *result = NULL;

	key = generate_key(&key_len);
	enc_len = pg_b64_enc_len(key_len);
	base64_dst = palloc(enc_len);
	rc = pg_b64_encode((char *)key, key_len, (char *)base64_dst);
	pfree(key);
	if (-1 == rc) {
		pfree(base64_dst);
		return NULL;
	}
	result = (text *)palloc(rc + VARHDRSZ);
	SET_VARSIZE(result, rc + VARHDRSZ);
	memcpy(VARDATA(result), base64_dst, rc);
	pfree(base64_dst);
	return result;
}

/*生成AES密钥*/
Datum generate_aes_key(PG_FUNCTION_ARGS) {
	text *result = generate_aes();
	if (NULL != result)
		PG_RETURN_TEXT_P(PointerGetDatum(result));
	else
		ereport(ERROR, (errcode(ERRCODE_DATA_EXCEPTION), errmsg("base64 encode fail!")));
	PG_RETURN_NULL();
}

/*生成AES向量*/
Datum generate_aes_iv(PG_FUNCTION_ARGS) {
	text *result = generate_aes();
	if (NULL != result)
		PG_RETURN_TEXT_P(PointerGetDatum(result));
	else
		ereport(ERROR, (errcode(ERRCODE_DATA_EXCEPTION), errmsg("base64 encode fail!")));
	PG_RETURN_NULL();
}

/*加密字符串*/
Datum aes_encrypt(PG_FUNCTION_ARGS) {
	int  base64_len, rc;
	uint32 data_len;
	char *p_key = NULL, *p_iv = NULL;
	uint8_t *key = NULL, *iv = NULL, *enc_data = NULL, *p_data = NULL;
	char *base64_dst = NULL;
	text *result = NULL;

	p_key = text_to_cstring(PG_GETARG_TEXT_P(0));
	key = decode_aes(p_key);
	if (NULL == key) {
		pfree(p_key);
		ereport(ERROR, (errcode(ERRCODE_DATA_EXCEPTION), errmsg("invalid KEY!")));
	}

	p_iv = text_to_cstring(PG_GETARG_TEXT_P(1));
	iv = decode_aes(p_iv);
	if (NULL == iv) {
		pfree(p_key); pfree(p_iv);
		ereport(ERROR, (errcode(ERRCODE_DATA_EXCEPTION), errmsg("invalid IV!")));
	}

	p_data = (uint8_t *)text_to_cstring(PG_GETARG_TEXT_P(2));
	data_len = (uint32)strlen((char *)p_data);
	if (0 != (data_len % AES_BLOCKLEN))
		data_len = data_len + (AES_BLOCKLEN - (data_len % AES_BLOCKLEN));

	enc_data = (uint8_t *)palloc(data_len);
	AES_CBC_encrypt_buffer(enc_data, p_data, data_len, key, iv);

	base64_len = pg_b64_enc_len(data_len);
	base64_dst = palloc(base64_len);
	rc = pg_b64_encode((char *)enc_data, data_len, (char *)base64_dst);
	if (-1 == rc) {
		pfree(p_key); pfree(p_iv); pfree(p_data);
		pfree(key); pfree(iv);
		pfree(enc_data); pfree(base64_dst);
		ereport(ERROR, (errcode(ERRCODE_DATA_EXCEPTION), errmsg("base64 encode fail!")));
	}

	result = (text *)palloc(rc + VARHDRSZ);
	SET_VARSIZE(result, rc + VARHDRSZ);
	memcpy(VARDATA(result), base64_dst, rc);

	pfree(p_key); pfree(p_iv); pfree(p_data);
	pfree(key); pfree(iv);
	pfree(enc_data); pfree(base64_dst);
	PG_RETURN_TEXT_P(PointerGetDatum(result));
}

/*解密字符串*/
Datum aes_decrypt(PG_FUNCTION_ARGS) {
	int  base64_len, rc;
	uint32 data_len;
	char *p_key = NULL, *p_iv = NULL;
	uint8_t *key = NULL, *iv = NULL, *dec_data = NULL, *p_data = NULL;
	char *base64_dst = NULL;
	text *result = NULL;
	size_t no_padding = 0;

	p_key = text_to_cstring(PG_GETARG_TEXT_P(0));
	key = decode_aes(p_key);
	if (NULL == key) {
		pfree(p_key);
		ereport(ERROR, (errcode(ERRCODE_DATA_EXCEPTION), errmsg("invalid KEY!")));
	}

	p_iv = text_to_cstring(PG_GETARG_TEXT_P(1));
	iv = decode_aes(p_iv);
	if (NULL == iv) {
		pfree(p_key); pfree(p_iv);
		ereport(ERROR, (errcode(ERRCODE_DATA_EXCEPTION), errmsg("invalid IV!")));
	}

	p_data = (uint8_t *)text_to_cstring(PG_GETARG_TEXT_P(2));
	data_len = (uint32)strlen((char *)p_data);

	base64_len = pg_b64_dec_len(data_len);
	base64_dst = palloc(base64_len);
	rc = pg_b64_decode((char *)p_data, data_len, base64_dst);
	if (-1 == rc) {
		pfree(p_key); pfree(p_iv); pfree(p_data); pfree(base64_dst);
		ereport(ERROR, (errcode(ERRCODE_DATA_EXCEPTION), errmsg("base64 encode fail!")));
	}
	if (0 != (rc % AES_BLOCKLEN)) {
		pfree(p_key); pfree(p_iv); pfree(p_data); pfree(base64_dst);
		ereport(ERROR, (errcode(ERRCODE_DATA_EXCEPTION), errmsg("invalid encrypt data!")));
	}

	dec_data = (uint8_t *)palloc(rc);
	AES_CBC_decrypt_buffer(dec_data, (uint8_t *)base64_dst, rc, key, iv);
	no_padding = strlen((char *)dec_data);

	result = (text *)palloc(no_padding + VARHDRSZ);
	SET_VARSIZE(result, no_padding + VARHDRSZ);
	memcpy(VARDATA(result), dec_data, no_padding);

	pfree(p_key); pfree(p_iv); pfree(p_data);
	pfree(key); pfree(iv); pfree(dec_data); pfree(base64_dst);
	PG_RETURN_TEXT_P(PointerGetDatum(result));
}