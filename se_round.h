#ifndef H_4613EEE7_145F_4E05_AA98_EFF1E3F0B038
#define H_4613EEE7_145F_4E05_AA98_EFF1E3F0B038

#include <stdint.h>
#include "postgres.h"
#include "se_std.h"

/*���ָ�ʽ��Ϊ�ַ���������С*/
#define SE_MAX_NUMSTR (64)
/*���ָ�ʽ��Ϊ�ַ���ʽ���ַ�����������С*/
#define SE_MAX_FORMAT (16)


/*
*	��������
*		val:						[in]		Ҫ��Լ��С��
*		precision:				[in]		С������(������λС��)
*	����ֵ:
*		��Լ���С��,round_v1(3.1415426,4)��ʾ3.141542��Լ����4λС��
*/
extern float128_t se_round_v1(float128_t val, uint32_t precision);

/*
*	���������忼��
*		val:						[in]		Ҫ��Լ��С��
*		precision:				[in]		С������(������λС��)
*	����ֵ:
*		��Լ���С��,round_v2(3.1415426,4)��ʾ3.141542��Լ����4λС��
*	��ע:
*		����Чλ����һλ�����֡�4ʱ����������ֱ����ȥ��
*		����Чλ����һλ�����֡�6ʱ�����������ȥ����1��
*		����Чλ����һλ������=5ʱ��Ҫ�����������
*			5����һλ���������������0������5��1��
*			5����һλ�����������0����Ҫ�����������
*				5��ǰ�����������������5��1
*				5��ǰ�������ż��������5����
*/
extern float128_t se_round_v2(float128_t val, uint32_t precision);

/*
*	��ʽ���������ݵ�ָ����С��λ��
*	ע���ͷ��ڴ�ʱҪfree(ptr_buf - SE_MAX_FORMAT);
*/
extern text *se_round_format(float128_t val, uint32_t precision);


/*��ָ�����ֽڴ�Сת��Ϊ����λ������ɶ��ĸ�ʽ */
extern text *se_byte_units(int64_t val);
#endif /*H_4613EEE7_145F_4E05_AA98_EFF1E3F0B038*/