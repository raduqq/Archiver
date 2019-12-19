#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tema3.h"

#define RECORDSIZE 512
#define LINE_MAX 512

int to_octal(int x) {
    int rest, octal = 0, reverse = 0;

    while(x) {
        rest = x - ((x / 8) * 8);
        octal = octal * 10 + rest;
        x /= 8;
    }

    while(octal) {
        reverse = reverse * 10 + (octal % 10);
        octal /= 10;
    }
    return reverse;
}

int main() {
//purpose: parsing files, usermap
    int i, j;
    char buffer[LINE_MAX], *p, *endptr, delim[] = {" "};
    union record filedata;

    char directory_name[] = {"radutest/"};  
    char files_path[LINE_MAX], usermap_path[LINE_MAX];

    strcpy(files_path, directory_name);
    strcpy(usermap_path, directory_name);

    strcat(files_path, "files.txt");
    strcat(usermap_path, "usermap.txt");

    FILE *files = fopen(files_path, "r");
    FILE *usermap = fopen(usermap_path, "r");
    

    while(fgets(buffer, sizeof(buffer), files)) {
        buffer[strlen(buffer) - 1] = '\0'; // solved the buffer ending in newline
        memset(&filedata, 0, sizeof(filedata));
        strcpy(filedata.header.magic, "GNUtar ");
        //TODO: size, timestamp, name/linkname

// permissions/mode
        p = strtok(buffer, delim);
        for (i = 5; i <= 7; i++) {
            for (j = (i - 5) * 3 + 1; j <= (i - 4) * 3; j++) {
                if (j % 3 == 1 && p[j] == 'r') {
                    filedata.header.mode[i] += 4;
                }
                if (j % 3 == 2 && p[j] == 'w') {
                    filedata.header.mode[i] += 2;
                }
                if(j % 3 == 0 && p[j] == 'x') {
                    filedata.header.mode[i] += 1;
                }
            }
        }

// no_links
        p = strtok(NULL, delim);

// owner_name
        p = strtok(NULL, delim);
        strcpy(filedata.header.uname, p);

// owner_group
        p = strtok(NULL, delim);
        strcpy(filedata.header.gname, p);

// size
        p = strtok(NULL, delim);
        int size = strtol(p, &endptr, 10); // wtf it s char?!?
        sprintf(filedata.header.size, "%o", size);
        //size = to_octal(size);
        puts(filedata.header.size);
    }

    fclose(files);
    fclose(usermap);
    
    return 0;
}
