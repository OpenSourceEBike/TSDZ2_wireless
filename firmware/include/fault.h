#pragma once

#include <stdint.h>

#ifdef SW102
#include "app_error.h"
#else

#define APP_ERROR_HANDLER(code) // FIXME - implement for 850c

#endif

// Standard app error codes
#define FAULT_SOFTDEVICE 1
#define FAULT_HARDFAULT 2
#define FAULT_NRFASSERT 3
#define FAULT_STACKOVERFLOW 4
#define FAULT_MISSEDTICK 5
#define FAULT_LOSTRX 6
#define FAULT_TSDZ2_WRONG_FIRMWARE 7
#define FAULT_GCC_ASSERT 10

void app_error_fault_handler(uint32_t id, uint32_t pc, uint32_t info);

