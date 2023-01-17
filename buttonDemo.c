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
          button(x, y, 4, 2, "ABCDEF", "IJKLMN", WHITE, BLACK, BLACK, BRED, true, false);
        } else {
          button(x, y, 4, 2, "ABC", NULL, WHITE, BLACK, BLACK, BRED, false, false);
        }
      }
    }
    DisplayOn(25);
    sleep_ms(15 * 1000);
}

typedef struct {
  uint8_t x, y;
  char *text;
} BUTTON_LAYOUT;

void alphaKeyboard(void) {
  BUTTON_LAYOUT alpha[] = {
    {0,7,"0"},
    {2,7,"1"},
    {4,7,"2"},
    {6,7,"3"},
    {8,7,"4"},
    {10,7,"5"},
    {12,7,"6"},
    {14,7,"7"},
    {16,7,"8"},
    {18,7,"9"},

    {0,9,"Q"},
    {2,9,"W"},
    {4,9,"E"},
    {6,9,"R"},
    {8,9,"T"},
    {10,9,"Y"},
    {12,9,"U"},
    {14,9,"I"},
    {16,9,"O"},
    {18,9,"P"},

    {1,11,"A"},
    {3,11,"S"},
    {5,11,"D"},
    {7,11,"F"},
    {9,11,"G"},
    {11,11,"H"},
    {13,11,"J"},
    {15,11,"K"},
    {17,11,"L"},

    {2,13,"Z"},
    {4,13,"X"},
    {6,13,"C"},
    {8,13,"V"},
    {10,13,"B"},
    {12,13,"N"},
    {14,13,"M"},
  
  };

  Clear(BLACK);

  button(16, 5, 4, 2, "ENTER", NULL, WHITE, CYAN, BLACK, BCYAN, false, true);
  for (uint8_t i = 0; i < sizeof(alpha) / sizeof(alpha[0]); i++) {
    button(alpha[i].x, alpha[i].y, 2, 2, alpha[i].text, NULL, WHITE, CYAN, BLACK, BCYAN, false, false);

  }
  button(16,13,4,2,"BACK", "SPACE", WHITE, CYAN, BLACK, BCYAN, false, false);
  DisplayOn(25);
  // sleep_ms(15 * 1000);
}