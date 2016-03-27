#ifndef COMMAND_H
#define COMMAND_H

/*
 *  Header file for command
 */

#define MAX_ARGS 1024

typedef struct Command Command;

struct Command {
  char *argv[MAX_ARGS];
  int argc;
  Command *next;
};

#endif
