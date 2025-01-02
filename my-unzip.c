#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function for decompressing the file
void decompress(FILE *file) {
    int count;
    char character;

    while (fread(&count, sizeof(int), 1, file) == 1 && fread(&character, sizeof(char), 1, file) == 1) {
        for (int i = 0; i < count; i++) {
            printf("%c", character);
        }
    }
}

int main(int argc, char *argv[]) {
    // If not enough arguments
    if (argc < 2) {
        printf("my-unzip: file1 [file2 ...]\n");
        return 1;
    }
    for (int i = 1; i < argc; i++) {
        FILE *file = fopen(argv[i], "rb");
        if (file == NULL) {
            printf("my-unzip: cannot open file\n");
            return 1;
        }
        decompress(file);
        fclose(file);
    }
    return 0;
}