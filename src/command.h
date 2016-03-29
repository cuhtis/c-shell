#ifndef COMMAND_H
#define COMMAND_H

/*
 *  Header file for command
 */

#define MAX_ARGS 1024

typedef struct Command Command;

typedef enum {
  NORMAL,
  PIPE,
  AND,
  OR,
  REDIRECT_IN,
  REDIRECT_OUT,
  REDIRECT_ERR,
  REDIRECT_ALLOUT,
  OPEN_PAREN,
  CLOSE_PAREN,
  SEMI_COLON,
  BACKGROUND,
  END,
  ERROR
} cmd_op;

struct Command {
  char *argv[MAX_ARGS];
  int argc;
  char *redirect[3];
  cmd_op op;
  Command *next;
  Command *sub;
};

#endif
