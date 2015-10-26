# ftime
Unix timestamp conversions and more time functions for PAWN!

#Download

Go to the [releases page](https://github.com/ThreeKingz/ftime/releases) and download the latest ftime.zip package which includes ftime.dll and ftime.inc


#Installation

- Decompress the ftime.zip
- Place the ftime.inc in your includes folder and ftime.dll in your plugins folder.
- Include the ftime.inc in your PAWN script.
- 
```pawn
#include <ftime> 
```

#Functions and their usage:

###getdayname

#####Description: 
      Gets a day's name given a year, month and day.
####Parameters:
```pawn
(year, month, day, string[], len = sizeof(string))
```
* `year`                 Self explanatory.
* `month`                Self explanatory.
* `day`                  Self explanatory.
* `string[]`             An array into which to store the day's name, passed by reference.
* `len`                  The length of the string that should be stored. Recommended to be '10'.

####Return values:
    Always returns 1.

```pawn
new year = 2015, month = 1, day = 5, dName[10];
getdayname(year, month, day, dName);
printf("%d/%d/%d was %s.", day, month, year, dName);
```
Outputs: `5/1/2015 was Monday.`

###cvstamp2datetime

#####Description: 
      Converts a unix timestamp to date-time(year, month, day, hour, minute, second)
####Parameters:
```pawn
(timestamp, &year, &month, &day, &hour, &minute, &second, GMT);
```
* `timestamp`            The unix timestamp you want to perform the conversion from.
* `&year`                The variable to store the year in, passed by reference.
* `&month`               The variable to store the month in, passed by reference.
* `&day`                 The variable to store the day in, passed by reference.
* `&hour`                The variable to store the hour in, passed by reference.
* `&minute`              The variable to store the minute in, passed by reference.
* `&second`              The variable to store the second in, passed by reference.
* `GMT`                  The GMT in your location.

####Return values:
    Always returns 1.

```pawn
new timestamp = 1445842664, year, month, day, hour, minute, second;
cvstamp2datetime(timestamp, year, month, day, hour, minute, second, -5);
printf("[Time] %d:%d:%d | [Date] %d/%d/%d", hour, minute, second, day, month, year);
```
Outputs: `[Time] 1:57:44 | [Date] 26/10/2015`


###cvdatetime2stamp

#####Description: 
      Converts a date-time(year, month, day, hour, minute, second) into a unix timestamp.
####Parameters:
```pawn
(&timestamp, year, month, day, hour, minute, second, GMT);
```
* `&timestamp`            The variable to store the unix timestamp in, passed by reference.
* `year`                Self explanatory.
* `month`              Self explanatory.
* `day`                 Self explanatory.
* `hour`                Self explanatory.
* `minute`              Self explanatory.
* `second`             Self explanatory.
* `GMT`                  The GMT in your location.

####Return values:
    Always returns 1.

```pawn
new timestamp, year = 2015, month = 10, day = 26, hour = 1, minute = 57, second = 44;
	cvdatetime2stamp(timestamp, year, month, day, hour, minute, second, -5);
	printf("[Time] %d:%d:%d | [Date] %d/%d/%d outputs %d as an unix timestamp.", hour, minute, second, day, month, year, timestamp);
```
Outputs: `[Time] 1:57:44 | [Date] 26/10/2015 outputs 1445842664 as an unix timestamp.`



