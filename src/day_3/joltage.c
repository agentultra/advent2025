#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 3643533343334553356323612233523343334333322332347332342233335334343353422313333362232334317333244336

// the greediest of algorithms, go forth my child and consume.
int max_joltage(const char* num, size_t num_len) {
    int i = 0;
    bool i_done = false;
    int j = (int)num_len;
    bool j_done = false;
    char digit[2] = {0};

    while (1) {
        assert(i < (int)num_len && j > 0);
        assert(i <= j);

        // consider moving `i`
        if (num[j] > num[i] && digit[0] != '9') {
            i++;

            if (digit[0] < num[i]) {
                digit[0] = num[i];
            }

            if (digit[0] == '9') {
                i_done = true;
            }
        }

        // consider moving `j`
        if (num[j] < num[i] && digit[1] != '9') {
            j--;

            if (digit[1] < num[j]) {
                digit[1] = num[j];
            }

            if (digit[1] == '9') {
                j_done = true;
            }
        }

        if (i + 1 == j || (i_done && j_done)) {
            break;
        }
    }

    return atoi(digit);
}

int main()
{
    char buf[101];
    uint64_t sum = 0;

    while (fgets(buf, sizeof (buf), stdin)) {
        int jolt_num = max_joltage(buf, strlen(buf));
        sum += jolt_num;
    }

    printf("Part 1: %" PRIu64 "\n", sum);
    return 0;
}
