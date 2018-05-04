#include "postgres.h"
#include "fmgr.h"
#include "funcapi.h"
#include "utils/datetime.h"
#include "utils/date.h"
#include "access/htup_details.h"

#include "pg_function.h"
#include "se_datetime.h"

PG_FUNCTION_INFO_V1(get_yydoy_from_sn);
PG_FUNCTION_INFO_V1(get_id_from_sn);
PG_FUNCTION_INFO_V1(get_yydoy_id_from_sn);
PG_FUNCTION_INFO_V1(get_date_from_sn);

/*从YYDOY编号中获取YYDOY部分*/
Datum get_yydoy_from_sn(PG_FUNCTION_ARGS) {
	int64 yy_doy_id = PG_GETARG_INT64(0);
	int32 precision = PG_GETARG_INT32(1);

	int64 lpow = (int64)powl(10.0, precision);
	int yy_doy = (int)(yy_doy_id / lpow);
	PG_RETURN_INT32(yy_doy);
}
/*从YYDOY编号中获取ID部分*/
Datum get_id_from_sn(PG_FUNCTION_ARGS) {
	int64 yy_doy_id = PG_GETARG_INT64(0);
	int32 precision = PG_GETARG_INT32(1);

	int64 lpow = (int64)powl(10.0, precision);
	int yy_doy = (int)(yy_doy_id / lpow);
	int64 id = yy_doy_id - (yy_doy * lpow);
	PG_RETURN_INT64(id);
}
/*从YYDOY编号中获取YYDOY和ID部分(一次性取出)*/
Datum get_yydoy_id_from_sn(PG_FUNCTION_ARGS) {
	FuncCallContext *funcctx;
	uint64 call_cntr, max_calls;

	/* stuff done only on the first call of the function */
	if (SRF_IS_FIRSTCALL()) {
		MemoryContext oldcontext;
		/* create a function context for cross-call persistence*/
		funcctx = SRF_FIRSTCALL_INIT();
		/* switch to memory context appropriate for multiple function calls */
		oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);
		/* total number of tuples to be returned */
		funcctx->max_calls = PG_GETARG_UINT32(2);
		/* Build a tuple descriptor for our result type */
		if (get_call_result_type(fcinfo, NULL, &funcctx->tuple_desc) != TYPEFUNC_COMPOSITE)
			ereport(ERROR, (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
				errmsg("function returning record called in context that cannot accept type record")));
		/* generate attribute metadata needed later to produce tuples from raw C strings */
		BlessTupleDesc(funcctx->tuple_desc);
		MemoryContextSwitchTo(oldcontext);
	}
	/* stuff done on every call of the function */
	funcctx = SRF_PERCALL_SETUP();
	call_cntr = funcctx->call_cntr;
	max_calls = funcctx->max_calls;
	if (call_cntr < max_calls) {  /* do when there is more left to send */
		HeapTuple tuple;
		Datum result;
		Datum retvals[2];
		bool retnulls[2];

		int64 yy_doy_id = PG_GETARG_INT64(0);
		int32 precision = PG_GETARG_INT32(1);

		int64 lpow = (int64)powl(10.0, precision);
		int yy_doy = (int)((int64)(yy_doy_id / lpow));
		int64 id = yy_doy_id - (yy_doy * lpow);

		retvals[0] = Int32GetDatum(yy_doy);
		retvals[1] = Int64GetDatum(id);

		retnulls[0] = false;
		retnulls[1] = false;

		tuple = heap_form_tuple(funcctx->tuple_desc, retvals, retnulls);
		result = HeapTupleGetDatum(tuple);
		SRF_RETURN_NEXT(funcctx, result);
	} else {  /* do when there is no more left */
		SRF_RETURN_DONE(funcctx);
	}
}
/*从YYDOY编号中获取日期*/
Datum get_date_from_sn(PG_FUNCTION_ARGS) {
	int64 yy_doy_id = PG_GETARG_INT64(0);
	int32 precision = PG_GETARG_INT32(1);

	DateADT  result = 0;

	int64 lpow = (int64)powl(10.0, precision);
	int yydoy = (int)(yy_doy_id / lpow);
	int year, doy, month, day_of_month;
	split_yy_doy_2000(yydoy, &year, &doy);
	generate_date_ex(year, doy, &month, &day_of_month);

	if (!IS_VALID_JULIAN(year, month, day_of_month)) {
		ereport(ERROR,
			(errcode(ERRCODE_DATETIME_VALUE_OUT_OF_RANGE),
				errmsg("date out of range")));
	}
	result = (DateADT)(date2j(year, month, day_of_month) - POSTGRES_EPOCH_JDATE);
	PG_RETURN_DATEADT(result);
}