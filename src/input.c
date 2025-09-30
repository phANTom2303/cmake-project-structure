// input.c: implementation for reading two integers from the user
#include "input.h"
#include <stdio.h>

static void clear_stdin(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        // discard
    }
}

int read_two_ints(int *a, int *b) {
    if (!a || !b)
        return -1;

    printf("Enter two integers separated by space: ");
    // uncomment the fflush() statemetn if you do not see the above prompt
    // during execution fflush(stdout);
    int matched = scanf("%d %d", a, b);
    // Always discard any remaining characters on the line
    clear_stdin();

    if (matched == 2) {
        return 0;
    }
    // If partial or invalid input, report error
    fprintf(stderr, "Invalid input. Please enter two integers.\n");
    return 1;
}
