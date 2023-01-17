#include "button.h"
#include "fonts.h"
#include "lcd.h"
#include "config.h"
#include "hardware/spi.h"
#include "pico/stdlib.h"
#include "liberationmono.h"
#include <stdint.h>
#include <string.h>

const sFONT *Large = &Liberation18;
const sFONT *Small = &Liberation9;

const uint8_t TOP_LEFT[] = {1,1,0, 1,0,2, 0,2,2};
const uint8_t TOP_RIGHT[] = {0,1,1, 2,0,1, 2,2,0};
const uint8_t BOTTOM_LEFT[] = {0,2,2, 1,0,2, 1,1,0};
const uint8_t BOTTOM_RIGHT[] = {2,2,0, 2,0,1, 0,1,1};

static void corner(const uint8_t *which, const uint16_t x, const uint16_t y, const uint16_t fg, const uint16_t obg, const uint16_t ibg) {
    const uint16_t colors[] = {fg, obg, ibg};

    SetWindow(x, y, x + 3, y + 3);
    gpio_put(LCD_DC_PIN, 1);
    gpio_put(LCD_CS_PIN, 0);
    for (uint8_t i = 0; i < 9; i++) {
        spi_write_blocking(SPI_PORT, (uint8_t *) &colors[which[i]], 2);
    }
    gpio_put(LCD_CS_PIN, 1);

}

static void frame(const uint16_t x, const uint16_t y,
  const uint16_t w, const uint16_t h,
  const uint16_t bg, const uint16_t screen_bg, const uint16_t outline) {
  ClearWindow(screen_bg, x + w, y, 1, h + 1); // right margin
  ClearWindow(screen_bg, x, y + h, w, 1);     // bottom margin

  corner(TOP_LEFT, x, y, outline, screen_bg, bg); // top left corner

  ClearWindow(outline, x + 3, y, w - 6, 1);        // top side
  ClearWindow(bg, x + 3, y + 1, w - 6, 2);         // top inside margin

  corner(TOP_RIGHT, x + w - 3, y, outline, screen_bg, bg);  // top right corner

  ClearWindow(outline, x + w - 1, y + 3, 1, h - 6);         // right side

  corner(BOTTOM_LEFT, x, y + h - 3, outline, screen_bg, bg); // bottom right corner

  ClearWindow(bg, x + 3, y + h - 3, w - 6, 2);               // bottom inside margin
  ClearWindow(outline, x + 3, y + h - 1, w - 6, 1);          // bottom side

  corner(BOTTOM_RIGHT, x + w - 3, y + h - 3, outline, screen_bg, bg); // bottom left corner

  ClearWindow(outline, x, y + 3, 1, h - 6);                // left side    
}

static void text_wings(const uint16_t x, const uint16_t y,
  const uint16_t w, const uint16_t h, const uint16_t bg) {
  ClearWindow(bg, x + 2, y + 1, w - 4, 2);         // top inside margin
  ClearWindow(bg, x + 1, y + 2, 1, 1);
  ClearWindow(bg, x + w - 2, y + 2, 1, 1);

  ClearWindow(bg, x + 1, y + h - 3, 1, 1);
  ClearWindow(bg, x + w - 2, y + h - 3, 1, 1);
  ClearWindow(bg, x + 2, y + h - 3, w - 4, 2);     // bottom inside margin
}

static void vertical_text_margins(const uint16_t x, const uint16_t y,
  const uint16_t w, const uint16_t h,
  const uint16_t top_text_margin, const uint16_t bottom_text_margin,
  const uint16_t bg) {
  if (top_text_margin) {
      ClearWindow(bg, x, y, w, top_text_margin);    // top text margin
  }
  if (bottom_text_margin) {
      ClearWindow(bg, x, y + h - bottom_text_margin, w, bottom_text_margin);    // bottom text margin
  }
}

static void text_line(const uint16_t x, const uint16_t y,
  const uint16_t w, const sFONT *font, const char *text, 
  const uint16_t fg, const uint16_t bg) {
  uint8_t text_width = strlen(text) * font->width;

  uint8_t left_text_margin = (w - text_width + 1) / 2;
  uint8_t right_text_margin = w - text_width - left_text_margin;

  if (left_text_margin) {
      ClearWindow(bg, x, y, left_text_margin, font->height);       // left text margin
  }

  if (right_text_margin) {
      ClearWindow(bg, x + text_width + left_text_margin, y,  right_text_margin, font->height);       // right text margin
  }
  DrawString(x + left_text_margin, y, text, font, fg, bg);

}

static void textArea(const uint16_t x, const uint16_t y,
  const uint16_t w, const uint16_t h,
  const uint16_t fg, const uint16_t bg,
  const char *l0, const bool smallfont) {
  const sFONT *font;
  uint8_t text_height;
  uint8_t top_text_margin;
  uint8_t bottom_text_margin;

  font = smallfont ? Small : Large;

  bottom_text_margin = (h - font->height + 1) / 2;
  top_text_margin = h - font->height - bottom_text_margin;

  vertical_text_margins(x, y, w, h, top_text_margin, bottom_text_margin, bg);
  text_line(x, y + top_text_margin, w, font, l0, fg, bg);
}

static void textArea2(const uint16_t x, const uint16_t y,
  const uint16_t w, const uint16_t h,
  const uint16_t fg, const uint16_t bg,
  const char *l0, const char *l1) {
  uint8_t text_height;
  uint8_t top_text_margin;
  uint8_t bottom_text_margin;

  bottom_text_margin = (h - 2 * Small->height + 1) / 2;
  top_text_margin = h - 2 * Small->height - bottom_text_margin;

  vertical_text_margins(x, y, w, h, top_text_margin, bottom_text_margin, bg);

  text_line(x, y + top_text_margin, w, Small, l0, fg, bg);
  text_line(x, y + top_text_margin + Small->height, w, Small, l1, fg, bg);
}

void button(const uint8_t gx, const uint8_t gy,
    const uint8_t gw, const uint8_t gh,
    const char *l0, const char *l1,
    const uint16_t fg, const uint16_t bg,
    const uint16_t screen_bg, const uint16_t outline,
    bool reverse, bool smallfont) {

    uint16_t x = gx << 4;
    uint16_t y = gy << 4;
    uint16_t w = (gw << 4) - 1;
    uint16_t h = (gh << 4) - 1; 

    uint16_t button_fg = fg;
    uint16_t button_bg = bg;
    if (reverse) {
        button_fg = bg;
        button_bg = fg;
    }

    frame(x, y, w, h, button_bg, screen_bg, outline);

    if (l1) {
      textArea2(x + 1, y + 3, w - 2, h - 6, fg, bg, l0, l1);
    } else {
      textArea(x + 1, y + 3, w - 2, h - 6, fg, bg, l0, smallfont);
    }
  
}
