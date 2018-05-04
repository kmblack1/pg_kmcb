#ifndef H_89F487F6_725E_4885_9112_80D1C4279F3B
#define H_89F487F6_725E_4885_9112_80D1C4279F3B

/*函数执行成功*/
#define SE_OK	(0)
/*无效的参数*/
#define SE_INVALID_PARAMETER_VALUE	(-1)
/*参数值超过了允许的范围*/
#define SE_PARAMETER_VALUE_OUT_OF_RANGE	(-2)

/*UTF-8字符长度查找表*/
extern const unsigned char utf8_look_for_table[];

/*UTF8单个字符占用的内存空间大小
*	UTF-8最大6字节，但生成的汉字最多4字节，因此设置为4
*/
#define  UTF8_MAX_SIZE 6

/*获取UTF8字符占用的内存大小（单位为字节）*/
#define UTF8LEN(x)  utf8_look_for_table[(x)]

/*获取ANIS字符占用的内存大小（单位为字节）*/
#define ANISLEN(x)  x < 0 ? 2 : 1

/*CRC32基本表*/
extern const unsigned int CRC32_BASE_TABLES[];

/*定义浮点数类型*/
typedef float float32_t;
typedef double float64_t;
typedef long double float128_t;


/*
*	计算UTF8字符串占用的内存大小（不包含\0）
*		str:						[in]		UTF-8字符串
*	返回值:
*		UTF-8字符串使用的内存大小,不包含\0
*/
extern size_t utf8_memory_size(const char *str, unsigned int str_mem_len);

/*
*	计算utf8字符数量
*		str:						[in]		UTF-8字符串
*	返回值:
*		UTF-8字符串的字符个数,英文和中文都算为1个字符
*/
extern int utf8_character_count(const char *str, unsigned int str_mem_len);

/*
*	计算ANIS字符串占用的内存大小（不包含\0）
*		str:						[in]		ANIS字符串
*	返回值:
*		ANIS字符串使用的内存大小,不包含\0
*/
extern size_t anis_memory_size(const char *str, unsigned int str_mem_len);

/*
*	计算ANIS字符数量
*		str:						[in]		ANIS字符串
*	返回值:
*		ANIS字符串的字符个数
*		ASCII值小于128的算1个字符,其它都算为2个字符
*/
extern int anis_character_count(const char *str, unsigned int str_mem_len);


/*
*	生成字符串CRC32校验码
*		input:					[in]		要生成CRC32校验码的字符串
*	返回值:
*		CRC32校验码
*/
extern unsigned int generate_crc32(const char *input);

#endif /*H_89F487F6_725E_4885_9112_80D1C4279F3B*/