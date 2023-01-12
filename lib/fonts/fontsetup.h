#pragma once

#include "fonts.h"

#define FONT_SETUP(data, fontname, width, height) \
    extern const unsigned char data[]; \
    const sFONT fontname = {data, width, height}
