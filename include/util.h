#ifndef UTIL_H
#define UTIL_H

char* readFile(const char* filename);

// colors
enum Colors {
    COLOR_BLACK = 30,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_BLUE,
    COLOR_PURPLE,
    COLOR_CYAN,
    COLOR_WHITE,
};

#define cprintf(color, format, ...) \
    printf("\033[0;%dm" format "\033[0m", color, __VA_ARGS__)

enum Types {
    TYPE_NUMBER = 0,
    TYPE_STRING,
    TYPE_TYPE,
};

unsigned long type_map[] = {
    [TYPE_NUMBER]   = COLOR_BLUE,
    [TYPE_STRING]   = COLOR_GREEN,
    [TYPE_TYPE]     = COLOR_YELLOW,
};

#endif // !UTIL_H