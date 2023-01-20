#include "buttonDemo.h"
#include <stdint.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "lcd.h"
#include "button.h"
#include "config.h"


BUTTON_LAYOUT alpha[] = {
  {0,7,2,2,"0",NULL,WHITE,CYAN,BCYAN,false},
  {2,7,2,2,"1",NULL,WHITE,CYAN,BCYAN,false},
  {4,7,2,2,"2",NULL,WHITE,CYAN,BCYAN,false},
  {6,7,2,2,"3",NULL,WHITE,CYAN,BCYAN,false},
  {8,7,2,2,"4",NULL,WHITE,CYAN,BCYAN,false},
  {10,7,2,2,"5",NULL,WHITE,CYAN,BCYAN,false},
  {12,7,2,2,"6",NULL,WHITE,CYAN,BCYAN,false},
  {14,7,2,2,"7",NULL,WHITE,CYAN,BCYAN,false},
  {16,7,2,2,"8",NULL,WHITE,CYAN,BCYAN,false},
  {18,7,2,2,"9",NULL,WHITE,CYAN,BCYAN,false},

  {0,9,2,2,"Q",NULL,WHITE,CYAN,BCYAN,false},
  {2,9,2,2,"W",NULL,WHITE,CYAN,BCYAN,false},
  {4,9,2,2,"E",NULL,WHITE,CYAN,BCYAN,false},
  {6,9,2,2,"R",NULL,WHITE,CYAN,BCYAN,false},
  {8,9,2,2,"T",NULL,WHITE,CYAN,BCYAN,false},
  {10,9,2,2,"Y",NULL,WHITE,CYAN,BCYAN,false},
  {12,9,2,2,"U",NULL,WHITE,CYAN,BCYAN,false},
  {14,9,2,2,"I",NULL,WHITE,CYAN,BCYAN,false},
  {16,9,2,2,"O",NULL,WHITE,CYAN,BCYAN,false},
  {18,9,2,2,"P",NULL,WHITE,CYAN,BCYAN,false},

  {1,11,2,2,"A",NULL,WHITE,CYAN,BCYAN,false},
  {3,11,2,2,"S",NULL,WHITE,CYAN,BCYAN,false},
  {5,11,2,2,"D",NULL,WHITE,CYAN,BCYAN,false},
  {7,11,2,2,"F",NULL,WHITE,CYAN,BCYAN,false},
  {9,11,2,2,"G",NULL,WHITE,CYAN,BCYAN,false},
  {11,11,2,2,"H",NULL,WHITE,CYAN,BCYAN,false},
  {13,11,2,2,"J",NULL,WHITE,CYAN,BCYAN,false},
  {15,11,2,2,"K",NULL,WHITE,CYAN,BCYAN,false},
  {17,11,2,2,"L",NULL,WHITE,CYAN,BCYAN,false},

  {2,13,2,2,"Z",NULL,WHITE,CYAN,BCYAN,false},
  {4,13,2,2,"X",NULL,WHITE,CYAN,BCYAN,false},
  {6,13,2,2,"C",NULL,WHITE,CYAN,BCYAN,false},
  {8,13,2,2,"V",NULL,WHITE,CYAN,BCYAN,false},
  {10,13,2,2,"B",NULL,WHITE,CYAN,BCYAN,false},
  {12,13,2,2,"N",NULL,WHITE,CYAN,BCYAN,false},
  {14,13,2,2,"M",NULL,WHITE,CYAN,BCYAN,false},

  {16,5,4,2,"ENTER",NULL,WHITE,CYAN,BCYAN,true},
  {16,13,4,2,"BACK","SPACE",WHITE,CYAN,BCYAN,false}

};

void alphaKeyboard(void) {
  Clear(BACKGROUND);

  // button(16, 5, 4, 2, "ENTER", NULL, WHITE, CYAN, BCYAN, false, true, false);
  for (uint8_t i = 0; i < sizeof(alpha) / sizeof(alpha[0]); i++) {
    button(alpha[i], false, false);

  }
  // button(16,13,4,2,"BACK", "SPACE", WHITE, CYAN, BCYAN, false, false, false);
  DisplayOn(25);
  // sleep_ms(15 * 1000);
}

BUTTON_LAYOUT small[300];

void highlightButton(uint16_t i, BUTTON_LAYOUT *layout, bool highlight) {
  button(layout[i], highlight, true);    
}

void regionButtons(void) {
  Clear(BACKGROUND);
  for (uint8_t y = 0; y < 15; y += 1) {
    for (uint8_t x = 0; x < 20; x += 1) {
      BUTTON_LAYOUT t = {x,y,1,1,NULL,NULL,WHITE,CYAN,BCYAN,false};
      small[y * 20 + x] = t;
    }
  }
  for (uint16_t i=0; i<sizeof(small)/sizeof(small[0]); i++) {
      button(small[i], false, false);
  }
  DisplayOn(25);
}