#ifndef H_D0606DC4_7203_46F0_BA19_74F6DF24EF39
#define H_D0606DC4_7203_46F0_BA19_74F6DF24EF39
#include "se_std.h"
#	if !defined(SE_USE_POSTGRESQL)
#		include "postgres.h"
#	endif


/*����ָ����Χ�������*/
#	if !defined(SE_USE_POSTGRESQL)
#		define RANGE_RANDOM(lmin,lmax)(random() % (lmax + 1 - lmin) + lmin )
#	else
#		define RANGE_RANDOM(lmin,lmax)(rand() % (lmax + 1 - lmin) + lmin )
#	endif

/*
*	���ݹ���GB18030-2005����1������
*		str:						[in,out]		���ɵĺ���,�̶�Ϊ4�ֽڴ�С
*	����ֵ:
*		UTF-8�����ַ����ڴ��С
*	��ע:
*		��Ҫֱ��ʹ���������,��ʹ��zh_random_utf8
*		UTF-8���ַ�Χ 20901 - 19968
*/
extern size_t random_utf8_zh(int code, char *buffer);

/*
*	���������ĸ
*		flag:						[in]			���ɱ��
*													1:	����������
*													2:	�����ɴ�д��ĸ
*													3:	���������ֻ��д��ĸ
*													4:	������Сд��ĸ
*													5:	���������ֻ�Сд��ĸ
*													6:	�����ɴ�д��ĸ��Сд��ĸ
*													7:	���������ֻ��д��ĸ��Сд��ĸ
*													8:	�����ɺ���
*													9:	���������ֻ���
*													10:	�����ɴ�д��ĸ����
*													11:	���������ֻ��д��ĸ����
*													12:	������Сд��ĸ����
*													13:	���������ֻ�Сд��ĸ����
*													14:	�����ɴ�д��ĸ��Сд��ĸ����
*													����:	�����������,��Сд��ĸ
*		buffer:					[in,out]		���ɵĺ���,�̶�Ϊ4�ֽڴ�С
*	����ֵ:
*		���������ĸ�����ַ����ڴ��С
*	��ע:
*		��Ҫֱ��ʹ���������,��ʹ��num_abc_zh_rand
*/
extern size_t rand_num_abc_zh(int flag, char *buffer);

/*
*	����flag������������\��д��ĸ\Сд��ĸ\���ֵ����
*/
extern int ckeck_flag(int flag);
/*
*	���ݱ��λ����������ִ�Сд��ĸ�ַ���,�ַ�������character_count����
*		flag:						[in]			���ɱ��
*		character_count:	[in]			Ҫ���ɵ��ַ�����
*		buffer:					[in,out]		�Ѿ�����õ��ڴ滺����
*													ע���ڴ��С����Ϊ(character_count * 4+ VARHDRSZ)

*		mem_size:				[out]			ʵ��ʹ�õ��ڴ��С
*	����ֵ:
*		0��ʾ�ɹ�
*		-1��ʾ��Ч�Ĳ���(NULL == buffer)
*		-2��ʾcharacter_count>32768���ַ�
*	��ע:
*		buffer���������ֽ�����'\0'
*/
extern int num_abc_zh_rand(int flag, int character_count, char *buffer, size_t *use_mem_size);



/*
*	���ݹ���GB18030-2005����1������
*		str:						[in,out]		���ɵĺ���,�̶�Ϊ2�ֽڴ�С
*	��ע:
*		��Ҫֱ��ʹ���������,��ʹ��zh_random_anis
*/
extern void random_anis_zh(char *buffer);

/*
*	���ANIS��ʽ�������ַ���,�����ַ�������character_count����
*		buffer:					[in,out]		�Ѿ�����õ��ڴ滺����
*													ע���ڴ��С����Ϊ(character_count * 2 + VARHDRSZ)
*		character_count:	[in]			Ҫ���ɵ��ַ�����
*		mem_size:				[out]			ʵ��ʹ�õ��ڴ��С
*	����ֵ:
*		0��ʾ�ɹ�
*		-1��ʾ��Ч�Ĳ���(NULL == buffer || NULL == mem_size)
*		-2��ʾcharacter_count>32768���ַ�
*	��ע:
*		buffer���������ֽ�����'\0'
*		C�����ڵ��ú���֮ǰ�����ȵ���srand((unsigned int)time(NULL))
*		PostgreSQL C����Ҫ�õ���srand
*		��������0�ɹ�,������ʾ��������ȷ
*/
extern int zh_random_anis(char *buffer, int character_count, size_t *use_mem_size);







#endif /*H_D0606DC4_7203_46F0_BA19_74F6DF24EF39*/