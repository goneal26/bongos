#include "tty.h"
#include "framebuffer.h"
#include <stdarg.h> // for variable args, TODO might need to implement our own?

// a simple text mode for debugging- this is likely to be removed/rewritten
// in the future in favor of a proper shell/terminal emulator and stdio support

typedef struct {
  uint32_t width;
  uint32_t height;
  uint32_t cornerx;
  uint32_t cornery;
  uint32_t cursorx;
  uint32_t cursory;
} Terminal;

Terminal terminal;

// start the tty terminal, drawing it at x, y (being the top-left coords)
void init_tty(uint32_t cx, uint32_t cy, uint32_t w, uint32_t h) {
  terminal.width = w;
  terminal.height = h;
  terminal.cornerx = cx;
  terminal.cornery = cy;
  terminal.cursorx = cx;
  terminal.cursory = cy;

  // fill the terminal with background color
  fb_rect(cx, cy, w, h, GB_BLACK);
}

// advances the cursor
static void advance() {
  if ((terminal.cursorx + 8) / 8 > terminal.width / 8) { // edge of terminal row
    terminal.cursory += 8; // TODO reaching max terminal height?
    terminal.cursorx = terminal.cornerx; // return, new row
  } else {
    terminal.cursorx += 8;
  }

}

static void nextline() {
  terminal.cursory += 8; // TODO reaching max terminal height?
  terminal.cursorx = terminal.cornerx;
  // drawchar('|', terminal.cursorx, terminal.cursory, GRAY, BLACK);
}

// print char at cursor position, then advance cursor
static void printchar(char c) {
  if (c == 0) {
    return; // don't advance cursor for nullchar
  }
  
  if (c == '\n') { // skip to next line on newline char
    nextline();
    return;
  }

  fb_char(c, terminal.cursorx, terminal.cursory, GB_WHITE, GB_BLACK);
  
  advance();
}

// print an unformatted string (array of char), basically just `puts()`
static void printstr(const char* str) {
  while (*str) {
    printchar(*str);
    str++;
  }
}

// print a single integer as a hexadecimal string
// basically just `itoa()`
static void printint(int64_t num, int base) { // should be able to use a plain `int`
  if (num == 0) {
    printchar('0');
    return;
  }

  if (base == 10 && num < 0) {
    printchar('-');
  }

  char str[100]; // TODO dynamically size?
  int i = 0;
  while (num != 0) {
    int rem = num % base;
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

static void printaddr(void* ptr) { // prints a pointer as a hex value
  uint64_t num = (uint64_t) ptr;

  if (!ptr) { // is null
    printstr("NULL");
    return;
  }
  
  char str[100]; // TODO dynamically size?
  int i = 0;
  while (num != 0) {
    int rem = num % 16;
    str[i++] = (rem > 9) ? (rem - 10) + 'A' : rem + '0'; 
    // ^^^ capital letters for pointers
    num = num / 16;
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

// uses a variadic list pointer to do some formatted printing!
static void vprint(char *fmt, va_list ap) {
  unsigned int i;
  char *s;
  void *p;
  
  va_list args; // init arguments
  va_copy(args, ap);
  
  while (*fmt) {
    if (*fmt == '%') { // is format specifier
      switch (*(fmt + 1)) { // switch for next char
        case 'c': 
          i = va_arg(args, int); // fetch char arg
          printchar(i);
          break;
        case 'd':
          i = va_arg(args, int); // int arg as decimal
          printint(i, 10);
          break;
        case 'x':
          i = va_arg(args, int); // int arg as hex
          printint(i, 16);
          break;
        case 's':
          s = va_arg(args, char*); // get string
          printstr(s);
          break;
        case 'p':
          p = va_arg(args, void*); // get ptr
          printaddr(p);
          break;
        case '%':
          printchar('%');
          break;
        default: // unsupported fmt specifier- just print the character for now
          printstr("FMTERR"); return;
      }
      fmt += 2;
    } else {
      printchar(*fmt);
      fmt++;
    }
  }
  va_end(args);
}

// formatted printing! At least, it's a start.
void print(char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprint(fmt, args);
  va_end(args);
}

void println(char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprint(fmt, args);
  printchar('\n');
  va_end(args);
}
