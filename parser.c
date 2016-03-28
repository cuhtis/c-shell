#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include "command.h"
#include "parser.h"

#define BLOCK_SIZE 1024
#define MAX_CMDS 128

char *read_line() {
  ssize_t size = 0, bytes;
  int blocks = 1;
  char *line = (char *) malloc(blocks * BLOCK_SIZE);
   
  // FIXME: When input size is a multiple of BLOCK_SIZE, will try to read more
  // Read, allocate more space if needed
  while (1) {
    bytes = read(0, line+size, BLOCK_SIZE);
    size += bytes;
    if (bytes != BLOCK_SIZE) break;
    line = (char *) realloc(line, ++blocks *  BLOCK_SIZE);
  }

  // Null terminate (Note: We may still have the newline char)
  *(line + size) = '\0';
      
  return line;
}

Command *parse_line(char *line) {
  cmd_tkn next_tkn;
  Command *cmd, *first_cmd, *prev_cmd = NULL;

  if (!line) return NULL;

  // Remove the trailing newline character
  if (line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = '\0';

  while (1) {
    // Initialize a new command
    cmd = (Command *)malloc(sizeof(Command));
    memset(cmd, 0, sizeof(Command));
    if (prev_cmd) prev_cmd->next = cmd;
    else first_cmd = cmd;


    // Get tokens
    while ((next_tkn = get_token(&line)).op == NORMAL ||
        next_tkn.op == REDIRECT_IN ||
        next_tkn.op == REDIRECT_OUT ||
        next_tkn.op == REDIRECT_ERR ||
        next_tkn.op == REDIRECT_ALLOUT) {
      
      if (next_tkn.op == NORMAL) {
        cmd->argv[cmd->argc++] = next_tkn.tkn; 
      }
      else if (next_tkn.op == REDIRECT_ALLOUT) {
        next_tkn = get_token(&line);
        cmd->redirect[1] = next_tkn.tkn;
        cmd->redirect[2] = next_tkn.tkn;
      }
      else if (next_tkn.op == REDIRECT_IN) {
        next_tkn = get_token(&line);
        cmd->redirect[0] = next_tkn.tkn;
      }
      else if (next_tkn.op == REDIRECT_OUT) {
        next_tkn = get_token(&line);
        cmd->redirect[1] = next_tkn.tkn;
      }
      else if (next_tkn.op == REDIRECT_ERR) {
        next_tkn = get_token(&line);
        cmd->redirect[2] = next_tkn.tkn;
      }
    }
    prev_cmd = cmd;
    cmd->op = next_tkn.op;
    
    assert(next_tkn.op != ERROR);
    
    if (next_tkn.op == END) {
      break;
    }
  }

  return first_cmd;
}

cmd_tkn get_token(char **line) {
  char *start;
  cmd_tkn tkn, new_tkn;
  int quotes = 0;

  // Trim leading whitespace
  while (**line == ' ') (*line)++;
  if (**line == '\0') {
    new_tkn.op = END;
    return new_tkn;
  }

  start = *line;
  
  if ((new_tkn = is_special(*line)).op != NORMAL) {
    // Special token
    *line += new_tkn.op_len;
    return new_tkn;
  }

  assert(new_tkn.op == NORMAL);

  while (**line != '\0' && **line != ' ') {
    // Stop at a special token
    if ((tkn = is_special(*line)).op != NORMAL) break;
    
    // Handle quotes
    if (**line == '\'' || **line == '\"' || **line == '`') {
      quotes = 1;
      if (!(*line = strchr(*line, **line))) {
        new_tkn.op = ERROR;
        return new_tkn;
      }
    }

    (*line)++;
  } 

  int len = *line - start;
  new_tkn.tkn = (char *) malloc(len + 1);
  strncpy(new_tkn.tkn, start, len);
  *(new_tkn.tkn + len) = '\0';

  return new_tkn;
}

void free_line(char *line) {
  free(line);
}

void free_cmds(Command *cmd) {
  if (!cmd) return;
  int i;

  // Free args
  for (i = 0; i < cmd->argc; i++)
    free(cmd->argv[i]);

  // Free redirect filenames
  for (i = 0; i < 3; i++) {
    if (cmd->redirect[i]) {
      if (i == 1 && cmd->redirect[2] && strcmp(cmd->redirect[1], cmd->redirect[2]) == 0) continue;
      free(cmd->redirect[i]);
    }
  }

  // Free next cmd
  if (cmd->next) free_cmds(cmd->next);

  // Free cmd
  free(cmd);
}

cmd_tkn is_special(char *c) {
  cmd_tkn tkn;
  tkn.tkn = c;

  if (strncmp(c, "&&", 2) == 0) {
    tkn.op_len = 2;
    tkn.op = AND;
  } else if (strncmp(c, "||", 2) == 0) {
    tkn.op_len = 2;
    tkn.op = OR;
  } else if (strncmp(c, "&>", 2) == 0) {
    tkn.op_len = 2;
    tkn.op = REDIRECT_ALLOUT;
  } else if (strncmp(c, "2>", 2) == 0) {
    tkn.op_len = 2;
    tkn.op = REDIRECT_ERR;
  } else if (strncmp(c, ">", 1) == 0) {
    tkn.op_len = 1;
    tkn.op = REDIRECT_OUT;
  } else if (strncmp(c, "<", 1) == 0) {
    tkn.op_len = 1;
    tkn.op = REDIRECT_IN;
  } else if (strncmp(c, "&", 1) == 0) {
    tkn.op_len = 1;
    tkn.op = BACKGROUND;
  } else if (strncmp(c, ";", 1) == 0) {
    tkn.op_len = 1;
    tkn.op = SEMI_COLON;
  } else if (strncmp(c, "(", 1) == 0) {
    tkn.op_len = 1;
    tkn.op = OPEN_PAREN;
  } else if (strncmp(c, ")", 1) == 0) {
    tkn.op_len = 1;
    tkn.op = CLOSE_PAREN;
  } else {
    tkn.op_len = 0;
    tkn.op = NORMAL;
  }
  
  return tkn;
}
