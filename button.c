#include "button.h"
#include "fonts.h"
#include "lcd.h"
#include "config.h"
#include "hardware/spi.h"
#include "pico/stdlib.h"
#include "liberationmono.h"
#include <stdint.h>

const sFONT *f[] = {&Liberation18, &Liberation9, &Liberation6};

static void topLeftBottomRight(const uint16_t x, const uint16_t y, const uint16_t fg, const uint16_t bg) {
    SetWindow(x, y, x + 3, y + 3);
    gpio_put(LCD_DC_PIN, 1);
    gpio_put(LCD_CS_PIN, 0);
    const bool border [] = {0,0,1, 0,1,0, 1,0,0};
    for (uint8_t i = 0; i < sizeof(border) / sizeof(border[1]); i++) {
        spi_write_blocking(SPI_PORT, (uint8_t *) (border[i] ? &fg : &bg), 2);
    }
    gpio_put(LCD_CS_PIN, 1);
}

static void topRightBottomLeft(const uint16_t x, const uint16_t y, const uint16_t fg, const uint16_t bg) {
    SetWindow(x, y, x + 3, y + 3);
    gpio_put(LCD_DC_PIN, 1);
    gpio_put(LCD_CS_PIN, 0);
    const bool border [] = {1,0,0, 0,1,0, 0,0,1};
    for (uint8_t i = 0; i < sizeof(border) / sizeof(border[1]); i++) {
        spi_write_blocking(SPI_PORT, (uint8_t *) (border[i] ? &fg : &bg), 2);
    }
    gpio_put(LCD_CS_PIN, 1);
}

// void DrawString(uint16_t x, uint16_t y, const char * pString, const sFONT* font, uint16_t foreground, uint16_t background);
// void ClearWindow(uint16_t color, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

void button(const uint8_t gx, const uint8_t gy,
            const uint8_t gw, const uint8_t gh,
            const char *l0, const char *l1, const char *l2,
            const uint16_t fg, const uint16_t bg, const uint16_t ol,
            bool reverse) {

    uint16_t x = gx << 4;
    uint16_t y = gy << 4;
    uint16_t w = (gw << 4) - 1;
    uint16_t h = (gh << 4) - 1; 

    topLeftBottomRight(x, y, ol, bg);                   // top left corner
    topRightBottomLeft(x + w - 3, y, ol, bg);           // top right corner
    topRightBottomLeft(x, y + h - 3, ol, bg);           // bottom left corner
    topLeftBottomRight(x + w - 3, y + h - 3, ol, bg);   // bottom right corner


    ClearWindow(ol, x + 3, y, w - 6, 1);                // top side
    ClearWindow(bg, x + 3, y + 1, w - 6, 2);            // top inside margin
    ClearWindow(ol, x, y + 3, 1, h - 6);                // left side    
    ClearWindow(ol, x + w - 1, y + 3, 1, h - 6);        // right side

    ClearWindow(bg, x + 3, y + h - 3, w - 6, 2);        // bottom inside margin
    ClearWindow(ol, x + 3, y + h - 1, w - 6, 1);        // bottom side

    // ClearWindow(bg, x + 1, y + 3, w - 2, h - 6);    // text area
    const sFONT *font = f[1];
    uint8_t top_text_margin = (h - 6 - font->height) / 2;
    uint8_t bottom_text_margin = h - 6 - font->height - top_text_margin;

    if (top_text_margin) {
        ClearWindow(bg, x + 1, y + 3, w - 2, top_text_margin);    // top text margin
    }

    if (bottom_text_margin) {
        ClearWindow(bg, x + 1, y + h - 3 - bottom_text_margin, w - 2, bottom_text_margin);    // bottom text margin
    }

    uint8_t left_text_margin = (w - 2 - 2 * font->width) / 2;
    uint8_t right_text_margin = w - 2 - 2 * font->width - left_text_margin;

    if (left_text_margin) {
        ClearWindow(bg, x + 1, y + 3 + top_text_margin, left_text_margin, font->height);       // left text margin
    }

    if (right_text_margin) {
        ClearWindow(bg, x + w - 2 - left_text_margin, y + 3 + top_text_margin,  left_text_margin, font->height);       // right text margin
    }

    DrawString(x + 1 + left_text_margin, y + 3 + top_text_margin, "AB", font, fg, bg);

    ClearWindow(bg, x + w, y, 1, h + 1);            // right margin
    ClearWindow(bg, x, y + h, w, 1);                // bottom margin
}
