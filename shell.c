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

#include "command.h"
#include "shell.h"
#include "parser.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

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
  printf("--------------------\n");
  setbuf(stdout, NULL);
  char *line;
  Command *cmd;

  // TODO: Error checking
  char user[1024];
  getlogin_r(user, 1024);
  char cwd[1024];
  getcwd(cwd, 1024);

  // Main program loop
  while(1) {
    // Read line
    printf(ANSI_COLOR_BLUE "%s:" ANSI_COLOR_CYAN "%s" ANSI_COLOR_RESET "$ ", user, cwd);
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

