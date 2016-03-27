#ifndef PARSER_H
#define PARSER_H

#include "command.h"

char *read_line();
Command *parse_line(char *line);
void free_line(char *line);
void free_cmds(Command *cmd);

#endif
