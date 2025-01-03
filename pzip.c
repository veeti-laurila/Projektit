#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sysinfo.h>
#include <stddef.h>

#define CHUNK_SIZE 8192

typedef struct {
    int id;
    char *input;
    size_t start;
    size_t end;
    char *output;
    size_t output_size;
} ThreadData;

void run_length_encode(char *input, size_t start, size_t end, char *output, size_t *output_size) {
    char current_char = input[start];
    int count = 1;
    size_t output_index = 0;

    for (size_t i = start + 1; i <= end; i++) {
        if (input[i] == current_char && count < 255) {
            count++;
        } else {
            output[output_index++] = count;
            output[output_index++] = current_char;
            current_char = input[i];
            count = 1;
        }
    }
    *output_size = output_index;
}

void *compress(void *arg) {
    ThreadData * data = (ThreadData *)arg;

    data->output = malloc((data->end - data->start) * 2);
    if (!data->output) {
        perror("Failed to allocate output buffer");
        pthread_exit(NULL);
    }

    run_length_encode(data->input, data->start, data->end, data->output, &data->output_size);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        exit(1);
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("Error opening file");
        exit(1);
    }

    struct stat statbuf;
    if (fstat(fd, &statbuf) < 0) {
        perror("Error getting file size");
        exit(1);
    }
    size_t file_size = statbuf.st_size;

    char *input = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (input == MAP_FAILED) {
        perror("Error mapping file");
        exit(1);
    }
    close(fd);

    int num_threads = get_nprocs();
    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];

    size_t chunk_size = file_size / num_threads;
    for (int i = 0; i < num_threads; i++) {
        thread_data[i].id = 1;
        thread_data[i].input = input;
        thread_data[i].start = i * chunk_size;
        thread_data[i].end = (i == num_threads - 1) ? file_size - 1 : (i + 1) * chunk_size - 1;
        pthread_create(&threads[i], NULL, compress, &thread_data[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);

        fwrite(thread_data[i].output, 1, thread_data[i].output_size, stdout);

        free(thread_data[i].output);
    }

    munmap(input, file_size);
    return 0;
}
