#include <assert.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

bool is_invalid_id(const char* n, int prefix_len, int num_repeats) {
    for (int i = 0; i < num_repeats; ++i) {
        if (strncmp(n, n + i * prefix_len, prefix_len) != 0) {
            return false;
        }
    }

    return true;
}

int main()
{
    int64_t sum = 0;
    uint64_t id1 = 0;
    uint64_t id2 = 0;

    while (fscanf(stdin, "%lu-%lu,", &id1, &id2) != EOF) {
        printf("id1: %" PRIu64", id2: %" PRIu64 "\n", id1, id2);
        for (uint64_t i = id1; i <= id2; ++i) {
            char num_str[21] = {0};
            sprintf(num_str, "%" PRIu64, i);
            int num_str_len = strlen(num_str);

            for (int prefix_len = 1; prefix_len <= num_str_len / 2; prefix_len++) {
                if (num_str_len % prefix_len != 0) {
                    continue;
                }

                int num_repeats = num_str_len / prefix_len;
                if (is_invalid_id(num_str, prefix_len, num_repeats)) {
                    sum += i;
                    break;
                }
            }
        }
    }

    printf("Part 2: %" PRIi64 "\n", sum);

    return 0;
}
// 1380780638002020 too high
