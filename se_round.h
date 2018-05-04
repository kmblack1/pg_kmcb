#ifndef H_4613EEE7_145F_4E05_AA98_EFF1E3F0B038
#define H_4613EEE7_145F_4E05_AA98_EFF1E3F0B038

#include <stdint.h>
#include "postgres.h"
#include "se_std.h"

/*数字格式化为字符缓冲区大小*/
#define SE_MAX_NUMSTR (64)
/*数字格式化为字符格式化字符串缓冲区大小*/
#define SE_MAX_FORMAT (16)


/*
*	四舍五入
*		val:						[in]		要修约的小数
*		precision:				[in]		小数精度(保留几位小数)
*	返回值:
*		修约后的小数,round_v1(3.1415426,4)表示3.141542修约后保留4位小数
*/
extern float128_t se_round_v1(float128_t val, uint32_t precision);

/*
*	四舍六入五考虑
*		val:						[in]		要修约的小数
*		precision:				[in]		小数精度(保留几位小数)
*	返回值:
*		修约后的小数,round_v2(3.1415426,4)表示3.141542修约后保留4位小数
*	备注:
*		当有效位数后一位的数字≤4时，其后的数字直接舍去；
*		当有效位数后一位的数字≥6时，其后数字舍去并进1；
*		当有效位数后一位的数字=5时，要分两种情况：
*			5后面一位的数字如果不等于0，则舍5进1；
*			5后面一位数字如果等于0，又要分两种情况：
*				5的前面如果是奇数，则舍5进1
*				5的前面如果是偶数，则舍5不进
*/
extern float128_t se_round_v2(float128_t val, uint32_t precision);

/*
*	格式化浮点数据到指定的小数位数
*	注意释放内存时要free(ptr_buf - SE_MAX_FORMAT);
*/
extern text *se_round_format(float128_t val, uint32_t precision);


/*将指定的字节大小转换为带单位的人类可读的格式 */
extern text *se_byte_units(int64_t val);
#endif /*H_4613EEE7_145F_4E05_AA98_EFF1E3F0B038*/