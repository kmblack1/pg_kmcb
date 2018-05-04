#ifndef H_89F487F6_725E_4885_9112_80D1C4279F3B
#define H_89F487F6_725E_4885_9112_80D1C4279F3B

/*����ִ�гɹ�*/
#define SE_OK	(0)
/*��Ч�Ĳ���*/
#define SE_INVALID_PARAMETER_VALUE	(-1)
/*����ֵ����������ķ�Χ*/
#define SE_PARAMETER_VALUE_OUT_OF_RANGE	(-2)

/*UTF-8�ַ����Ȳ��ұ�*/
extern const unsigned char utf8_look_for_table[];

/*UTF8�����ַ�ռ�õ��ڴ�ռ��С
*	UTF-8���6�ֽڣ������ɵĺ������4�ֽڣ��������Ϊ4
*/
#define  UTF8_MAX_SIZE 6

/*��ȡUTF8�ַ�ռ�õ��ڴ��С����λΪ�ֽڣ�*/
#define UTF8LEN(x)  utf8_look_for_table[(x)]

/*��ȡANIS�ַ�ռ�õ��ڴ��С����λΪ�ֽڣ�*/
#define ANISLEN(x)  x < 0 ? 2 : 1

/*CRC32������*/
extern const unsigned int CRC32_BASE_TABLES[];

/*���帡��������*/
typedef float float32_t;
typedef double float64_t;
typedef long double float128_t;


/*
*	����UTF8�ַ���ռ�õ��ڴ��С��������\0��
*		str:						[in]		UTF-8�ַ���
*	����ֵ:
*		UTF-8�ַ���ʹ�õ��ڴ��С,������\0
*/
extern size_t utf8_memory_size(const char *str, unsigned int str_mem_len);

/*
*	����utf8�ַ�����
*		str:						[in]		UTF-8�ַ���
*	����ֵ:
*		UTF-8�ַ������ַ�����,Ӣ�ĺ����Ķ���Ϊ1���ַ�
*/
extern int utf8_character_count(const char *str, unsigned int str_mem_len);

/*
*	����ANIS�ַ���ռ�õ��ڴ��С��������\0��
*		str:						[in]		ANIS�ַ���
*	����ֵ:
*		ANIS�ַ���ʹ�õ��ڴ��С,������\0
*/
extern size_t anis_memory_size(const char *str, unsigned int str_mem_len);

/*
*	����ANIS�ַ�����
*		str:						[in]		ANIS�ַ���
*	����ֵ:
*		ANIS�ַ������ַ�����
*		ASCIIֵС��128����1���ַ�,��������Ϊ2���ַ�
*/
extern int anis_character_count(const char *str, unsigned int str_mem_len);


/*
*	�����ַ���CRC32У����
*		input:					[in]		Ҫ����CRC32У������ַ���
*	����ֵ:
*		CRC32У����
*/
extern unsigned int generate_crc32(const char *input);

#endif /*H_89F487F6_725E_4885_9112_80D1C4279F3B*/