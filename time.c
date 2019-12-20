#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tema3.h"

int main() {
    /*time_t rawtime;
    time(&rawtime);
    printf("Rawtime = %ld\n", rawtime);

    struct tm timeinfo;
    memset(&timeinfo, '\0', sizeof(timeinfo));
    mktime(&timeinfo);
    printf("Mktime = %p\n", timeinfo);*/

    struct tm last_modified_time;

    char input_date[] = {"2019-12-17 19:39:39.888792925 +0200"}, input_time[] = {"2019-12-17 19:39:39.888792925 +0200"};
    char space[] = {" "}, dash[] = {"-"}, colon[] = {":"};

// year
    char *p = strtok(input_date, space);
    last_modified_time.tm_year = atoi(strtok(p, dash));
// month
    last_modified_time.tm_mon = atoi(strtok(NULL, dash));
// day
    last_modified_time.tm_mday = atoi(strtok(NULL, space));
//hour
    p = strtok(input_time, space);
   /* p = strtok(input_time, space);
    p = strtok(NULL, space);
    p = strtok(NULL, colon);
    //last_modified_time.tm_hour = atoi(strtok(NULL, colon));
    puts(p);
    //printf("%d\n", last_modified_time.tm_hour); */
    return 0;
}
