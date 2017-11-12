/*
Copyright [2015] [ThreeKingz]

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include "../SDK/plugin.h"

typedef void
(*logprintf_t)(char* format, ...)
;

logprintf_t
logprintf
;

void
**ppPluginData
;

extern void
*pAMXFunctions
;

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf = (logprintf_t)ppData[PLUGIN_DATA_LOGPRINTF];
	logprintf("	FTIME v2.0 loaded!");
	return 1;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
}


PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX *amx)
{
	return AMX_ERR_NONE;
}

static const unsigned char monthdays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

#define SECONDS_PER_MINUTE	60
#define SECONDS_PER_HOUR	3600
#define SECONDS_PER_DAY		86400
#define SECONDS_PER_YEAR	31556952	
#define NULL				0

bool c_IsLeapYear(int year)
{
	if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0)) return true;
	return false;
}

//Author: @Compuphase:
//Source available at: https://github.com/compuphase/pawn/blob/master/amx/amxtime.c
//ThreeKingz: Added GMT parameter.
void v_stamp2datetime(unsigned long sec1970, int *year, int *month, int *day, int *hour, int *minute, int *second, int GMT)
{
	if (GMT > 12 || GMT < -12) return;
	int days, seconds;
	/* find the year */
	for (*year = 1970; ; *year += 1)
	{
		days = 365 + (c_IsLeapYear(*year) != 0);
		seconds = days * SECONDS_PER_DAY;
		if ((unsigned long)seconds > sec1970)
			break;
		sec1970 -= seconds;
	} /* if */
	  /* find the month */
	for (*month = 1; ; *month += 1) {
		days = monthdays[*month - 1];
		seconds = days * SECONDS_PER_DAY;
		if ((unsigned long)seconds > sec1970) break;
		sec1970 -= seconds;
	} /* if */
	  /* find the day */
	for (*day = 1; sec1970 >= SECONDS_PER_DAY; *day += 1)
	{
		sec1970 -= SECONDS_PER_DAY;
	}
	/* find the hour */
	for (*hour = 0; sec1970 >= SECONDS_PER_HOUR; *hour += 1)
		sec1970 -= SECONDS_PER_HOUR;
	/* Check for GMT */
	if (*hour == 0)
	{
		*hour = 24;
		*hour += GMT;
	}
	else
	{
		*hour += GMT;
		if (*hour < 0)
		{
			//1 - 5 = -4
			*hour += 24;
			*day -= 1;
		}
		else if (*hour > 24)
		{
			*hour -= 24;
			*day += 1;
		}
	}
	/* find the minute */
	for (*minute = 0; sec1970 >= SECONDS_PER_MINUTE; *minute += 1)
		sec1970 -= SECONDS_PER_MINUTE;
	/* remainder is the number of seconds */
	*second = sec1970;
}
//Author: @Compuphase:
//Source available at: https://github.com/compuphase/pawn/blob/master/amx/amxtime.c
//ThreeKingz: Added GMT parameter.
int v_datetime2stamp(int year, int month, int day, int hour, int minute, int second, int GMT)
{
	unsigned long sec1970 = 0;
	for (int y = 1970; y < year; y++)
		day += (365 + c_IsLeapYear(y));

	for (int m = 1; m < month; m++)
		day += monthdays[m - 1];

	day -= 1;
	sec1970 += (day * SECONDS_PER_DAY);
	hour -= GMT;
	sec1970 += ((hour)* SECONDS_PER_HOUR);
	sec1970 += ((minute)* SECONDS_PER_MINUTE);
	sec1970 += second;
	return sec1970;
}
//Author: Tomohiko Sakamoto
int v_getdaynumber(int year, int month, int day)
{
	char t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
	year -= (month < 3);
	return (year + year / 4 - year / 100 + year / 400 + t[month - 1] + day) % 7;
}
//native cvdatetime2stamp(&timestamp, year, month, day, hour, minute, second, GMT = 0);
static cell AMX_NATIVE_CALL n_cvdatetime2stamp(AMX *amx, cell *params)
{
	if (params[8] > 12 || params[8] < -12) //gmt
	{
		logprintf("[cvdatetime2stamp] GMT can't be greater than 12 or less than -12.");
		return 0;
	}
	cell* addr = NULL;
	amx_GetAddr(amx, params[1], &addr);
	*addr = v_datetime2stamp(params[2], params[3], params[4], params[5], params[6], params[7], params[8]);
	return 1;
}
//native cvstamp2datetime(timestamp, &year, &month, &day, &hour, &minute, &second, GMT = -5);
static cell AMX_NATIVE_CALL n_cvstamp2datetime(AMX *amx, cell *params)
{
	if (params[1] == NULL) return 0;
	if (params[8] > 12 || params[8] < -12) //gmt
	{
		logprintf("[cvstamp2datetime] GMT can't be greater than 12 or less than -12.");
		return 0;
	}
	int year, month, day, hour, minute, second;
	v_stamp2datetime(params[1], &year, &month, &day, &hour, &minute, &second, params[8]);
	cell* addr[6] = { NULL, NULL, NULL, NULL, NULL };
	amx_GetAddr(amx, params[2], &addr[0]);
	amx_GetAddr(amx, params[3], &addr[1]);
	amx_GetAddr(amx, params[4], &addr[2]);
	amx_GetAddr(amx, params[5], &addr[3]);
	amx_GetAddr(amx, params[6], &addr[4]);
	amx_GetAddr(amx, params[7], &addr[5]);
	*addr[0] = year;
	*addr[1] = month;
	*addr[2] = day;
	*addr[3] = hour;
	*addr[4] = minute;
	*addr[5] = second;
	return 1;
}

//native getdayname(year, month, day, string[], len = sizeof(string));
static cell AMX_NATIVE_CALL n_getdayname(AMX *amx, cell *params)
{
	int dnumber;
	dnumber = v_getdaynumber(params[1], params[2], params[3]);
	cell* addr = NULL;
	amx_GetAddr(amx, params[4], &addr);
	switch (dnumber)
	{
	case 0: amx_SetString(addr, "Sunday", 0, 0, params[4]);
		break;
	case 1: amx_SetString(addr, "Monday", 0, 0, params[4]);
		break;
	case 2: amx_SetString(addr, "Tuesday", 0, 0, params[4]);
		break;
	case 3: amx_SetString(addr, "Wednesday", 0, 0, params[4]);
		break;
	case 4: amx_SetString(addr, "Thursday", 0, 0, params[4]);
		break;
	case 5: amx_SetString(addr, "Friday", 0, 0, params[4]);
		break;
	case 6: amx_SetString(addr, "Saturday", 0, 0, params[4]);
		break;
	}
	return 1;
}
//native bool:isleapyear(year);
static cell AMX_NATIVE_CALL n_isleapyear(AMX *amx, cell *params)
{
	if (params[1] == NULL) return 0;
	cell* addr = NULL;
	return c_IsLeapYear(params[1]);
}
AMX_NATIVE_INFO projectNatives[] =
{
	{ "cvstamp2datetime", n_cvstamp2datetime },
	{ "cvdatetime2stamp", n_cvdatetime2stamp },
	{ "getdayname",  n_getdayname },
	{ "isleapyear",  n_isleapyear },
	{ NULL, NULL }
};
PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *amx)
{
	return amx_Register(amx, projectNatives, -1);
}
