#ifndef H_4735388C_A36A_4553_90E3_D194E1E9DB6B
#define H_4735388C_A36A_4553_90E3_D194E1E9DB6B

/*年的天数和月份对照表,包含普通年和闰年*/
extern const unsigned short int SE_MON_YDAY[2][13];

/*
*	根据year和dayOfYear构造日期对象
*		year:						[in]			完整年份
*		dayOfYear:			[in]			年度的第几天
*		mon:						[out]			月份
*		dayOfMonth:		[out]			月份的第几天
*/
extern void generate_date_ex(int full_year, int dayOfYear, int *mon, int *dayOfMonth);

/*
*	拆分YYDOY
*		yy_doy:					[in]			缩写年度和当年第几天
*		year:						[out]			年度
*		doy:						[out]			当年第几天
*	备注:
*		函数只能用于2000-2999年之间
*/
extern void split_yy_doy_2000(int yy_doy, int *year, int *doy);

#endif /*H_4735388C_A36A_4553_90E3_D194E1E9DB6B*/