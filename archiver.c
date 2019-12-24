// Copyright 2019 Radu-Stefan Minea 314CA

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./tema3.h"

int main() {
  char input[LINE_MAX], delim[] = {" "}, *p;
  char ok, cnt, command[LINE_MAX], archive_name[LINE_MAX], file_name[LINE_MAX],
      directory_name[LINE_MAX], command_arguments, command_type;

  // task 0: interpretarea input-ului

  while (1) {
    fgets(input, sizeof(input), stdin);
    input[strlen(input) - 1] = '\0';

    p = strtok(input, delim);
    cnt = 1;
    strcpy(command, p);

    ok = 1;
    while (p != NULL && ok == 1) {
      if (cnt > 3) {
        ok = 0;
      }
      if (cnt == 1) {
        if (strcmp(command, "create") == 0) {
          command_type = 1;
          command_arguments = 3;
        } else if (strcmp(command, "list") == 0) {
          command_type = 2;
          command_arguments = 2;
        } else if (strcmp(command, "extract") == 0) {
          command_type = 3;
          command_arguments = 3;
        } else if (strcmp(command, "exit") == 0) {
          command_type = 4;
          command_arguments = 1;
        } else {
          ok = 0;
        }
      }
      if (cnt == 2) {
        if (command_type == 1 || command_type == 2) {
          strcpy(archive_name, p);
        }
        if (command_type == 3) {
          strcpy(file_name, p);
        }
      }
      if (cnt == 3) {
        if (command_type == 1) {
          strcpy(directory_name, p);
        }
        if (command_type == 3) {
          strcpy(archive_name, p);
        }
      }

      p = strtok(NULL, delim);
      cnt++;
    }
    cnt--;
    if (cnt != command_arguments) {
      ok = 0;
    }

    if (ok == 0) {
      printf("> Wrong command!\n");
    } else {
      switch (command_type) {
      case 1:
        // task 1: create
        create(archive_name, directory_name);
        break;
      case 2:
        // task 2: list
        list(archive_name);
        break;
      case 3:
        // task 3: extract
        extract(file_name, archive_name);
        break;
      case 4:
        exit(0);
      }
    }
  }
  return 0;
}
