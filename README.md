# Advent of Code 2025

Just my collection of solutions to [Advent of Code](https://adventofcode.com/2025).

## Project Structure

Each solution is stored in `src/day_N/` for each day.

Common code can be added as header files in `includes/`.  The include
path is added to each solution.

## Building

All solutions are in C.  You'll need a compiler and `buildtools`
installed.

You can build the solution for a single day:

    make day DAY=<N>

Where `<N>` is a numbered day.  It will compile all the `*.c` files
it finds there into executables in the output directory named after
the source files.

## Running

You can run the solutions for a given day:

    make run DAY=<N>

Where `<N>` is a numbered day.  The target will test that the output
directory for that day is there and execute any binaries it finds in
that path.  Caveat emptor, I guess, _don't put silly things in that
path_.
