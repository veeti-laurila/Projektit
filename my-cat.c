#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // If no files
    if (argc < 2) {
        return 0;
    }

    // For loop for each file
    for (int i = 1; i < argc; i++) {
        // Open the file
        FILE *file = fopen(argv[i], "r");
        if (file == NULL) {
            printf("my-cat: cannot open file\n");
            return 1;
        }
        // Read and print the file
        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            printf("%s", buffer);
        }
        // Close the file
        fclose(file);
    }
    return 0;
}