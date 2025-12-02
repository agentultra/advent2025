#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "mod_arith.h"

#define VAULT_MAX 99
#define VAULT_MOD 100

int main()
{
    char buf[20];

    int current_num = 50;
    int zeroes_count = 0;

    while (fgets(buf, sizeof (buf), stdin) != NULL) {
        char turn = buf[0];
        int amt = strtol(buf+1, NULL, 10);

        assert(current_num >= 0 && current_num <= VAULT_MAX);

        int full_cycles = amt / VAULT_MOD;
        int remaining_cycles = amt % VAULT_MOD;

        if (turn == 'L') {
            // calculate the remaining turn, if there is one
            int extra = current_num != 0 && remaining_cycles >= current_num ? 1 : 0;

            zeroes_count += full_cycles + extra;
            current_num = mod_sub(current_num, amt, VAULT_MOD);
        }

        if (turn == 'R') {
            int extra = current_num + remaining_cycles >= VAULT_MOD ? 1 : 0;

            zeroes_count += full_cycles + extra;
            current_num = mod_add(current_num, amt, VAULT_MOD);
        }
    }

    printf("Day 2: %d\n", zeroes_count);

    return 0;
}
