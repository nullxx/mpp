#define MUNIT_ENABLE_ASSERT_ALIASES

#include "../../../src/lib/utils.h"

#include "../../_munit/munit.h"

void test_get_used_bits(void) {
    int i1 = 1;
    int i1u = get_used_bits(i1);

    assert_int(i1u, ==, 1);

    int i2 = 2;
    int i2u = get_used_bits(i2);

    assert_int(i2u, ==, 2);

    int i3 = 3;
    int i3u = get_used_bits(i3);

    assert_int(i3u, ==, 2);
}

void test_get_num_digits(void) {
    int n, nc;

    for (int i = 1; i < 9; i++) {
        n = i;
        nc = get_num_digits(n);

        assert_int(nc, ==, 1);
    }

    for (int i = 10; i < 99; i++) {
        n = i;
        nc = get_num_digits(n);

        assert_int(nc, ==, 2);
    }

    for (int i = 100; i < 999; i++) {
        n = i;
        nc = get_num_digits(n);

        assert_int(nc, ==, 3);
    }
}

void test_random_int(void) {
    // this only checks values are inside bounds
    int n;
    srand(0);

    for (int i = 0; i < 1000; i++) {
        n = random_int(0, 100);
        assert_int(n, >=, 0);
        assert_int(n, <=, 100);
    }
}

void test_num_to_str(void) {
    int num = 25;
    char *num_str = num_to_str(num);

    assert_string_equal(num_str, "25");
    free(num_str);

    num = 105;
    num_str = num_to_str(num);

    assert_string_equal(num_str, "105");
    free(num_str);

    num = 13502;
    num_str = num_to_str(num);

    assert_string_equal(num_str, "13502");
    free(num_str);
}

void test_get_bit(void) {
    int n = 1;

    int bn0 = get_bit(n, 0);
    int bn1 = get_bit(n, 1);
    assert_int(bn0, ==, 1);
    assert_int(bn1, ==, 0);

    n = 3;
    bn0 = get_bit(n, 0);
    bn1 = get_bit(n, 1);
    assert_int(bn0, ==, 1);
    assert_int(bn1, ==, 1);

    n = 15;
    bn0 = get_bit(n, 0);
    bn1 = get_bit(n, 1);
    int bn2 = get_bit(n, 2);
    int bn3 = get_bit(n, 3);
    assert_int(bn0, ==, 1);
    assert_int(bn1, ==, 1);
    assert_int(bn2, ==, 1);
    assert_int(bn3, ==, 1);
}

int main(void) {
    test_get_used_bits();
    test_get_num_digits();
    test_random_int();
    test_num_to_str();
    test_get_bit();

    return 0;
}
