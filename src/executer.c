/*
 *  Executer file
 *  Written by Curtis Li
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include "command.h"
#include "executer.h"
#include "builtin.h"
#include "debug.h"

void exec_cmd(Command *cmd, int pipe_in) {
  if (!cmd) return;
  if (cmd->argc == 0) return;
  
  int ret_status = 0;
  int pid, i, fd, pipe_fd[2];

  if (!handle_builtin(cmd)) {
    // Not built in
    
    // Piping
    if (cmd->op == PIPE && pipe(pipe_fd) < 0)
      panic("pipe");
   
    switch (pid = fork()) {
    case 0: // Child
      // Pipes
      if (cmd->op == PIPE) {
        if (close(pipe_fd[0]) < 0) perror("close");
        if (close(1) < 0) perror("close");
        if (dup2(pipe_fd[1], 1) < 0) panic("dup2");
        if (close(pipe_fd[1]) < 0) perror("close");
      }
      if (pipe_in) {
        if (close(0) < 0) perror("close");
        if (dup2(pipe_in, 0) < 0) panic("dup2");
        if (close(pipe_in) < 0) perror("close");
      }

      // Redirection
      handle_redirect(cmd);
      
      execvp(cmd->argv[0], cmd->argv);
      perror(cmd->argv[0]);
      exit(0);
      break;

    default: // Parent
      // Pipes
      if (cmd->op == PIPE) {
        if (close(pipe_fd[1]) < 0) perror("close");
        pipe_in = pipe_fd[0];
      } else pipe_in = 0;

      // Wait for child if not foreground process
      if (cmd->op != BACKGROUND)
        // TODO: Error handling
        waitpid(pid, &ret_status, 0); 
      else
        // Assume success
        ret_status = 0;
      break;
    }
  }

  // Run next command
  if (cmd->next) {
    // Check for boolean operators
    if (cmd->op != AND && cmd->op != OR)
      exec_cmd(cmd->next, pipe_in);
    else if (cmd->op == AND && ret_status == 0)
      exec_cmd(cmd->next, pipe_in);
    else if (cmd->op == OR && ret_status != 0)
      exec_cmd(cmd->next, pipe_in);
  }
}

static void handle_redirect(Command *cmd) {
  int fd;
  
  // stdin redirect
  if (cmd->redirect[0]) {
    if ((fd = open(cmd->redirect[0], O_RDONLY)) < 0) panic("open");
    if (dup2(fd, 0) < 0) panic("dup2");
    if (close(fd) < 0) perror("close");
  }
  
  // stdout, stderr redirect
  if (cmd->redirect[1] &&
      cmd->redirect[2] && 
      strcmp(cmd->redirect[1], cmd->redirect[2]) == 0) {
    if ((fd = open(cmd->redirect[1], O_WRONLY|O_CREAT|O_TRUNC, 0666)) < 0) panic("open");
    if (dup2(fd, 1) < 0) panic("dup2");
    if (dup2(fd, 2) < 0) panic("dup2");
    if (close(fd) < 0) perror("close");
  } else if (cmd->redirect[1]) {
    if ((fd = open(cmd->redirect[1], O_WRONLY|O_CREAT|O_TRUNC, 0666)) < 0) panic("open");
    if (dup2(fd, 1) < 0) panic("dup2");
    if (close(fd) < 0) perror("close");
  } else if (cmd->redirect[2]) {
    if ((fd = open(cmd->redirect[2], O_WRONLY|O_CREAT|O_TRUNC, 0666)) < 0) panic("open");
    if (dup2(fd, 2) < 0) panic("dup2");
    if (close(fd) < 0) perror("close");
  }
}

static int handle_builtin(Command *cmd) {
  if (strcmp(cmd->argv[0], "cd") == 0) {
    return builtin_cd(cmd);
  } else if (strcmp(cmd->argv[0], "exit") == 0) {
    // Builtin: exit
    printf("Bye.\n");
    exit(0);
  }

  return 0;
}
