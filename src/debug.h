#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <errno.h>

//#define DEBUG

inline void panic(const char *c) {
  perror(c);
  exit(1);
}

#endif
