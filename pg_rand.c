#include "postgres.h"
#include "fmgr.h"
#include "pg_function.h"
#include "se_rand.h"

PG_FUNCTION_INFO_V1(generate_rand_string);

/*随机生成数字大小写字母组成的字符串*/
Datum generate_rand_string(PG_FUNCTION_ARGS) {
	int32 min_len = PG_GETARG_INT32(0);
	int32 max_len = PG_GETARG_INT32(1);
	int32 iswitch;
	int32 flag = PG_GETARG_INT32(2);
	int code, character_count;
	size_t usr_mem_size = 0, alloc_size;
	text *result;
	/*生成随机长度的字符串*/
	if ((min_len < 1 || max_len < 1) || (min_len > 0x8000 || max_len > 0x8000))
		ereport(ERROR, (errcode(ERRCODE_INVALID_PARAMETER_VALUE),
			errmsg("function returning ""%s"" fail(""%s"")!", "generate_rand_string",
				"min_len < 0 or max_len > 32768")
			));
	if (min_len > max_len) {
		iswitch = max_len;
		max_len = min_len;
		min_len = iswitch;
	}
	character_count = RANGE_RANDOM(min_len, max_len);
	/*为返回的text分配内存,注意varattrib_4b包含varlena
	* 数据不是以NULL结束的,数据大小va_header或vl_len_(4字节)决定
	* 因此分配内存时注意必须包含VARHDRSZ((int32) sizeof(int32))
	* 设置数据的大小用SET_VARSIZE,访问数据用VARDATA
	* 在c.h和postgres.h中定义
	*/
	alloc_size = (character_count * UTF8_MAX_SIZE);/*UTF-8最大6字节，但生成的汉字最多4字节，因此设置为4*/
	result = (text *)palloc(alloc_size + VARHDRSZ);
	if (SE_OK != (code = num_abc_zh_rand(flag, character_count, VARDATA(result), &usr_mem_size)))
		ereport(ERROR, (errcode(ERRCODE_INVALID_PARAMETER_VALUE),
			errmsg("function returning ""%s"" fail(""%s"")!", "generate_rand_string",
				SE_INVALID_PARAMETER_VALUE == code ? "invalid parameter" : "min_len < 0 or max_len > 32768")
			));
	/*设置数据的实际大小*/
	//elog(NOTICE, "%d,%d,%d", character_count, alloc_size, usr_mem_size);
	SET_VARSIZE(result, usr_mem_size + VARHDRSZ);
	PG_RETURN_TEXT_P(PointerGetDatum(result));
	//PG_RETURN_TEXT_P(PointerGetDatum(cstring_to_text(result)));

}