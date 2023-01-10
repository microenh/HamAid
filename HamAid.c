#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

#include "config.h"
#include "display.h"
#include "lcd.h"
#include "fonts/liberationmono/liberationmono.h"

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments

void setup() {
    // static struct repeating_timer timer;

    // set_sys_clock_48mhz();
    // add_repeating_timer_ms(125, heartbeat, NULL, &timer);
    InitHardware();
    
    BacklightLevel(0);
    /* LCD Init */
    InitLCD(HORIZONTAL);
    Invert(true);
    Clear(BACKGROUND);


    BacklightLevel(25);

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

    for (int i=0; i<12; i++) {
        sprintf(disp_buf, "%d pt", points[i]);
        DrawString(0, y, disp_buf, fonts[i], fgs[index], BACKGROUND);
        index = !index;
        y += fonts[i]->height + 1;
    }

}


void loop() {

}

int main(void)
{
    setup();
    while(1){
        loop();
    }
    return 0;
}