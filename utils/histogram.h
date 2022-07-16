#pragma once

#include <stdint.h>
#include "png_wrapper.h"

#define MIN(a, b)	(((a) < (b)) ? (a) : (b))
#define MAX(a, b)	(((a) > (b)) ? (a) : (b))

enum { HIST_WIDTH = 512, HIST_HEIGHT = 256,
	   MAX_COLOR = 255, NUM_COLORS = MAX_COLOR + 1 };

void
histogram_calc(size_t *hist, uint8_t **pixels,
			   size_t y_start, size_t y_end,
			   size_t x_start, size_t x_end);

void
histogram_draw(struct Image in, struct Image out);

void
cdf_lut_calc(uint8_t *lut, uint8_t **pixels,
			 size_t y_start, size_t y_end,
			 size_t x_start, size_t x_end);

void
cdf_lut_calc_from_hist(uint8_t *lut, size_t *hist,
					   size_t y_start, size_t y_end,
					   size_t x_start, size_t x_end);
