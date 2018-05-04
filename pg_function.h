#ifndef H_3069A164_35BD_4687_868B_1F5B20E2030E
#define H_3069A164_35BD_4687_868B_1F5B20E2030E
#include <stdarg.h>
#include "postgres.h"
#include "fmgr.h"


/*С����Լ*/
PGDLLEXPORT Datum round_v1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum round2str_v1(PG_FUNCTION_ARGS);
/*���������忼��*/
PGDLLEXPORT Datum round_v2(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum round2str_v2(PG_FUNCTION_ARGS);
/*��ָ�����ֽڴ�Сת��Ϊ����λ������ɶ��ĸ�ʽ */
PGDLLEXPORT Datum byte_units(PG_FUNCTION_ARGS);
/*��ȡ������*/
PGDLLEXPORT Datum hostname(PG_FUNCTION_ARGS);

/*�ַ�������*/
PGDLLEXPORT Datum split_string(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum generate_crc32_v1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum string_simil(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum string_same(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum generate_rand_string(PG_FUNCTION_ARGS);
/*���ܽ���*/
PGDLLEXPORT Datum generate_aes_key(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum generate_aes_iv(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum aes_encrypt(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum aes_decrypt(PG_FUNCTION_ARGS);
/*ID������*/
PGDLLEXPORT Datum get_yydoy_from_sn(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum get_id_from_sn(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum get_yydoy_id_from_sn(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum get_date_from_sn(PG_FUNCTION_ARGS);

#endif /*H_3069A164_35BD_4687_868B_1F5B20E2030E*/