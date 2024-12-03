#include <stdio.h>
#include <stdint.h>

#ifndef BMPC
#define BMPC
extern "C" {
void bmp_write_rgba(FILE *out, uint32_t *pixels, int width, int height);
void bmp_write_gray8(FILE *out, uint8_t *pixels, uint32_t *pal, int width, int height);
}
#endif


