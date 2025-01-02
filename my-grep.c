#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to check if line contains the search term
int check_for_term(const char *line, const char *search_term) {
    return strstr(line, search_term) != NULL;
}

void search_file(const char *search_term, FILE *file) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    // Read every line and check for the search term
    while ((read = getline(&line, &len, file)) != -1) {
        if (check_for_term(line, search_term)) {
            printf("%s", line);
        }
    }

    // Free buffer
    free(line);
}

int main(int argc, char *argv[]) {
    // If no arguments
    if (argc < 2) {
        printf("my-grep: searchterm [file ...]\n");
        return 1;
    }

    const char *search_term = argv[1];

    // If no files
    if (argc == 2) {
        search_file(search_term, stdin);
    } else {
        // For loop for all files
        for (int i = 2; i < argc; i++) {
            FILE *file = fopen(argv[i], "r");
            if (file == NULL) {
                printf("my-grep: cannot open file\n");
                return 1;
            }
            // Search and close the file
            search_file(search_term, file);
            fclose(file);
        }
    }
    return 0;
}