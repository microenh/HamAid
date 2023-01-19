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



uint8_t tp_irq = 0;
uint8_t last_tp_irq = 0;

void gpio_irq(uint gpio, uint32_t events) {
  if ((gpio == TP_IRQ_PIN) ) {
    uint8_t new_tp_irq = events & GPIO_IRQ_EDGE_RISE ? GPIO_IRQ_EDGE_RISE : GPIO_IRQ_EDGE_FALL;
    if (new_tp_irq != last_tp_irq) {
      tp_irq = new_tp_irq;
    }
  }
}

static void gpio_pullup(uint8_t pin) {
    gpio_set_dir(pin, GPIO_IN);
    gpio_pull_up(pin); 
}

void init_irq(void) {
  gpio_pullup(TP_IRQ_PIN);
  gpio_set_irq_enabled_with_callback(TP_IRQ_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_irq);
}


bool looping = true;



static bool heartbeat(struct repeating_timer *t) {
  if (tp_irq) {
    last_tp_irq = tp_irq;
    printf("%sing\r\n", tp_irq == GPIO_IRQ_EDGE_RISE ? "ris" : "fall");
    if (tp_irq == GPIO_IRQ_EDGE_RISE) {
      printf("\r\n");
    }
    tp_irq = 0;
  }
}


static void TP_calibrate(void) {
  TP_XY raw[4];
  TP_XY dummy;
  const TP_XY cross[] = {{20,20},{300,20},{20,220},{300,220}};
  Clear(BLACK);
  DisplayOn(25);
  for (uint8_t i = 0; i < 4; i++) {
    bool done = false;
    DrawCross(WHITE, cross[i].x, cross[i].y, 10);
    while (!done) {
      done = raw_touch(raw + i);
      sleep_ms(125);
    }
    DrawCross(BLACK, cross[i].x, cross[i].y, 10);
    done = true;
    while (done) {
      done = raw_touch(&dummy);
      sleep_ms(125);
    }
  } 
  DisplayOff();
  calibrate(raw);
}

void setup() {
  static struct repeating_timer timer;

  set_sys_clock_48mhz();
  add_repeating_timer_ms(125, heartbeat, NULL, &timer);
  System_Init();

  // const char * DirName = "/";
  // f_mount(&microSDFatFs, DirName, 1);

  InitPWM();
  
  /* LCD Init */
  InitLCD(HORIZONTAL);

  TP_Init();

  init_irq(); 
  // TP_calibrate();

  printf("Starting...\r\n");
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
  DisplayOff();
  // TP_XY point;
  // if (tp_irq == IRQ_FALL) {
  //   if (TP_Scan(&point)) {
  //     tp_irq = false;
  //     // printf("(%d,%d)\r\n", point.x, point.y);
  //   }
  // }
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