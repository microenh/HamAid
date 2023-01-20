#pragma once

#include "fonts.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct {
  uint8_t x, y, w, h;
  char *l0, *l1;
  uint16_t fg, bg, outline;
  bool smallfont;
} BUTTON_LAYOUT;

void button(const BUTTON_LAYOUT layout, bool inverse, bool update);

// void button(const uint8_t gx, const uint8_t gy,
//     const uint8_t gw, const uint8_t gh,
//     const char *l0, const char *l1,
//     const uint16_t fg, const uint16_t bg, const uint16_t outline,
//     bool reverse, bool smallfont, bool update);

int16_t buttonIndex(int16_t grid, BUTTON_LAYOUT *layout, int16_t button_count);