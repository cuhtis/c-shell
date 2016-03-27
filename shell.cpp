/*
 *  Main file
 *  Written by Curtis Li
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>

#include "shell.h"
#include "includes/command.h"
#include "includes/string.h"

#define BLOCK_SIZE 16
#define MAX_CMD_NUM 128

using namespace std;

// TODO: Make this customizable, config file
static char prompt[] = "shell: ";


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
  string line;
  string cmd[MAX_CMD_NUM];

  // Main program loop
  while(1) {
    // Read line
    cout << prompt;
    getline(cin, line);
   
    // TODO: TEMP: Echo the response
    cout << "You said: " << line << endl;

    // Split into commands
    vector<string> commands = split(line, ';');
    for(vector<string>::iterator it = commands.begin(); it != commands.end(); ++it) {
      cout << *it << endl;
    }
  }
}
