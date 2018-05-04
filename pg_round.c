#include "postgres.h"
#include "fmgr.h"
#include "pg_function.h"
#include "se_round.h"

PG_FUNCTION_INFO_V1(round_v1);
PG_FUNCTION_INFO_V1(round2str_v1);
PG_FUNCTION_INFO_V1(round_v2);
PG_FUNCTION_INFO_V1(round2str_v2);

/*四舍五入*/
Datum round_v1(PG_FUNCTION_ARGS) {
	float8 val = 0;
	uint32_t  precision = 0;
	val = PG_GETARG_FLOAT8(0);
	precision = PG_GETARG_INT32(1);
	PG_RETURN_FLOAT8(se_round_v1(val, precision));
}

/*四舍五入*/
Datum round2str_v1(PG_FUNCTION_ARGS) {
	float8 val = 0;
	int32_t  precision = 0;
	text *ptr_text = NULL;

	val = PG_GETARG_FLOAT8(0);
	precision = PG_GETARG_INT32(1);
	ptr_text = se_round_format(se_round_v1(val, precision), precision);
	if (NULL == ptr_text)
		PG_RETURN_NULL();
	else
		PG_RETURN_TEXT_P(PointerGetDatum(ptr_text));
}

/*四舍六入五考虑*/
Datum round_v2(PG_FUNCTION_ARGS) {
	float8 val = 0;
	int32_t  precision = 0;
	val = PG_GETARG_FLOAT8(0);
	precision = PG_GETARG_INT32(1);
	PG_RETURN_FLOAT8(se_round_v2(val, precision));
}

/*四舍六入五考虑*/
Datum round2str_v2(PG_FUNCTION_ARGS) {
	float8 val = 0;
	int32_t  precision = 0;
	text *ptr_text = NULL;

	val = PG_GETARG_FLOAT8(0);
	precision = PG_GETARG_INT32(1);
	ptr_text = se_round_format(se_round_v2(val, precision), precision);
	//elog(NOTICE, "%s", VARDATA(ptr_text));
	if (NULL == ptr_text)
		PG_RETURN_NULL();
	else
		PG_RETURN_TEXT_P(PointerGetDatum(ptr_text));
}
