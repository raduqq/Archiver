// Copyright 2019 Radu-Stefan Minea 314CA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tema3.h"

void opened_file_check(FILE *f) {
    if(f == NULL) {
        printf("< Failed!\n");
        exit(-1);
    }
}

void get_mode(char *mode, char *p) {
    unsigned int i, j;
    char space[] = {" "};    
    memset(mode, '0', sizeof(mode));
    for (i = 4; i < 7; i++) {
      for (j = (i - 4) * 3 + 1; j <= (i - 3) * 3; j++) {
        if (j % 3 == 1 && p[j] == 'r') {
          mode[i] += 4;
        }
        if (j % 3 == 2 && p[j] == 'w') {
          mode[i] += 2;
        }
        if (j % 3 == 0 && p[j] == 'x') {
          mode[i] += 1;
        }
      }
    }
    mode[sizeof(mode) - 1] = '\0';
}
