/*
 *  Main file
 *  Written by Curtis Li
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <string.h>

#include "shell.h"

#define BLOCK_SIZE 16
#define MAX_CMDS 128
#define MAX_ARGS 1024

// TODO: Make this customizable, config file
static char prompt[] = "shell: ";

struct Command {
  char *argv[MAX_ARGS];
  int argc;
  Command *next;
};


/**************************************************
 *                                                *
 *    Functions                                   *
 *                                                *
 **************************************************/

/*
 *  Main function
 */
int main() {
  printf("Shell v0.1\n");
  setbuf(stdout, NULL);
  char *line;
  Command *cmd;

  // Main program loop
  while(1) {
    // Read line
    printf("%s", prompt);
    line = read_line();
    cmd = parse_line(line);
    while (cmd) {
      int i;
      for (i = 0; i < cmd->argc; i++) {
        printf("argv[%d]: %s (%lu)\n", i, cmd->argv[i], strlen(cmd->argv[i]));
      }
      cmd = cmd->next;
    }
    free_cmds(cmd);
    free_line(line);
  }
}

static char *read_line() {
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

static Command *parse_line(char *line) {
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
    printf("LINE: %s\n", line);
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

static inline void free_line(char *line) {
  free(line);
}

static inline void free_cmds(Command *cmd) {
  if (!cmd) return;
  if (cmd->next) free_cmds(cmd->next);
  free(cmd);
}
