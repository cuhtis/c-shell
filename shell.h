/*
 *  Header file for shell.c
 */

typedef struct Command Command;

static char *read_line();
static Command *parse_line(char *line);
static inline void free_line(char *line);
static inline void free_cmds(Command *cmd);
