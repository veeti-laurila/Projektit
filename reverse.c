#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

// function for printing the file in reverse order
void reverse_lines(FILE *input, FILE *output) {
    char **lines = malloc(MAX_LINE_LENGTH * sizeof(char*));
    if (lines == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }

    size_t count = 0;
    char buffer[MAX_LINE_LENGTH];

    while (fgets(buffer, MAX_LINE_LENGTH, input)) {
        lines[count] = malloc(strlen(buffer) + 1);
        if (lines[count] == NULL) {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }
        strcpy(lines[count], buffer);
        count++;
    }

    for (size_t i = count; i > 0; i--) {
        fprintf(output, "%s", lines[i - 1]);
        free(lines[i - 1]);
    }

    free(lines);
}

int main(int argc, char *argv[]) {
    FILE *input = stdin;
    FILE *output = stdout;

    // If-else which checks how many arguments are given and prints the possible errors.
    if (argc == 1) {

    } else if (argc == 2) {
        input = fopen(argv[1], "r");
        if (input == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
    } else if (argc == 3) {
        if (strcmp(argv[1], argv[2]) == 0) {
            fprintf(stderr, "Input and output file must differ\n");
            exit(1);
        }
        
        input = fopen(argv[1], "r");
        if (input == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[1]);
            exit(1);
        }

        output = fopen(argv[2], "w");
        if (output == NULL) {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[2]);
            exit(1);
        }
    } else {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }

    reverse_lines(input, output);

    if (input != stdin) fclose(input);
    if (output != stdout) fclose(output);

    return 0;
}