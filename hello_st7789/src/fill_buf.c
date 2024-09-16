#include "fill_buf.h"

void fill_buffer_argb8888(enum corner corner, uint8_t grey, uint8_t *buf,
                 size_t buf_size)
{
    uint32_t color = 0;

    switch (corner) {
    case TOP_LEFT:
        color = 0x00FF0000u;
        break;
    case TOP_RIGHT:
        color = 0x0000FF00u;
        break;
    case BOTTOM_RIGHT:
        color = 0x000000FFu;
        break;
    case BOTTOM_LEFT:
        color = grey << 16 | grey << 8 | grey;
        break;
    }

    for (size_t idx = 0; idx < buf_size; idx += 4) {
        *((uint32_t *)(buf + idx)) = color;
    }
}

void fill_buffer_rgb888(enum corner corner, uint8_t grey, uint8_t *buf,
                   size_t buf_size)
{
    uint32_t color = 0;

    switch (corner) {
    case TOP_LEFT:
        color = 0x00FF0000u;
        break;
    case TOP_RIGHT:
        color = 0x0000FF00u;
        break;
    case BOTTOM_RIGHT:
        color = 0x000000FFu;
        break;
    case BOTTOM_LEFT:
        color = grey << 16 | grey << 8 | grey;
        break;
    }

    for (size_t idx = 0; idx < buf_size; idx += 3) {
        *(buf + idx + 0) = color >> 16;
        *(buf + idx + 1) = color >> 8;
        *(buf + idx + 2) = color >> 0;
    }
}

uint16_t get_rgb565_color(enum corner corner, uint8_t grey)
{
    uint16_t color = 0;
    uint16_t grey_5bit;

    switch (corner) {
    case TOP_LEFT:
        color = 0xF800u;
        break;
    case TOP_RIGHT:
        color = 0x07E0u;
        break;
    case BOTTOM_RIGHT:
        color = 0x001Fu;
        break;
    case BOTTOM_LEFT:
        grey_5bit = grey & 0x1Fu;
        /* shift the green an extra bit, it has 6 bits */
        color = grey_5bit << 11 | grey_5bit << (5 + 1) | grey_5bit;
        break;
    }
    return color;
}

void fill_buffer_rgb565(enum corner corner, uint8_t grey, uint8_t *buf,
                   size_t buf_size)
{
    uint16_t color = get_rgb565_color(corner, grey);

    for (size_t idx = 0; idx < buf_size; idx += 2) {
        *(buf + idx + 0) = (color >> 8) & 0xFFu;
        *(buf + idx + 1) = (color >> 0) & 0xFFu;
    }
}

void fill_buffer_bgr565(enum corner corner, uint8_t grey, uint8_t *buf,
                   size_t buf_size)
{
    uint16_t color = get_rgb565_color(corner, grey);

    for (size_t idx = 0; idx < buf_size; idx += 2) {
        *(uint16_t *)(buf + idx) = color;
    }
}

void fill_buffer_mono(enum corner corner, uint8_t grey,
                 uint8_t black, uint8_t white,
                 uint8_t *buf, size_t buf_size)
{
    uint16_t color;

    switch (corner) {
    case BOTTOM_LEFT:
        color = (grey & 0x01u) ? white : black;
        break;
    default:
        color = black;
        break;
    }

    memset(buf, color, buf_size);
}

