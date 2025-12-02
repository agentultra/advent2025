int mod_add(int a, int b, int m) {
    return ((a % m) + (b % m)) % m;
}

int mod_sub(int a, int b, int m) {
    return ((a % m) - (b % m) + m) % m;
}
