#ifndef TTY_H
#define TTY_H

#include <stdint.h>

void init_tty(uint64_t cx, uint64_t cy, uint64_t w, uint64_t h);
void printchar(char c);
void printstr(const char* str);
void printint(int num, int base);

#endif
