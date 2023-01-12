#include "config.h"
#include "lcd.h"
#include "liberationmono.h"

const uint8_t DEBOUNCE = 1;   // 125 mSec ticks for btn debounce
const uint8_t FLASH_CTR = 1;  // 125 mSec ticks for btn detect flash
const sFONT* fontTop = &Liberation8;
const sFONT* fontBottom = &Liberation6;
const char* TO_MSG = "TIMEOUT";  
const char* ID_MSG = "ID";       

// colors
const short BACKGROUND = BLACK;
const short TOP_FOREGROUND = WHITE;
const short BOTTOM_FOREGROUND = BYELLOW;