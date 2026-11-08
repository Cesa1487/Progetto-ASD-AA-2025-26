#include <stdio.h>
#include <string.h>
#include "../include/edit_distance.h"

typedef struct {
    const char *s1;
    const char *s2;
    int expected;
} TestCase;

void run_test(const char *name, int actual, int expected) {
    if (actual == expected)
        printf("[OK]   %s\n", name);
    else
        printf("[FAIL] %s: expected %d, got %d\n", name, expected, actual);
}

int main(void) {
    TestCase tests[] = {
        {"",     "",       0},
        {"casa", "",       4},
        {"",     "casa",   4},
        {"casa", "cassa",  1},
        {"casa", "cara",   2},
        {"vinaio", "vino", 2},
        {"tassa", "passato", 4},
        {"pioppo", "pioppo", 0}
    };
    int n = sizeof(tests) / sizeof(TestCase);

    printf("Testing edit_distance (recursive):\n");
    for (int i = 0; i < n; ++i) {
        char name[128];
        snprintf(name, sizeof(name), "edit_distance(\"%s\", \"%s\")", tests[i].s1, tests[i].s2);
        int result = edit_distance(tests[i].s1, tests[i].s2);
        run_test(name, result, tests[i].expected);
    }

    printf("\nTesting edit_distance_dyn (dynamic):\n");
    for (int i = 0; i < n; ++i) {
        char name[128];
        snprintf(name, sizeof(name), "edit_distance_dyn(\"%s\", \"%s\")", tests[i].s1, tests[i].s2);
        int result = edit_distance_dyn(tests[i].s1, tests[i].s2);
        run_test(name, result, tests[i].expected);
    }

    return 0;
}