#include <limits.h>
#include <time.h>

struct sessions {
    struct tm dt;
    char cinema[CHAR_MAX];
    char film[CHAR_MAX];
    int hall;
    char type[CHAR_MAX];
};

int menu();
