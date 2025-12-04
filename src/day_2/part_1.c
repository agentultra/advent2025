#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

bool is_valid_id(const char* num, size_t size) {
    if (size % 2 != 0) {
        return true;
    }

    int mid = size / 2;

    for (int i = 0; i < mid; ++i) {
        if (num[i] != num[i+mid]) {
            return true;
        }
    }

    return false;
}

int main()
{
    int64_t sum = 0;
    uint64_t id1 = 0;
    uint64_t id2 = 0;

    while (fscanf(stdin, "%lu-%lu,", &id1, &id2) != EOF) {
        printf("%" PRIu64 "\n", id2);
        for (int64_t i = id1; i <= id2; ++i) {
            char nstr[200];
            sprintf(nstr, "%" PRIu64, i);
            if (!is_valid_id(nstr, (size_t)strlen(nstr))) {
                sum += i;
            }
        }
    }

    printf("Part 1: %" PRIi64 "\n", sum);

    return 0;
}
