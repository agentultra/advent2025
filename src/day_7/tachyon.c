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
    uint64_t split_count = 0;

    while (fgets(buf, sizeof (buf), stdin)) {
        size_t buf_len = strlen(buf);
        buf[buf_len-1] = '\0';
        buf_len--;

        printf("%s\n", buf);
        if (beams == NULL) {
            // not the most efficient way to pack flags but it will do, pig.
            beams = (bool*)malloc(buf_len);
        }

        for (size_t i = 0; i < buf_len; ++i) {
            if (buf[i] == 'S') {
                beams[i] = true;
            }

            if (buf[i] == '^' && beams[i]) {
                beams[i-1] = true;
                beams[i] ^= true;
                beams[i+1] = true;
                split_count++;
            }
        }
    }

    printf("Part 1: %" PRIu64 "\n", split_count);
    return 0;
}
