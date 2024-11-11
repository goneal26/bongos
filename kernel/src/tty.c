#include "tty.h"
#include "graphics.h"

// a simple text mode for debugging- this is likely to be removed/rewritten
// in the future in favor of a proper shell/terminal emulator and stdio support

typedef struct {
  uint64_t width;
  uint64_t height;
  uint64_t cornerx;
  uint64_t cornery;
  uint64_t cursorx;
  uint64_t cursory;
} Terminal;

Terminal terminal;

// start the tty terminal, drawing it at x, y (being the top-left coords)
void init_tty(uint64_t cx, uint64_t cy, uint64_t w, uint64_t h) {
  terminal.width = w;
  terminal.height = h;
  terminal.cornerx = cx;
  terminal.cornery = cy;
  terminal.cursorx = cx;
  terminal.cursory = cy;

  // fill the terminal with background color
  fillrect(cx, cy, w, h, BLACK);

  // draw the cursor for debugging
  // drawchar('|', cx, cy, GRAY, BLACK);
}

// advances the cursor
static void advance() {
  if ((terminal.cursorx + 8) / 8 > terminal.width / 8) { // edge of terminal row
    terminal.cursory += 8; // TODO reaching max terminal height?
    terminal.cursorx = terminal.cornerx; // return, new row
  } else {
    terminal.cursorx += 8;
  }

  // drawchar('|', terminal.cursorx, terminal.cursory, GRAY, BLACK);
}

static void nextline() {
  terminal.cursory += 8; // TODO reaching max terminal height?
  terminal.cursorx = terminal.cornerx;
  // drawchar('|', terminal.cursorx, terminal.cursory, GRAY, BLACK);
}

// print char at cursor position, then advance cursor
void printchar(char c) {
  if (c == 0) {
    return; // don't advance cursor for nullchar
  }
  
  if (c == '\n') { // skip to next line on newline char
    nextline();
    return;
  }

  drawchar(c, terminal.cursorx, terminal.cursory, WHITE, BLACK);
  
  advance();
}

// print an unformatted string (array of char), basically just `puts()`
void printstr(const char* str) {
  while (*str) {
    printchar(*str);
    str++;
  }
}

// print a single integer as a hexadecimal string
// basically just `itoa()`
void printint(int num, int base) { // should be able to use a plain `int`
  if (num == 0) {
    printchar('0');
    return;
  }

  if (base == 10 && num < 0) {
    printchar('-');
  }

  if (base == 16) {
    printchar('0');
    printchar('x'); // TODO add more base prefixes?
  }

  char str[100]; // TODO dynamically size?
  int i = 0;
  while (num != 0) {
    int rem = num % base; // for other bases do remainder (base)
    str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
    num = num / base;
  }

  // put null char at the end
  str[i] = '\0';

  // now `str` holds the hex digits, but reversed, so we reverse:
  int start = 0;
  int end = i - 1;
  while (start < end) {
    char temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    end--;
    start++;
  }

  // and then print `str`
  printstr(str);
}

// TODO format strings?
