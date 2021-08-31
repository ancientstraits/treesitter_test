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

#define cprintf(color, format, ...) printf("\033[0;%dm" format "\033[0m", color, __VA_ARGS__)

enum Types {
    TYPE_INCLUDE = 28,
    
};

unsigned long type_map[] = {
};

#endif // !UTIL_H