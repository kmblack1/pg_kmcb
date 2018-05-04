#include "postgres.h"
#include "fmgr.h"
#include "utils/array.h"
#include "utils/lsyscache.h"
#include "catalog/pg_type.h"
#include "tsearch/ts_type.h"
#include "utils/builtins.h"

#include "pg_function.h"
#include "se_string.h"
#include "simil.h"

PG_FUNCTION_INFO_V1(split_string);
//PG_FUNCTION_INFO_V1(split_tsvector);
PG_FUNCTION_INFO_V1(generate_crc32_v1);

PG_FUNCTION_INFO_V1(string_simil);
PG_FUNCTION_INFO_V1(string_same);


/*将字符串按指定的字数转换为数组*/
Datum split_string(PG_FUNCTION_ARGS) {
	uint32 array_count;
	text **ptr_values;
	int32 split_size;
	text *in_txt;

	ArrayType	*result;
	int16 typlen;
	bool typbyval;
	char typalign;

	if (PG_ARGISNULL(0))
		PG_RETURN_NULL();
	in_txt = PG_GETARG_TEXT_P(0);
	split_size = PG_GETARG_INT32(1);
	if (split_size < 0 || split_size > 6) {
		ereport(ERROR, (errcode(ERRCODE_INVALID_PARAMETER_VALUE),
			errmsg("function returning fail(""%s"")!", "invalid parameter,split_size > 0 and split_size <= 6")));
	}
	ptr_values = split_string_v0(in_txt, split_size, &array_count);
	get_typlenbyvalalign(TEXTOID, &typlen, &typbyval, &typalign);
	result = construct_array((Datum *)ptr_values, array_count, TEXTOID, typlen, typbyval, typalign);
	PG_RETURN_ARRAYTYPE_P(result);
}

//
///*将字符串按指定的字数转换为tsvector*/
//Datum split_tsvector(PG_FUNCTION_ARGS) {
//	uint32 character_count;
//	int32 split_size;
//	text *in_txt;
//	text **ptr_values;
//
//	TSVector tsout;
//	Datum *dlexemes;
//	WordEntry  *arrout;
//	uint32 i, j;
//	int tslen, datalen = 0;
//	char *cur;
//
//	if (PG_ARGISNULL(0))
//		PG_RETURN_NULL();
//	in_txt = PG_GETARG_TEXT_P(0);
//	split_size = PG_GETARG_INT32(1);
//	if (split_size < 0 || split_size > 6) {
//		ereport(ERROR, (errcode(ERRCODE_INVALID_PARAMETER_VALUE),
//			errmsg("function returning fail(""%s"")!", "invalid parameter,split_size > 0 and split_size <= 6")));
//	}
//	ptr_values = split_string_v0(in_txt, split_size, &character_count);
//	/**/
//	dlexemes = (Datum *)ptr_values;
//	if (character_count > 1) {
//		qsort(dlexemes, character_count, sizeof(Datum), compare_text_lexemes);
//		/*去重*/
//		j = 0;
//		for (i = 1; i < character_count; i++) {
//			if (compare_text_lexemes(&dlexemes[j], &dlexemes[i]) < 0)
//				dlexemes[++j] = dlexemes[i];
//		}
//		character_count = ++j;
//	}
//	/* Calculate space needed for surviving lexemes. */
//	for (i = 0; i < character_count; i++)
//		datalen += VARSIZE(dlexemes[i]) - VARHDRSZ;
//	tslen = CALCDATASIZE(character_count, datalen);
//
//	/* Allocate and fill tsvector. */
//	tsout = (TSVector)palloc0(tslen);
//	SET_VARSIZE(tsout, tslen);
//	tsout->size = character_count;
//
//	arrout = ARRPTR(tsout);
//	cur = STRPTR(tsout);
//	for (i = 0; i < character_count; i++) {
//		char *lex = VARDATA(dlexemes[i]);
//		int lex_len = VARSIZE(dlexemes[i]) - VARHDRSZ;
//
//		memcpy(cur, lex, lex_len);
//		arrout[i].haspos = 0;
//		arrout[i].len = lex_len;
//		arrout[i].pos = (uint32)(cur - STRPTR(tsout));
//		cur += lex_len;
//	}
//	pfree(ptr_values);
//	PG_RETURN_POINTER(tsout);
//}

/*计算字符串的CRC32值*/
Datum generate_crc32_v1(PG_FUNCTION_ARGS) {
	text *in_txt = NULL;
	char *utf8_str = NULL;
	uint32 crc32_val = 0;
	/*获取参数并检查是否为''，为''时直接返回NULL*/
	in_txt = PG_GETARG_TEXT_P(0);
	utf8_str = (char *)VARDATA(in_txt);
	//utf8_str = text_to_cstring(PG_GETARG_TEXT_P(0));
	if ('\0' == utf8_str[0])
		PG_RETURN_NULL();
	crc32_val = generate_crc32(utf8_str);
	//elog(NOTICE, "%s,%u", utf8_str,crc32_val);
	PG_RETURN_INT64(crc32_val);
}


/*比较两个字符串的相似度*/
Datum string_simil(PG_FUNCTION_ARGS) {
	text *in_txt1, *in_txt2;
	text *tunpacked1, *tunpacked2;
	uint32 len1, len2;
	char *ptr_in_tmp1, *ptr_in_tmp2;
	float result;

	in_txt1 = PG_GETARG_TEXT_P(0);
	in_txt2 = PG_GETARG_TEXT_P(1);

	tunpacked1 = pg_detoast_datum_packed((struct varlena *) in_txt1);
	len1 = VARSIZE_ANY_EXHDR(tunpacked1);
	ptr_in_tmp1 = VARDATA_ANY(in_txt1);

	tunpacked2 = pg_detoast_datum_packed((struct varlena *) in_txt2);
	len2 = VARSIZE_ANY_EXHDR(tunpacked2);
	ptr_in_tmp2 = VARDATA_ANY(in_txt2);

	result = simil(ptr_in_tmp1, len1, ptr_in_tmp2, len2);
	PG_RETURN_FLOAT4(result);
}

/*获取两个字符串共同的部份*/
Datum string_same(PG_FUNCTION_ARGS) {
	text *in_txt1, *in_txt2, *result;
	char *ptr_in_tmp1, *ptr_in_tmp2;

	in_txt1 = PG_GETARG_TEXT_P(0);
	in_txt2 = PG_GETARG_TEXT_P(1);

	ptr_in_tmp1 = text_to_cstring(in_txt1);
	ptr_in_tmp2 = text_to_cstring(in_txt2);

	result = same(ptr_in_tmp1, (uint32)strlen(ptr_in_tmp1), ptr_in_tmp2, (uint32)strlen(ptr_in_tmp2));
	pfree(ptr_in_tmp1); pfree(ptr_in_tmp2);
	//char *data = (char *)result;
	//int size = VARSIZE(result) + VARHDRSZ;
	//for (int i = 0; i < size; ++i)
	//	elog(NOTICE, "0x%x", data[i]);
	//elog(NOTICE, "%d,", size);
	PG_RETURN_TEXT_P(PointerGetDatum(result));
}