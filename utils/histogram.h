#pragma once

#include <stdint.h>
#include "png_wrapper.h"

#define MIN(a, b)	(((a) < (b)) ? (a) : (b))
#define MAX(a, b)	(((a) > (b)) ? (a) : (b))

enum { HIST_WIDTH = 512, HIST_HEIGHT = 256, MAX_COLOR = 255 };

void histogram_calc(size_t *hist, uint8_t **pixels,
					size_t y_start, size_t y_end,
					size_t x_start, size_t x_end);
void histogram_draw(struct Image in, struct Image out);
