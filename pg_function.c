//#include <stdlib.h>
#include <time.h>
#ifdef _MSC_VER
#include <io.h>
#else
#include <sys/time.h>
#include <sys/resource.h>
#endif
#include <unistd.h>

#include "pg_function.h"
#include "funcapi.h"
#include "datatype/timestamp.h"
#include "utils/array.h"
#include "utils/date.h"
#include "utils/timestamp.h"
#include "utils/datetime.h"
#include "utils/builtins.h"
#include "utils/lsyscache.h"
#include "utils/snapmgr.h"
#include "utils/memutils.h"
#include "utils/typcache.h"
#include "utils/varbit.h"
#include "utils/guc.h"
#include "utils/datum.h"
#include "utils/nabstime.h"
#include "utils/geo_decls.h"
#include "storage/itemptr.h"
#include "catalog/pg_type.h"


#include "pgstat.h"
#include "access/htup_details.h"
#include "access/xact.h"
#include "access/hash.h"
#include "commands/async.h"
#include "executor/spi.h"
#include "executor/executor.h"
#include "miscadmin.h"
#include "storage/lwlock.h"
#include "storage/shmem.h"
#include "storage/spin.h"
#include "storage/ipc.h"
#include "storage/fd.h"
#include "storage/itemptr.h"
#include "lib/stringinfo.h"

#include "tsearch/ts_type.h"

#include "se_round.h"


#define MAX_HOST_SIZE (256)

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif


PG_FUNCTION_INFO_V1(byte_units);
PG_FUNCTION_INFO_V1(hostname);

/*��ָ�����ֽڴ�Сת��Ϊ����λ������ɶ��ĸ�ʽ*/
Datum byte_units(PG_FUNCTION_ARGS) {
	int64_t val = 0;
	text *ptr_text = NULL;

	val = PG_GETARG_INT64(0);
	ptr_text = se_byte_units(val);
	if (NULL == ptr_text)
		PG_RETURN_NULL();
	else
		PG_RETURN_TEXT_P(PointerGetDatum(ptr_text));
}

/*��ȡ������*/
Datum hostname(PG_FUNCTION_ARGS) {
	text *result;
	size_t len;
	char buf[MAX_HOST_SIZE + sizeof(char)];

	gethostname(buf, MAX_HOST_SIZE);
	len = strlen(buf);
	//elog(NOTICE, "hostname=%s\n",buf);
	result = (text *)palloc(len + VARHDRSZ);
	SET_VARSIZE(result, len + VARHDRSZ);
	memcpy(VARDATA(result), buf, len);
	PG_RETURN_TEXT_P(PointerGetDatum(result));
}







///*�������ANIS��ʽ�ַ���,�ַ�����������С��󳤶�֮��*/
//Datum generate_rand_anis_v1(PG_FUNCTION_ARGS) {
//	int32 min_len = PG_GETARG_INT32(0);
//	int32 max_len = PG_GETARG_INT32(1);
//	int code;
//	int usr_mem_size = 0;
//	/*����������ȵ��ַ���*/
//	int character_count = (random() % (max_len + 1 - min_len) + min_len);
//	/*Ϊ���ص�text�����ڴ�,ע��varattrib_4b����varlena
//	* ���ݲ�����NULL������,���ݴ�Сva_header��vl_len_(4�ֽ�)����
//	* ��˷����ڴ�ʱע��������VARHDRSZ((int32) sizeof(int32))
//	* �������ݵĴ�С��SET_VARSIZE,����������VARDATA
//	* ��c.h��postgres.h�ж���
//	*/
//	int alloc_size = character_count * 2 + VARHDRSZ;/*ANIS�ַ����2�ֽ�,*/
//	text *result = (text *)palloc(alloc_size);
//	if (SE_OK != (code = zh_random_anis(VARDATA(result), character_count, &usr_mem_size)))
//		ereport(ERROR, (errcode(ERRCODE_INVALID_PARAMETER_VALUE),
//			errmsg("function returning ""%s"" fail(""%s"")!",
//				"generate_rand_anis_v1",
//				SE_INVALID_PARAMETER_VALUE == code ? "Invalid parameter" : "min_len < 0 or max_len > 32768")
//			));
//	/*�������ݵ�ʵ�ʴ�С*/
//	SET_VARSIZE(result, usr_mem_size + VARHDRSZ);
//	PG_RETURN_TEXT_P(PointerGetDatum(result));
//}


//
///*if (NULL != result) {
//MemoryContext context;
//context = MemoryContextSwitchTo(old_context);
//result_text = cstring_to_text(result);
//MemoryContextSwitchTo(context);
//}*/
//void executeQuery(char* query) {
//	StartTransactionCommand();
//	SetCurrentStatementStartTimestamp();
//	SPI_connect();
//	PushActiveSnapshot(GetTransactionSnapshot());
//	SPI_exec(query, 0);
//	SPI_finish();
//	PopActiveSnapshot();
//	CommitTransactionCommand();
//	ProcessCompletedNotifies();
//}


