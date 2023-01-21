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
#include "liberationmono.h"



// uint8_t tp_irq = 0;

// void gpio_irq(uint gpio, uint32_t events) {
//   if ((gpio == TP_IRQ_PIN) ) {
//     tp_irq = events & GPIO_IRQ_EDGE_RISE ? GPIO_IRQ_EDGE_RISE : GPIO_IRQ_EDGE_FALL;
//   }
// }

// static void gpio_pullup(uint8_t pin) {
//     gpio_set_dir(pin, GPIO_IN);
//     gpio_pull_up(pin); 
// }

// void init_irq(void) {
//   gpio_pullup(TP_IRQ_PIN);
//   gpio_set_irq_enabled_with_callback(TP_IRQ_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_irq);
// }


bool looping = true;

bool tp_hb = false;

static bool heartbeat(struct repeating_timer *t) {
  tp_hb = true;
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

  // init_irq(); 
  // TP_calibrate();

  // printf("Starting...\r\n");
  // DisplayOff();
  // regionButtons();
  alphaKeyboard();
}

void loop() {
  char disp_buf[20];
  if (tp_hb) {
    tp_hb = false;
    TP_Update();
  }
  if (clear_grid > -1) {
    int16_t button = buttonIndex(clear_grid, alpha, 38);
    if (button > -1) {
      highlightButton(button, alpha, false);
    }
    clear_grid = -1;
  }
  if (highlight_grid > -1) {
    int16_t button = buttonIndex(highlight_grid, alpha, 38);
    if (button > -1) {
      highlightButton(button, alpha, true);
    }
    highlight_grid = -1;
  }

  if (press_grid > -1) {
    int16_t button_index = buttonIndex(press_grid, alpha, 38);
    if (button_index > -1) {
      sprintf(disp_buf, "%3d press", button_index);
      DrawString(0, 0, disp_buf, &Liberation18, BRED, BACKGROUND);
    }
    press_grid = -1;
  }

  if (hold_grid > -1) {
    int16_t button_index = buttonIndex(hold_grid, alpha, 38);
    if (button_index > -1) {
      sprintf(disp_buf, "%3d hold ", button_index);
      DrawString(0, 0, disp_buf, &Liberation18, BCYAN, BACKGROUND);
    }
    hold_grid = -1;
  }

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
  // DisplayOff();
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