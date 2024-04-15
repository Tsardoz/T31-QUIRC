#include <stdio.h>
#include <gmp.h>

int main() {
    mpz_t n;
    mpz_init(n);
    mpz_set_ui(n, 123456789);
    gmp_printf("Here is a number: %Zd\n", n);
    mpz_clear(n);
    return 0;
}