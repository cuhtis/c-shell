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

// TODO: Make this customizable, config file
static char prompt[] = "shell: ";


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

