#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <string.h>

#include "command.h"
#include "parser.h"

#define BLOCK_SIZE 16
#define MAX_CMDS 128

char *read_line() {
  ssize_t size = 0, bytes;
  char *line = (char *) malloc(BLOCK_SIZE*sizeof(char));
    
  while (1) {
    bytes = read(0, line+size, BLOCK_SIZE);
    size += bytes;
    if (bytes != BLOCK_SIZE) break;
    line = (char *) realloc(line, sizeof(line)+BLOCK_SIZE*sizeof(char));
  }
      
  return line;
}

Command *parse_line(char *line) {
  char *next_cmd, *next_tkn;
  int argc;
  Command *cmd, *first_cmd, *prev_cmd = NULL;

  // Remove the trailing newline character
  if (line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = '\0';

  while(line) {
    // Initialize a new command
    cmd = (Command *)malloc(sizeof(Command));
    memset(cmd, 0, sizeof(Command));
    if (prev_cmd) prev_cmd->next = cmd;
    else first_cmd = cmd;

    // Check for more commands
    if ((next_cmd = strchr(line, ';'))) {
      *next_cmd = '\0';
      do next_cmd++;
      while(*next_cmd == ' ');
    }

    // Tokenize arguments
    argc = 0;
    while (*line != '\0') {
      // Trim leading whitespace
      while (*line == ' ') line++;
      if (*line == '\0') break;

      // Set the arg
      cmd->argv[argc++] = line;

      // Find end of arg
      while (*line != ' ' && *line != '\0') line++;
      if (*line == '\0') break;
      *line = '\0';
      line++;

      // Trim trailing whitespace
      while (*line == ' ') line++;
    }
    cmd->argc = argc;

    // Go to next (or null)
    line = next_cmd;
    prev_cmd = cmd;
  }

  return first_cmd;
}

void free_line(char *line) {
  free(line);
}

void free_cmds(Command *cmd) {
  if (!cmd) return;
  if (cmd->next) free_cmds(cmd->next);
  free(cmd);
}
