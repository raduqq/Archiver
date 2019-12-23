#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RECORDSIZE 512

void function(union record filedata, char *field) {
    memeset(filedata.header.field, '0', sizeof(filedata.header.field));
}

int main() {
    union record filedata;
    function(filedata, filedata.header.uname);
    return 0;
}
