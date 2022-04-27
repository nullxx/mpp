#define MUNIT_ENABLE_ASSERT_ALIASES

#include "../../../src/lib/watcher.h"

#include "../../_munit/munit.h"

void test_register_watcher(RegisterWatcher *rw) {
    register_watcher(rw);  // nothing to test
}

void test_unregister_watcher(RegisterWatcher *rw) {
    int success = unregister_watcher(rw);
    assert_true(success);

    success = unregister_watcher(rw);
    assert_false(success);
}

void test_get_register(Register *pr) {
    Register *r = get_register(WATCHER_TYPE_ACUM);

    assert_ptr_equal(r, pr);
}

int main(void) {
    Register reg = {.bit_length = 1, .value = 1};
    RegisterWatcher rw = {.type = WATCHER_TYPE_ACUM, .reg = &reg};

    test_register_watcher(&rw);
    test_get_register(&reg);
    test_unregister_watcher(&rw);

    return 0;
}
