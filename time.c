#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tema3.h"


int main() {
    struct tm last_modified_time = {0};
    strptime("2019-12-17 19:39:39.888792925 +0200", "%Y-%m-%d %H:%M:%S", &last_modified_time);
   // strptime("2001-11-12 18:31:01", "%Y-%m-%d %H:%M:%S", &last_modified_time);
    //char *s = strptime("2019-12-17 19:39:39.888792925 +0200", "%Y-%m-%d %H:%M:%S", &last_modified_time);
    printf("Year = %d\nMonth= %d\nDay = %d\nHour = %d\nMinute = %d\nSeconds = %d\n", last_modified_time.tm_year, last_modified_time.tm_mon, last_modified_time.tm_mday, last_modified_time.tm_hour, last_modified_time.tm_min, last_modified_time.tm_sec);
    time_t rawtime = mktime(&last_modified_time);
    printf("RAWTIME = %ld\n", rawtime);
    return 0;
}
