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

static void topLeft(const uint16_t x, const uint16_t y, const uint16_t fg, const uint16_t obg, const uint16_t ibg) {
    SetWindow(x, y, x + 3, y + 3);
    gpio_put(LCD_DC_PIN, 1);
    gpio_put(LCD_CS_PIN, 0);
    spi_write_blocking(SPI_PORT, (uint8_t *) &obg, 2);
    spi_write_blocking(SPI_PORT, (uint8_t *) &obg, 2);
    spi_write_blocking(SPI_PORT, (uint8_t *) &fg, 2);

    spi_write_blocking(SPI_PORT, (uint8_t *) &obg, 2);
    spi_write_blocking(SPI_PORT, (uint8_t *) &fg, 2);
    spi_write_blocking(SPI_PORT, (uint8_t *) &ibg, 2);

    spi_write_blocking(SPI_PORT, (uint8_t *) &fg, 2);
    spi_write_blocking(SPI_PORT, (uint8_t *) &ibg, 2);
    spi_write_blocking(SPI_PORT, (uint8_t *) &ibg, 2);
    gpio_put(LCD_CS_PIN, 1);
}

static void topRight(const uint16_t x, const uint16_t y, const uint16_t fg, const uint16_t obg, const uint16_t ibg) {
    SetWindow(x, y, x + 3, y + 3);
    gpio_put(LCD_DC_PIN, 1);
    gpio_put(LCD_CS_PIN, 0);
    spi_write_blocking(SPI_PORT, (uint8_t *) &fg, 2);
    spi_write_blocking(SPI_PORT, (uint8_t *) &obg, 2);
    spi_write_blocking(SPI_PORT, (uint8_t *) &obg, 2);

    spi_write_blocking(SPI_PORT, (uint8_t *) &ibg, 2);
    spi_write_blocking(SPI_PORT, (uint8_t *) &fg, 2);
    spi_write_blocking(SPI_PORT, (uint8_t *) &obg, 2);

    spi_write_blocking(SPI_PORT, (uint8_t *) &ibg, 2);
    spi_write_blocking(SPI_PORT, (uint8_t *) &ibg, 2);
    spi_write_blocking(SPI_PORT, (uint8_t *) &fg, 2);
    gpio_put(LCD_CS_PIN, 1);
}

static void bottomLeft(const uint16_t x, const uint16_t y, const uint16_t fg, const uint16_t obg, const uint16_t ibg) {
    SetWindow(x, y, x + 3, y + 3);
    gpio_put(LCD_DC_PIN, 1);
    gpio_put(LCD_CS_PIN, 0);
    spi_write_blocking(SPI_PORT, (uint8_t *) &fg, 2);
    spi_write_blocking(SPI_PORT, (uint8_t *) &ibg, 2);
    spi_write_blocking(SPI_PORT, (uint8_t *) &ibg, 2);

    spi_write_blocking(SPI_PORT, (uint8_t *) &obg, 2);
    spi_write_blocking(SPI_PORT, (uint8_t *) &fg, 2);
    spi_write_blocking(SPI_PORT, (uint8_t *) &ibg, 2);

    spi_write_blocking(SPI_PORT, (uint8_t *) &obg, 2);
    spi_write_blocking(SPI_PORT, (uint8_t *) &obg, 2);
    spi_write_blocking(SPI_PORT, (uint8_t *) &fg, 2);
    gpio_put(LCD_CS_PIN, 1);
}

static void bottomRight(const uint16_t x, const uint16_t y, const uint16_t fg, const uint16_t obg, const uint16_t ibg) {
    SetWindow(x, y, x + 3, y + 3);
    gpio_put(LCD_DC_PIN, 1);
    gpio_put(LCD_CS_PIN, 0);
    spi_write_blocking(SPI_PORT, (uint8_t *) &ibg, 2);
    spi_write_blocking(SPI_PORT, (uint8_t *) &ibg, 2);
    spi_write_blocking(SPI_PORT, (uint8_t *) &fg, 2);

    spi_write_blocking(SPI_PORT, (uint8_t *) &ibg, 2);
    spi_write_blocking(SPI_PORT, (uint8_t *) &fg, 2);
    spi_write_blocking(SPI_PORT, (uint8_t *) &obg, 2);

    spi_write_blocking(SPI_PORT, (uint8_t *) &fg, 2);
    spi_write_blocking(SPI_PORT, (uint8_t *) &obg, 2);
    spi_write_blocking(SPI_PORT, (uint8_t *) &obg, 2);
    gpio_put(LCD_CS_PIN, 1);
}

void button(const uint8_t gx, const uint8_t gy,
            const uint8_t gw, const uint8_t gh,
            const char *l0, const char *l1,
            const uint16_t fg, const uint16_t bg, const uint16_t ol,
            bool reverse) {

    uint16_t x = gx << 4;
    uint16_t y = gy << 4;
    uint16_t w = (gw << 4) - 1;
    uint16_t h = (gh << 4) - 1; 

    uint16_t bfg = fg;
    uint16_t bbg = bg;
    if (reverse) {
        bfg = bg;
        bbg = fg;
    }

    topLeft(x, y, ol, bg, bbg);                         // top left corner
    topRight(x + w - 3, y, ol, bg, bbg);                // top right corner
    bottomLeft(x, y + h - 3, ol, bg, bbg);              // bottom left corner
    bottomRight(x + w - 3, y + h - 3, ol, bg, bbg);     // bottom right corner

    ClearWindow(ol, x + 3, y, w - 6, 1);                // top side
    ClearWindow(bbg, x + 3, y + 1, w - 6, 2);           // top inside margin
    ClearWindow(ol, x, y + 3, 1, h - 6);                // left side    
    ClearWindow(ol, x + w - 1, y + 3, 1, h - 6);        // right side

    ClearWindow(bbg, x + 3, y + h - 3, w - 6, 2);       // bottom inside margin
    ClearWindow(ol, x + 3, y + h - 1, w - 6, 1);        // bottom side

    const sFONT *font;
    uint8_t text_height;
    uint8_t top_text_margin;
    uint8_t bottom_text_margin;
    if (l1) {
        font = Small;
        text_height = 2 * font->height;
    } else {
        font = Large;
        text_height = font->height;
    }

    bottom_text_margin = (h - 6 - text_height + 1) / 2;
    top_text_margin = h - 6 - text_height - bottom_text_margin;

    if (top_text_margin) {
        ClearWindow(bbg, x + 1, y + 3, w - 2, top_text_margin);    // top text margin
    }

    if (bottom_text_margin) {
        ClearWindow(bbg, x + 1, y + h - 3 - bottom_text_margin, w - 2, bottom_text_margin);    // bottom text margin
    }

    uint8_t text_width = strlen(l0) * font->width;

    uint8_t left_text_margin = (w - 2 - text_width + 1) / 2;
    uint8_t right_text_margin = w - 2 - text_width - left_text_margin;
 
    if (left_text_margin) {
        ClearWindow(bbg, x + 1, y + 3 + top_text_margin, left_text_margin, text_height);       // left text margin
    }

    if (right_text_margin) {
        ClearWindow(bbg, x + 1 + text_width + left_text_margin, y + 3 + top_text_margin,  right_text_margin, text_height);       // right text margin
    }

    DrawString(x + 1 + left_text_margin, y + 3 + top_text_margin, l0, font, bfg, bbg);
    if (l1) {
        DrawString(x + 1 + left_text_margin, y + 3 + top_text_margin + font->height, l1, font, bfg, bbg);
    }

    ClearWindow(bg, x + w, y, 1, h + 1);            // right margin
    ClearWindow(bg, x, y + h, w, 1);                // bottom margin
}
