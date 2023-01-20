#pragma once

#include "button.h"
#include <stdint.h>
#include <stdbool.h>

void buttonDemo(void);
void alphaKeyboard(void);
void regionButtons(void);
void highlightButton(uint16_t i, BUTTON_LAYOUT *layout, bool highlight);

extern BUTTON_LAYOUT alpha[];