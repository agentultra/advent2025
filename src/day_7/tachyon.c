#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 1024

int main()
{
    char buf[BUF_SIZE] = {0};
    bool* beams = NULL;
    uint64_t* paths = NULL;
    size_t paths_len = 0;
    uint64_t* next_paths = NULL;
    uint64_t split_count = 0;

    while (fgets(buf, sizeof (buf), stdin)) {
        size_t buf_len = strlen(buf);
        buf[buf_len-1] = '\0';
        buf_len--;

        if (beams == NULL) {
            // not the most efficient way to pack flags but it will do, pig.
            beams = (bool*)malloc(buf_len);
            paths =
                (uint64_t*)calloc(buf_len * sizeof (uint64_t),
                                  sizeof (uint64_t));
            paths_len = buf_len;
            next_paths =
                (uint64_t*)calloc(buf_len * sizeof (uint64_t),
                                  sizeof (uint64_t));
        }

        for (size_t i = 0; i < buf_len; ++i) {
            if (buf[i] == 'S') {
                beams[i] = true;
                next_paths[i] = 1;
            }

            if (buf[i] == '.' && paths[i] > 0) {
                next_paths[i] += paths[i];
            }

            if (buf[i] == '^' && beams[i]) {
                beams[i-1] = true;
                beams[i] ^= true;
                beams[i+1] = true;
                split_count++;

                next_paths[i-1] += paths[i];
                next_paths[i+1] += paths[i];
            }
        }

        memcpy(paths, next_paths, buf_len * sizeof(int));
        memset(next_paths, 0, buf_len * sizeof (int));
    }

    printf("Part 1: %" PRIu64 "\n", split_count);

    uint64_t sum_2 = 0;

    for (size_t i = 0; i < paths_len; ++i) {
        sum_2 += paths[i];
    }

    printf("Part 2: %" PRIu64 "\n", sum_2);

    return 0;
}

// 34768417257 too low
// 17553948956660 too low
