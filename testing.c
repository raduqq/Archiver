// INCLUDE FAIL WHEN OPENING FILES

#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "tema3.h"

int main() {
//purpose: parsing files, usermap
    int i, j;
    char buffer[LINE_MAX], *p, *endptr, space[] = {" "}, colon[] = {":"};
    union record filedata;

    char directory_name[] = {"radutest/"};  
    char files_path[LINE_MAX], usermap_path[LINE_MAX];

    strcpy(files_path, directory_name);
    strcpy(usermap_path, directory_name);

    strcat(files_path, "files.txt");
    strcat(usermap_path, "usermap.txt");

    FILE *files, *usermap;    

    files = fopen(files_path, "r");
    while(fgets(buffer, sizeof(buffer), files)) {
        buffer[strlen(buffer) - 1] = '\0'; // solved the buffer ending in newline
        memset(&filedata, 0, sizeof(filedata));
        strcpy(filedata.header.magic, "GNUtar ");

// permissions/mode
        p = strtok(buffer, space);
        for (i = 0; i <= 2; i++) {
            for (j = i * 3 + 1; j <= (i + 1) * 3; j++) {
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
        p = strtok(NULL, space);

// owner_name
        p = strtok(NULL, space);
        strcpy(filedata.header.uname, p);

// owner_group
        p = strtok(NULL, space);
        strcpy(filedata.header.gname, p);

// size
        p = strtok(NULL, space);
        int size = strtol(p, &endptr, 10);
        sprintf(filedata.header.size, "%o", size);
        //puts(filedata.header.size);
        // possible problem here: not sure @proper 0000172 archiving

// last_change_time
        p = strtok(NULL, space);
        struct tm last_modified_time = {0};
        strptime(p, "%Y-%m-%d", &last_modified_time);
        //last_modified_time.tm_year += 1900; last_modified_time.tm_mon += 1;


        p = strtok(NULL, space);
        strptime(p, "%H:%M:%S", &last_modified_time);

        time_t rawtime = mktime(&last_modified_time);

        sprintf(filedata.header.mtime, "%ld", rawtime);

// name
        p = strtok(NULL, space); p = strtok(NULL, space);
        strcpy(filedata.header.name, p);
        strcpy(filedata.header.linkname, p);

// parsing usermap
        usermap = fopen(usermap_path, "r");
        char ok = 0;
        while(fgets(buffer, sizeof(buffer), usermap) && ok == 0) {
            p = strtok(buffer, ":");
            if(strcmp(p, filedata.header.uname) == 0) {
                ok = 1;
                p = strtok(NULL, colon); p = strtok(NULL, colon);
                strcpy(filedata.header.uid, p);

                p = strtok(NULL, colon);
                strcpy(filedata.header.gid, p);
            }
        }
        fclose(usermap);

// chksum
        int sum = 0;
        for (i = 0; i < sizeof(filedata.header.name); i++) {
            sum += (int)filedata.header.name[i];
            sum += (int)filedata.header.linkname[i];
        }
        for (i = 0; i < sizeof(filedata.header.uname); i++) {
            sum += (int)filedata.header.uname[i];
            sum += (int)filedata.header.gname[i];
        }
        for (i = 0; i < sizeof(filedata.header.size); i++) {
            sum += (int)filedata.header.size[i];
            sum += (int)filedata.header.mtime[i];
        }
        for (i = 0; i < sizeof(filedata.header.mode); i++) {
            sum += (int)filedata.header.mode[i];
            sum += (int)filedata.header.uid[i];
            sum += (int)filedata.header.gid[i];
            sum += (int)filedata.header.magic[i];
            sum += (int)filedata.header.devmajor[i];
            sum += (int)filedata.header.devminor[i];
        }
        sprintf(filedata.header.chksum, "%o", sum);

// all we gotta do: write the actual file.
    }
    fclose(files);
    return 0;
}
