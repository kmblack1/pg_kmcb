#ifndef H_4735388C_A36A_4553_90E3_D194E1E9DB6B
#define H_4735388C_A36A_4553_90E3_D194E1E9DB6B

/*����������·ݶ��ձ�,������ͨ�������*/
extern const unsigned short int SE_MON_YDAY[2][13];

/*
*	����year��dayOfYear�������ڶ���
*		year:						[in]			�������
*		dayOfYear:			[in]			��ȵĵڼ���
*		mon:						[out]			�·�
*		dayOfMonth:		[out]			�·ݵĵڼ���
*/
extern void generate_date_ex(int full_year, int dayOfYear, int *mon, int *dayOfMonth);

/*
*	���YYDOY
*		yy_doy:					[in]			��д��Ⱥ͵���ڼ���
*		year:						[out]			���
*		doy:						[out]			����ڼ���
*	��ע:
*		����ֻ������2000-2999��֮��
*/
extern void split_yy_doy_2000(int yy_doy, int *year, int *doy);

#endif /*H_4735388C_A36A_4553_90E3_D194E1E9DB6B*/