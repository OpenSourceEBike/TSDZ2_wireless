#pragma once

/// msecs since boot (note: will roll over every 50 days)
// btw @casainho: the return value of this function is by definition never volatile, so I've removed that keyword
uint32_t get_time_base_counter_1ms(void);