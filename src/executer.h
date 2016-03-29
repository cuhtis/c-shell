#ifndef EXEC_H
#define EXEC_H

/*
 *  Header file for shell.c
 */

#include "command.h"

void exec_cmd(Command *cmd, int pipe_in);

static int handle_builtin(Command *cmd);
static void handle_redirect(Command *cmd);

#endif
