#include "fontDemo.h"
#include <stdio.h>
#include "fonts.h"
#include "lcd.h"
#include "liberationmono.h"
#include "config.h"
#include "pico/stdlib.h"

void fontDemo(void) {
    uint y = 0;

    const short unsigned fgs[] = {TOP_FOREGROUND, BOTTOM_FOREGROUND};
    uint index = 0;

    const sFONT* fonts[] = {
        &Liberation6,
        &Liberation7,
        &Liberation8,
        &Liberation9,
        &Liberation10,
        &Liberation11,
        &Liberation12,
        &Liberation14,
        &Liberation16,
        &Liberation18,
        &Liberation20,
        &Liberation22,
        #if 0
        &Liberation24,
        &Liberation26,
        &Liberation28,
        &Liberation36,
        &Liberation48,
        &Liberation72   
        #endif 
    };

    uint points[] = {
        6,7,8,9,10,11,12,14,16,18,20,22,24,26,28,36,48,72
    };

    char disp_buf[6];
    Clear(BLACK);
    for (int i=0; i<12; i++) {
        sprintf(disp_buf, "%d pt", points[i]);
        DrawString(0, y, disp_buf, fonts[i], fgs[index], BACKGROUND);
        index = !index;
        y += fonts[i]->height + 1;
    }
    DisplayOn(25);
    sleep_ms(5 * 1000);
    DisplayOff();
}