#include "gumby.h"

long gumby::Raise(int base, int exponent) {
    long product = 1;
    for (int i = 0; i < exponent; i++) {
        product *= base;
    }
    return product;
}
