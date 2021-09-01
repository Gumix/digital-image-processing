#include "histogram.h"

void
histogram_calc(size_t *hist, uint8_t **pixels,
			   size_t y_start, size_t y_end,
			   size_t x_start, size_t x_end)
{
	for (size_t y = y_start; y < y_end; y++)
		for (size_t x = x_start; x < x_end; x++)
			hist[pixels[y][x]]++;
}

void
histogram_draw(struct Image in, struct Image out)
{
	size_t hist[MAX_COLOR + 1] = { };
	histogram_calc(hist, in.pixels, 0, in.height, 0, in.width);

	size_t max_count = 0;
	for (size_t c = 0; c <= MAX_COLOR; c++)
		if (hist[c] > max_count)
			max_count = hist[c];

	double k = (double) HIST_HEIGHT / max_count;

	for (size_t y = 0; y < HIST_HEIGHT; y++)
		for (size_t x = 0; x < HIST_WIDTH; x++)
			out.pixels[y][x] = 0;

	for (size_t c = 0; c <= MAX_COLOR; c++)
	{
		size_t x = c * 2;
		size_t h = MIN(hist[c] * k + 0.5, HIST_HEIGHT);

		for (size_t y = 0; y < h; y++)
		{
			out.pixels[HIST_HEIGHT - y - 1][x] = 255;
			out.pixels[HIST_HEIGHT - y - 1][x + 1] = 222;
		}
	}
}

void
cdf_lut_calc(uint8_t *lut, uint8_t **pixels,
			 size_t y_start, size_t y_end,
			 size_t x_start, size_t x_end)
{
	size_t hist[NUM_COLORS] = { };
	histogram_calc(hist, pixels, y_start, y_end, x_start, x_end);

	// Cumulative distribution function
	double cdf[NUM_COLORS];
	size_t total_pixels = (y_end - y_start) * (x_end - x_start);
	cdf[0] = (double) hist[0] / total_pixels;

	for (size_t i = 1; i <= MAX_COLOR; i++)
		cdf[i] = cdf[i - 1] + (double) hist[i] / total_pixels;

	for (size_t i = 0; i <= MAX_COLOR; i++)
		lut[i] = cdf[i] * MAX_COLOR;
}
