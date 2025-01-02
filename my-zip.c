#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function for compressing the file
void compress(FILE *file) {
    int count = 0;
    char current = 0;
    char previous = 0;
    char buffer[1024];

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        for (int i = 0; buffer[i] != '\0'; i++) {
            current = buffer[i];
            if (current == previous) {
                count++;
            } else {
                if (count > 0) {
                    fwrite(&count, sizeof(int), 1, stdout);
                    fwrite(&previous, sizeof(char), 1, stdout);
                }
                previous = current;
                count = 1;
            }
        }
    }
    if (count > 0) {
        fwrite(&count, sizeof(int), 1, stdout);
        fwrite(&previous, sizeof(char), 1, stdout);
    }
}

int main(int argc, char *argv[]) {
    // If not enough arguments
    if (argc < 2) {
        printf("my-zip: file1 [file2 ...]\n");
        return 1;
    }
    for (int i = 1; i < argc; i++) {
        FILE *file = fopen(argv[i], "r");
        if (file == NULL) {
            printf("my-zip: cannot open file\n");
            return 1;
        }
        compress(file);
        fclose(file);
    }
    return 0;
}