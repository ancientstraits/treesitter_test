#include <stdio.h>
#include <stdlib.h>

#define cprintf(color, format, ...) \
    printf("\033[%d%sm" format "\033[0m", color % 100, \
    color / 100 == 1 ? ";1" : "", __VA_ARGS__);

enum Colors {
    COLOR_BLACK = 30,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_BLUE,
    COLOR_PURPLE,
    COLOR_CYAN,
    COLOR_WHITE,

    BCOLOR_BLACK = 130,
    BCOLOR_RED,
    BCOLOR_GREEN,
    BCOLOR_YELLOW,
    BCOLOR_BLUE,
    BCOLOR_PURPLE,
    BCOLOR_CYAN,
    BCOLOR_WHITE,
};

int main() {
    for (int i = 30; i < 38; i++) {
        cprintf(i, "Color %d\n", i);
    }
    for (int i = 130; i < 138; i++) {
        cprintf(i, "Color %d\n", i);
    }
}