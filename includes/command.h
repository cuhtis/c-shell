#ifndef COMMAND_H
#define COMMAND_H

class Command
{
  
  private:
    char *cmd;
    char **args;
    Command *next;

  public:
    Command(char *cmdline);

};

#endif
