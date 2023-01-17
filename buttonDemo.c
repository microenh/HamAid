#include "buttonDemo.h"
#include <stdint.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "lcd.h"
#include "button.h"

void buttonDemo(void) {
    Clear(BLACK);
    for(uint8_t y = 1; y < 14; y += 2) {
      for (uint8_t x = 0; x < 20; x += 4) {
        if (x == 4) {
          button(x, y, 4, 2, "ABCDEF", "IJKLMN", WHITE, BLACK, BRED, true);
        } else {
          button(x, y, 4, 2, "ABC", NULL, WHITE, BLACK, BRED, false);
        }
      }
    }
    DisplayOn(25);
    sleep_ms(15 * 1000);
}