#pragma once

#include "fonts.h"
#include <stdint.h>
#include <stdbool.h>

void button(const uint8_t gx, const uint8_t gy,
            const uint8_t gw, const uint8_t gh,
            const char *l0, const char *l1,
            const uint16_t fg, const uint16_t bg, const uint16_t ol,
            bool reverse);