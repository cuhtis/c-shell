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
#include <pwd.h>
#include <fcntl.h>

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
  printf("Created by Curtis Li\n");
  printf("--------------------\n");
  setbuf(stdout, NULL);
  char user[1024], pn[1024], *line, *homedir, *p, *cwd = pn;
  Command *cmd;

  // TODO: Error checking
  getlogin_r(user, 1024);

  // Main program loop
  while(1) {
    // Prompt
    if ((homedir = getenv("HOME")) == NULL) {
          homedir = getpwuid(getuid())->pw_dir;
    }
    getcwd(pn, 1024);
    if (strncmp(pn, homedir, strlen(homedir)) == 0) {
      cwd = pn + strlen(homedir) - 1;
      *cwd = '~';
    }
    printf(ANSI_COLOR_BLUE "%s:" ANSI_COLOR_CYAN "%s" ANSI_COLOR_RESET "$ ", user, cwd);

    // Read line
    line = read_line();
    cmd = parse_line(line);

    /*
    Command *cur = cmd;
    while (cur) {
      int i;
      printf("argc = %d\n", cur->argc);
      for (i = 0; i < cur->argc; i++) {
        printf("argv[%d] = %s\n", i, cur->argv[i]);
      }
      for (i = 0; i < 3; i++) {
        printf("redirect[%d] = %s\n", i, cur->redirect[i]);
      }
      cur = cur->next;
    }
    */

    // Execute
    exec_cmd(cmd);

    // Free space
    free_cmds(cmd);
    free_line(line);
  }
}

void exec_cmd(Command *cmd) {
  if (cmd->argc == 0) return;
  
  int ret_status = 0;

  // Built ins
  if (strcmp(cmd->argv[0], "cd") == 0) {
    // TODO: Error checking
    if (cmd->argc < 2) {
      char *homedir;
      if ((homedir = getenv("HOME")) == NULL) {
        homedir = getpwuid(getuid())->pw_dir;
      }
      chdir(homedir);
    } else chdir(cmd->argv[1]);
  } else if (strcmp(cmd->argv[0], "exit") == 0) {
    // TODO: Error checking
    printf("Bye.\n");
    exit(0);
  } else {
    // Not built in
    int pid = fork();
  
    if (pid == 0) {
      // Child
      // Manipulate stdin, stdout, stderr
      int i, fd;
      // stdin redirect
      if (cmd->redirect[0]) {
        fd = open(cmd->redirect[0], O_RDONLY);
        dup2(fd, 0);
        close(fd);
      }
      // stdout, stderr redirect
      if (cmd->redirect[1] &&
          cmd->redirect[2] && 
          strcmp(cmd->redirect[1], cmd->redirect[2]) == 0) {
        fd = open(cmd->redirect[1], O_WRONLY|O_CREAT|O_TRUNC, 0666);
        dup2(fd, 1);
        dup2(fd, 2);
        close(fd);
      } else if (cmd->redirect[1]) {
        fd = open(cmd->redirect[1], O_WRONLY|O_CREAT|O_TRUNC, 0666);
        printf("stdout to %s at fd %d\n", cmd->redirect[1], fd);
        dup2(fd, 1);
        close(fd);
      } else if (cmd->redirect[2]) {
        fd = open(cmd->redirect[2], O_WRONLY|O_CREAT|O_TRUNC, 0666);
        dup2(fd, 2);
        close(fd);
      }
      
      // TODO: Error checking
      execvp(cmd->argv[0], cmd->argv);
      printf("Could not locate %s\n", cmd->argv[0]);
      exit(0);
    } else {
      // Parent
      // Wait for child if not foreground process
      if (cmd->op != BACKGROUND)
        waitpid(pid, &ret_status, 0); 
      else
        // Assume success
        ret_status = 0;
    }
  }

  // Run next command
  if (cmd->next) {
    // Check for boolean operators
    if (cmd->op != AND && cmd->op != OR)
      exec_cmd(cmd->next);
    else if (cmd->op == AND && ret_status == 0)
      exec_cmd(cmd->next);
    else if (cmd->op == OR && ret_status != 0)
      exec_cmd(cmd->next);
  }
}
