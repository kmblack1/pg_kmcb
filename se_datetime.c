#include <time.h>
#include "postgres.h"
#include "fmgr.h"
#include "utils/datetime.h"
#include "se_std.h"
#include "se_datetime.h"

#define TM_YEAR_BASE 1900
#define MONTHS_IN_YEAR 12

const unsigned short int SE_MON_YDAY[2][13] = {
	/* Normal years.  */
	{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 },
	/* Leap years.  */
	{ 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 }
};


void generate_date_ex(int full_year, int dayOfYear, int *month, int *dayOfMonth) {
	int leapYearIndex = isleap(full_year) ? 1 : 0;
	int daysInYear = leapYearIndex ? 366 : 365;
	if (dayOfYear >= 1 && dayOfYear <= daysInYear) {
		for (int mon = 0; mon < MONTHS_IN_YEAR; mon++) {
			if (dayOfYear <= SE_MON_YDAY[leapYearIndex][mon + 1]) {
				*month = mon + 1;
				*dayOfMonth = dayOfYear - SE_MON_YDAY[leapYearIndex][mon];
				break;
			}
		}
	} else {
		*month = 0;
		*dayOfMonth = 0;
	}
	//struct tm timeinfo = { 0 };
	//timeinfo.tm_year = full_year - TM_YEAR_BASE;
	//timeinfo.tm_mon = month;
	//timeinfo.tm_mday = dayOfMonth;
	//return mktime(&timeinfo);
	//return *(localtime(&loctime) );
}

void split_yy_doy_2000(int yy_doy, int *year, int *doy) {
	int yy = (int)(yy_doy / 1000);
	*year = 2000 + yy;
	*doy = yy_doy - (yy * 1000);
}