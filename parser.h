#ifndef PARSER_H
#define PARSER_H

#include "command.h"

typedef struct cmd_tkn {
  char *tkn;
  size_t op_len;
  cmd_op op;
} cmd_tkn;

char *read_line();
Command *parse_line(char *line);
void free_line(char *line);
void free_cmds(Command *cmd);

cmd_tkn get_token(char **line);
cmd_tkn is_special(char *c);

#endif
