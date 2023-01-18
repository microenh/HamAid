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
#include "lcd_touch.h"


bool tp_irq = false;

void gpio_irq(uint gpio, uint32_t events) {
  // printf("GPIO: %d, Events: %d\r\n", gpio, events);
  tp_irq = true;
}

static void gpio_pullup(uint8_t pin)
{
    gpio_set_dir(pin, GPIO_IN);
    gpio_pull_up(pin); 
}

void init_irq(void) {
  gpio_init(TP_IRQ_PIN);
  gpio_set_irq_enabled_with_callback(TP_IRQ_PIN, /* GPIO_IRQ_EDGE_RISE | */ GPIO_IRQ_EDGE_FALL, true, &gpio_irq);
}


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

  TP_Init();
  TP_GetAdFac();

  init_irq();  

}




void loop() {
  // testBTree();
  // fontDemo();
  // buttonDemo();
  // alphaKeyboard();
  // TP_DrawBoard();
  // sleep_ms(125);
  // Clear(BLACK);
  // DisplayOn(25);
  // DrawCross(WHITE, 20,20,10);
  // DrawCross(WHITE, 300,20,10);
  // DrawCross(WHITE, 20,220,10);
  // DrawCross(WHITE, 300,220,10);
  // looping = false;
  if (tp_irq) {
    tp_irq = false;
    TP_DrawBoard();
  }
}

int main(void)
{
    setup();

    while(looping) {
      loop();
    }
    //DisplayOff();
    return 0;
}