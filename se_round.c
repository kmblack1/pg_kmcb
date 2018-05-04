#include <math.h>
#include "se_round.h"

text *byte_format(float128_t val, uint32_t precision, uint32_t current_index);
float128_t byte_round(float128_t val, uint32_t *precision);

/*
*	格式化浮点数到指定的小数位数
*	注意释放内存时要free(ptr_buf - SE_MAX_FORMAT);
*/
text *se_round_format(float128_t val, uint32_t precision) {
	char *ptr_format = NULL, *ptr_buf = NULL;
	text *ptr_txt = NULL;
	//elog(NOTICE, "%llf,%lu", val, precision);
	if (precision > 32 )
		return NULL;

	/*palloc不需要检查是否分配成功,内部已经检查*/
	ptr_format = (char *)palloc(SE_MAX_FORMAT);
	ptr_txt = (text *)palloc(VARHDRSZ + SE_MAX_NUMSTR);
	ptr_buf = VARDATA(ptr_txt);

	snprintf(ptr_format, SE_MAX_FORMAT, "%%.0%dllf", precision);
	snprintf(ptr_buf, SE_MAX_NUMSTR, ptr_format, val);
	pfree(ptr_format);
	SET_VARSIZE(ptr_txt, VARHDRSZ + strlen(ptr_buf));
	//elog(NOTICE, "%s", ptr_buf);
	return ptr_txt;
}

/*
四舍五入
*/
float128_t se_round_v1(float128_t val, uint32_t precision) {
	float128_t ratio = pow(10.0, (float128_t)(precision));
	float128_t x = val * ratio;
	int64_t ival = (int64_t)x;
	/*获取有效的小数*/
	float128_t m1 = x - (float128_t)ival;
	return (m1 >= 0.5) ? ((ival + 1) / ratio) : (ival / ratio);
}

/*
四舍六入五考虑
当有效位数后一位的数字≤4时，其后的数字直接舍去；
当有效位数后一位的数字≥6时，其后数字舍去并进1；
当有效位数后一位的数字=5时，要分两种情况：
5后面一位的数字如果不等于0，则舍5进1；
5后面一位数字如果等于0，又要分两种情况：
5的前面如果是奇数，则舍5进1
5的前面如果是偶数，则舍5不进
*/
float128_t se_round_v2(float128_t val, uint32_t precision) {
	float128_t ratio = pow(10.0, (float128_t)(precision));
	float128_t x = val * ratio;
	int64_t ival = (int64_t)x;

	/*获取有效的小数*/
	float128_t m1 = x - (float128_t)ival;
	float128_t m2 = m1 * 100.0;
	/*获取有效的小数之后的小数位*/
	int32_t dm = (int32_t)m2;
	float128_t mod = (float128_t)dm / 100;

	float128_t result = 0.0;
	if (mod >= 0.6) {	/*六入*/
		result = (ival + 1) / ratio;
	} else if (mod < 0.5) {		/*四舍*/
		result = ival / ratio;
	} else {
		if (0.50 == mod) {		/*5后面一位数字如果等于0，又要分两种情况*/
			int32_t f = (int32_t)(ival % 10);/*获取个位值*/
			result = (0 == (f % 2) ? ival : ival + 1) / ratio;
		} else {		/*5后面一位的数字如果不等于0，则舍5进1*/
			result = (ival + 1) / ratio;
		}
	}
	return result;
}

/*
*	字节大小格式化浮点数据到指定的小数位数
*	注意释放内存时要free(ptr_buf - SE_MAX_FORMAT);
*/
text *byte_format(float128_t val, uint32_t precision, uint32_t current_index) {
	static char *units[] = { "B", "KB", "MB", "GB", "TB","PB" };
	char *ptr_format = NULL, *ptr_buf = NULL;
	text *ptr_txt = NULL;

	if (precision > 2 )
		return NULL;
	current_index = (current_index > 5 ? 5 : current_index);

	ptr_format = (char *)palloc(SE_MAX_FORMAT);
	ptr_txt = (text *)palloc(VARHDRSZ + SE_MAX_NUMSTR);
	ptr_buf = VARDATA(ptr_txt);

	if (precision > 0) {
		snprintf(ptr_format, SE_MAX_FORMAT, "%%.0%dllf%s", precision, units[current_index]);
	} else {
		snprintf(ptr_format, SE_MAX_FORMAT, "%%.0llf%s", units[current_index]);
	}
	snprintf(ptr_buf, SE_MAX_NUMSTR, ptr_format, val);

	pfree(ptr_format);
	SET_VARSIZE(ptr_txt, VARHDRSZ + strlen(ptr_buf));
	return ptr_txt;
}

/*字节大小四舍五入*/
float128_t byte_round(float128_t val, uint32_t *precision) {
	float128_t ratio = powl(10.0, (float128_t)(*precision));
	float128_t x = val * ratio;
	int64_t ival = (int64_t)x;
	/*获取有效的小数*/
	float128_t m1 = x - (float128_t)ival;
	if (0 == m1) {
		(*precision) = 0;
		return val;
	} else {
		return (m1 >= 0.5 ? ((ival + 1) / ratio) : (ival / ratio));
	}
}

/*将指定的字节大小转换为带单位的人类可读的格式 */
text *se_byte_units(int64_t val) {
	uint32_t current_index = 0, precision = 0;
	float128_t dval = (float128_t)val, dresult = 0, result = 0;
	current_index = (uint32_t)(logl(dval) / logl(1024));

	precision = (current_index > 2 ? 2 : (current_index == 2 ? 1 : 0));
	dresult = dval / powl(1024, current_index);
	result = byte_round(dresult, &precision);
	return result > 0 ? byte_format(result, precision, current_index) : NULL;
}