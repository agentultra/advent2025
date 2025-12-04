#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// the greediest of algorithms, go forth my child and consume.
uint64_t max_joltage(const char* num, size_t num_len, uint8_t digit_len) {
    if (num_len < 2) {
        return -1;
    }

    if (digit_len > num_len) {
        return -1;
    }

    char* digit = (char*)malloc(digit_len+1);
    memset(digit, '0', digit_len+1);
    digit[0] = num[0];

    int remaining = digit_len - 1; // remaining digits to fill in
    int largest_i = 0;
    uint8_t next_digit = 1;

    // find largest first digit
    for (size_t i = 1; i < num_len - remaining; ++i) {
        if (num[i] > digit[0]) {
            digit[0] = num[i];
            largest_i = i;
        }
    }

    remaining--;

    // look for the next digit to fill in
    while (next_digit < digit_len) {
        // find the next largest digit after largest_i
        for (size_t i = largest_i + 1; i < num_len - remaining; ++i) {
            if (num[i] > digit[next_digit]) {
                digit[next_digit] = num[i];
                largest_i = i;
            }
        }
        next_digit++;
        remaining--;
    }

    digit[digit_len] = '\0';

    uint64_t result = strtoll(digit, NULL, 10);

    free(digit);

    return result;
}

int main()
{
    char buf[200];
    uint64_t sum_part1 = 0;
    uint64_t sum_part2 = 0;

    while (fgets(buf, sizeof (buf), stdin)) {
        size_t buf_len = strlen(buf);
        buf[buf_len-1] = '\0'; // strip newline
        buf_len--;
        uint64_t jolt_num1 = max_joltage(buf, buf_len, 2);
        uint64_t jolt_num2 = max_joltage(buf, buf_len, 12);
        sum_part1 += jolt_num1;
        sum_part2 += jolt_num2;
    }

    printf("Part 1: %" PRIu64 "\n", sum_part1);
    printf("Part 2: %" PRIu64 "\n", sum_part2);
    return 0;
}
