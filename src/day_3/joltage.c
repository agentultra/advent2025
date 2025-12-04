#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// the greediest of algorithms, go forth my child and consume.
int max_joltage(const char* num, size_t num_len) {
    if (num_len < 2) {
        return -1;
    }

    char digit[2] = {'0'};
    digit[0] = num[0];
    int largest_i = 0;

    // find largest tens digit
    for (size_t i = 1; i < num_len - 1; ++i) {
        if (num[i] > digit[0]) {
            digit[0] = num[i];
            largest_i = i;
        }
    }

    // find the next largest digit after largest_i
    for (size_t i = largest_i + 1; i < num_len; ++i) {
        if (num[i] > digit[1]) {
            digit[1] = num[i];
        }
    }

    return atoi(digit);
}

int main()
{
    char buf[200];
    uint64_t sum = 0;

    while (fgets(buf, sizeof (buf), stdin)) {
        size_t buf_len = strlen(buf);
        buf[buf_len-1] = '\0'; // strip newline
        buf_len--;
        int jolt_num = max_joltage(buf, buf_len);
        sum += jolt_num;
    }

    printf("Part 1: %" PRIu64 "\n", sum);
    return 0;
}
