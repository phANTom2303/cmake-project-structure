// main.c: reads two integers using input module, computes sum, and displays it
#include <stdio.h>
#include "input.h"

int main(void) {
    int a = 0, b = 0;
    if (read_two_ints(&a, &b) != 0) {
        return 1; // error already reported by input module
    }
    int sum = a + b;
    printf("Sum: %d\n", sum);
    return 0;
}