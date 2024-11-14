#ifndef TTY_H
#define TTY_H

#include <stdint.h>

void init_tty(uint32_t cx, uint32_t cy, uint32_t w, uint32_t h);
void print(char* fmt, ...);
void println(char* fmt, ...);

#endif
