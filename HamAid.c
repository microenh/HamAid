#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "button.h"

#include "config.h"
#include "DEV_Config.h"
#include "display.h"
#include "lcd.h"
#include "fontDemo.h"
#include "buttonDemo.h"
#include "lcd_touch.h"
#include "liberationmono.h"
#include "ff.h"
#include "lib/sqlite/cAPI.h"


bool looping = true;

bool tp_hb = false;

static bool heartbeat(struct repeating_timer *t) {
  tp_hb = true;
}

FATFS microSDFatFs;

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

char input[11] = "";
sqliteReader *r;


void setup() {
  static struct repeating_timer timer;

  set_sys_clock_48mhz();
  add_repeating_timer_ms(125, heartbeat, NULL, &timer);
  System_Init();

  const char * DirName = "/";
  f_mount(&microSDFatFs, DirName, 1);
  r = sql_open("fcc.db");
  
  InitPWM();
  
  /* LCD Init */
  InitLCD(HORIZONTAL);

  TP_Init();

  TP_calibrate();

  // printf("Starting...\r\n");
  // DisplayOff();
  // regionButtons();
  alphaKeyboard();
}

void loop() {
  LOOKUP *lu;
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
    int8_t button_index = buttonIndex(press_grid, alpha, 38);
    if (button_index < 36) {
      uint16_t l = strlen(input);
      input[l] = alpha[button_index].l0[0];
      input[l+1] = 0;
    } else if (button_index == 36) {
      ClearWindow(BACKGROUND, 0, 0, 320, 4 * Liberation10.height);
      ClearWindow(BACKGROUND, 0, 80, 256, Liberation18.height);
      lu = sql_find_call(r, input);
      if (lu) {
        DrawString(0, 0, lu->callsign, &Liberation10, BCYAN, BACKGROUND);
        DrawString(0, Liberation10.height, lu->entity_name, &Liberation10, BCYAN, BACKGROUND);
      } else {
        DrawString(0, 0, "NOT FOUND", &Liberation10, BRED, BACKGROUND);  
      }
      input[0] = 0;
    } else if (button_index == 37) {
      uint16_t l = strlen(input);
      if (l) {
        input[l-1] = 0;
        DrawString(Liberation18.width * (l-1) , 80, " ", &Liberation18, BCYAN, BACKGROUND);
      }
    }
    // sprintf(disp_buf, "%s", input);
    DrawString(0, 80, input, &Liberation18, BCYAN, BACKGROUND);
    press_grid = -1;
  }

  if (hold_grid > -1) {
    // sprintf(disp_buf, "%3d hold ", buttonIndex(hold_grid, alpha, 38));
    // DrawString(0, 0, disp_buf, &Liberation18, BCYAN, BACKGROUND);
    hold_grid = -1;
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