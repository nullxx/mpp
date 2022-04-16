
int last_clock_tick = 0;
int get_clock_tick(void) {
    last_clock_tick = !last_clock_tick;
    return last_clock_tick;
}