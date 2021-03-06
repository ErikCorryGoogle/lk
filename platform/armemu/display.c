/*
 * Copyright (c) 2010 Travis Geiselbrecht
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <err.h>
#include <debug.h>
#include <platform.h>
#include "platform_p.h"
#include <platform/armemu.h>
#include <dev/display.h>
#include <lib/gfx.h>
#include <reg.h>
#include <assert.h>

#define DRAW_TEST_PATTERN 0

static int display_w, display_h;
static void *display_fb;

inline static int has_display(void)
{
    return *REG32(SYSINFO_FEATURES) & SYSINFO_FEATURE_DISPLAY;
}

void platform_init_display(void)
{
    if (!has_display())
        return;

    display_fb = (void *)DISPLAY_FRAMEBUFFER;
    display_w = *REG32(DISPLAY_WIDTH);
    display_h = *REG32(DISPLAY_HEIGHT);

#if DRAW_TEST_PATTERN
    gfx_draw_pattern();
#endif
}

status_t display_get_framebuffer(struct display_framebuffer *fb)
{
    DEBUG_ASSERT(fb);
    if (!has_display())
        return ERR_NOT_FOUND;

    fb->image.format = IMAGE_FORMAT_RGB_x888;
    fb->image.pixels = display_fb;
    fb->image.width = display_w;
    fb->image.height = display_h;
    fb->image.stride = display_w;
    fb->image.rowbytes = display_w * 4;
    fb->flush = NULL;
    fb->format = DISPLAY_FORMAT_RGB_x888;

    return NO_ERROR;
}

status_t display_get_info(struct display_info *info)
{
    DEBUG_ASSERT(info);
    if (!has_display())
        return ERR_NOT_FOUND;

    info->format = DISPLAY_FORMAT_RGB_x888;
    info->width = display_w;
    info->height = display_h;

    return NO_ERROR;
}

