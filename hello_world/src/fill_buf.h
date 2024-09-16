#ifndef __FILL_BUF_H__
#define __FILL_BUF_H__

#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/kernel.h>

#include <string.h>

enum corner {
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_RIGHT,
    BOTTOM_LEFT
};

typedef void (*fill_buffer)(enum corner corner, uint8_t grey, uint8_t *buf,
                size_t buf_size);

void fill_buffer_argb8888(enum corner corner, uint8_t grey, uint8_t *buf,
                 size_t buf_size);

void fill_buffer_rgb888(enum corner corner, uint8_t grey, uint8_t *buf,
                   size_t buf_size);

uint16_t get_rgb565_color(enum corner corner, uint8_t grey);

void fill_buffer_rgb565(enum corner corner, uint8_t grey, uint8_t *buf,
                   size_t buf_size);

void fill_buffer_bgr565(enum corner corner, uint8_t grey, uint8_t *buf,
                   size_t buf_size);

void fill_buffer_mono(enum corner corner, uint8_t grey,
                 uint8_t black, uint8_t white,
                 uint8_t *buf, size_t buf_size);

#endif // __FILL_BUF_H__
