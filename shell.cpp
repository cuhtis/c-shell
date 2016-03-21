/*
 *  Main file
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include "shell.h"
#include "command.h"

#define BLOCK_SIZE 16

char prompt[] = "shell: ";

int main() {
  printf("Shell v0.1\n");

  while(1) {
    printf("%s", prompt);
    readline();
  }

  return 0;
}

void readline() {
  ssize_t size = 0, bytes;
  char *line = (char *) malloc(BLOCK_SIZE*sizeof(char));

  while (1) {
    bytes = read(0, line+size, BLOCK_SIZE);
    size += bytes;
    if (bytes != BLOCK_SIZE) break;
    line = (char *) realloc(line, sizeof(line)+BLOCK_SIZE*sizeof(char));
  }

  printf("%s", line);

  return;
}
