#ifndef H_D0606DC4_7203_46F0_BA19_74F6DF24EF39
#define H_D0606DC4_7203_46F0_BA19_74F6DF24EF39
#include "se_std.h"
#	if !defined(SE_USE_POSTGRESQL)
#		include "postgres.h"
#	endif


/*生成指定范围的随机数*/
#	if !defined(SE_USE_POSTGRESQL)
#		define RANGE_RANDOM(lmin,lmax)(random() % (lmax + 1 - lmin) + lmin )
#	else
#		define RANGE_RANDOM(lmin,lmax)(rand() % (lmax + 1 - lmin) + lmin )
#	endif

/*
*	根据国标GB18030-2005生成1个汉字
*		str:						[in,out]		生成的汉字,固定为4字节大小
*	返回值:
*		UTF-8单个字符的内存大小
*	备注:
*		不要直接使用这个函数,请使用zh_random_utf8
*		UTF-8汉字范围 20901 - 19968
*/
extern size_t random_utf8_zh(int code, char *buffer);

/*
*	随机生成字母
*		flag:						[in]			生成标记
*													1:	仅生成数字
*													2:	仅生成大写字母
*													3:	仅生成数字或大写字母
*													4:	仅生成小写字母
*													5:	仅生成数字或小写字母
*													6:	仅生成大写字母或小写字母
*													7:	仅生成数字或大写字母或小写字母
*													8:	仅生成汉字
*													9:	仅生成数字或汉字
*													10:	仅生成大写字母或汉字
*													11:	仅生成数字或大写字母或汉字
*													12:	仅生成小写字母或汉字
*													13:	仅生成数字或小写字母或汉字
*													14:	仅生成大写字母或小写字母或汉字
*													其它:	随机生成数字,大小写字母
*		buffer:					[in,out]		生成的汉字,固定为4字节大小
*	返回值:
*		随机生成字母单个字符的内存大小
*	备注:
*		不要直接使用这个函数,请使用num_abc_zh_rand
*/
extern size_t rand_num_abc_zh(int flag, char *buffer);

/*
*	根据flag计算生成数字\大写字母\小写字母\汉字的组合
*/
extern int ckeck_flag(int flag);
/*
*	根据标记位随机生成数字大小写字母字符串,字符数量由character_count决定
*		flag:						[in]			生成标记
*		character_count:	[in]			要生成的字符数量
*		buffer:					[in,out]		已经分配好的内存缓冲区
*													注意内存大小至少为(character_count * 4+ VARHDRSZ)

*		mem_size:				[out]			实际使用的内存大小
*	返回值:
*		0表示成功
*		-1表示无效的参数(NULL == buffer)
*		-2表示character_count>32768个字符
*	备注:
*		buffer不包含文字结束符'\0'
*/
extern int num_abc_zh_rand(int flag, int character_count, char *buffer, size_t *use_mem_size);



/*
*	根据国标GB18030-2005生成1个汉字
*		str:						[in,out]		生成的汉字,固定为2字节大小
*	备注:
*		不要直接使用这个函数,请使用zh_random_anis
*/
extern void random_anis_zh(char *buffer);

/*
*	随机ANIS格式的中文字符串,中文字符数量由character_count决定
*		buffer:					[in,out]		已经分配好的内存缓冲区
*													注意内存大小至少为(character_count * 2 + VARHDRSZ)
*		character_count:	[in]			要生成的字符数量
*		mem_size:				[out]			实际使用的内存大小
*	返回值:
*		0表示成功
*		-1表示无效的参数(NULL == buffer || NULL == mem_size)
*		-2表示character_count>32768个字符
*	备注:
*		buffer不包含文字结束符'\0'
*		C语言在调用函数之前必须先调用srand((unsigned int)time(NULL))
*		PostgreSQL C不需要用调用srand
*		函数返回0成功,其它表示参数不正确
*/
extern int zh_random_anis(char *buffer, int character_count, size_t *use_mem_size);







#endif /*H_D0606DC4_7203_46F0_BA19_74F6DF24EF39*/