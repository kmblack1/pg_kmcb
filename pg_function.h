#ifndef H_3069A164_35BD_4687_868B_1F5B20E2030E
#define H_3069A164_35BD_4687_868B_1F5B20E2030E
#include <stdarg.h>
#include "postgres.h"
#include "fmgr.h"


/*小数修约*/
PGDLLEXPORT Datum round_v1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum round2str_v1(PG_FUNCTION_ARGS);
/*四舍六入五考虑*/
PGDLLEXPORT Datum round_v2(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum round2str_v2(PG_FUNCTION_ARGS);
/*将指定的字节大小转换为带单位的人类可读的格式 */
PGDLLEXPORT Datum byte_units(PG_FUNCTION_ARGS);
/*获取主机名*/
PGDLLEXPORT Datum hostname(PG_FUNCTION_ARGS);

/*字符串处理*/
PGDLLEXPORT Datum split_string(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum generate_crc32_v1(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum string_simil(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum string_same(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum generate_rand_string(PG_FUNCTION_ARGS);
/*加密解密*/
PGDLLEXPORT Datum generate_aes_key(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum generate_aes_iv(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum aes_encrypt(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum aes_decrypt(PG_FUNCTION_ARGS);
/*ID和日期*/
PGDLLEXPORT Datum get_yydoy_from_sn(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum get_id_from_sn(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum get_yydoy_id_from_sn(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum get_date_from_sn(PG_FUNCTION_ARGS);

#endif /*H_3069A164_35BD_4687_868B_1F5B20E2030E*/