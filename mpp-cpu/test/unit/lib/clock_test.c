#define MUNIT_ENABLE_ASSERT_ALIASES

#include "../../../src/lib/clock.h"

#include "../../_munit/munit.h"

int main(void) {
    int clock_tick1 = get_clock_tick();
    int clock_tick2 = get_clock_tick();

    assert_int(clock_tick1, !=, clock_tick2);
    assert_int(clock_tick1, ==, 1);
    assert_int(clock_tick2, ==, 0);

    return 0;
}
