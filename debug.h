#ifndef DEBUG_H
#define DEBUG_H

//#define DEBUG

inline void panic(const char *c) {
  perror(c);
  exit(1);
}

#endif
