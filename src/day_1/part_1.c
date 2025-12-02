#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "mod_arith.h"

#define VAULT_MAX 99

int main()
{
    char buf[20];

    int current_num = 50;
    int zeroes_count = 0;

    while (fgets(buf, sizeof (buf), stdin) != NULL) {
        char turn = buf[0];
        int amt = strtol(buf+1, NULL, 10);

        assert(current_num >= 0 && current_num <= 99);

        if (turn == 'L') {
            current_num = mod_sub(current_num, amt, VAULT_MAX + 1);
        }

        if (turn == 'R') {
            current_num = mod_add(current_num, amt, VAULT_MAX + 1);
        }

        if (current_num == 0) {
            zeroes_count++;
        }
    }

    printf("Day 1: %d\n", zeroes_count);

    return 0;
}
