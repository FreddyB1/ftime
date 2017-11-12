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

native getdayname(year, month, day, string[], len = sizeof(string));
native cvstamp2datetime(timestamp, &year, &month, &day, &hour, &minute, &second, GMT);
native cvdatetime2stamp(&timestamp, year, month, day, hour, minute, second, GMT);
native isleapyear(year);
stock GetDayName(year, month, day)
{
	new day_n[64];
	getdayname(year, month, day, day_n, sizeof(day_n));
	return day_n;
}
//native cvdatetime2secs(minutes, hours, days, months, years);
#define cvdatetime2secs(%0,%1,%2,%3,%4) \ 
	((%0 * 60)+(%1 * 3600) + (%2 * 86400) + (%3 * 2678400) + (%4 * 31556952)) 
	
