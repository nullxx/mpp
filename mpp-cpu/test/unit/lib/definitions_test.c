#define MUNIT_ENABLE_ASSERT_ALIASES
#include "../../../src/lib/definitions.h"

#include "../../../src/lib/logger.h"
#include "../../_munit/munit.h"

void test_initialize_word(void) {
    Word test_word;
    initialize_word(&test_word, 0);
    for (int i = 0; i < WORD_SIZE_BIT; i++) {
        assert_int(test_word.bits[i], ==, 0);
    }
}

void test_word_to_int(void) {
    Word test_word;
    initialize_word(&test_word, 0);

    test_word.bits[0] = 1;
    test_word.bits[1] = 1;
    test_word.bits[2] = 1;
    test_word.bits[3] = 1;
    test_word.bits[4] = 1;
    test_word.bits[5] = 1;
    test_word.bits[6] = 1;
    test_word.bits[7] = 1;

    int result = word_to_int(test_word);
    assert_int(result, ==, 0xFF);
}

void test_int_to_word(void) {
    int num = 0xFF;
    Word result = int_to_word(num);

    assert_int(result.bits[0], ==, 1);
    assert_int(result.bits[1], ==, 1);
    assert_int(result.bits[2], ==, 1);
    assert_int(result.bits[3], ==, 1);
    assert_int(result.bits[4], ==, 1);
    assert_int(result.bits[5], ==, 1);
    assert_int(result.bits[6], ==, 1);
    assert_int(result.bits[7], ==, 1);
}

int main(void) {
    log_set_quiet(1);

    test_initialize_word();
    test_word_to_int();
    test_int_to_word();

    return 0;
}
