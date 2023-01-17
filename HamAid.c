#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "button.h"
#include "btreeTest.h"

#include "config.h"
#include "DEV_Config.h"
#include "display.h"
#include "lcd.h"
#include "fontDemo.h"
#include "buttonDemo.h"
// #include "liberationmono.h"

bool looping = true;

void setup() {
    // static struct repeating_timer timer;

    set_sys_clock_48mhz();
    // add_repeating_timer_ms(125, heartbeat, NULL, &timer);
    System_Init();

    // const char * DirName = "/";
    // f_mount(&microSDFatFs, DirName, 1);

    InitPWM();
    
    /* LCD Init */
    InitLCD(HORIZONTAL);
}




void loop() {
  testBTree();
  fontDemo();
  buttonDemo();
  looping = false;
}

int main(void)
{
    setup();
    sleep_ms(15 * 1000);

    while(looping) {
      loop();
    }
    DisplayOff();
    return 0;
}