// Copyright 2019 Radu-Stefan Minea 314CA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tema3.h"

int to_octal(int *x) {
    int rest, octal = 0;
    while(x) {
        rest = x - ((x / 8) * 8);
        octal = octal * 10 + rest;
        x /= 8;
    }
    return octal;
}