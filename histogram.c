#include "histogram.h"

void histogram(struct Image in, struct Image out)
{
	size_t pixels_count[MAX_COLOR + 1] = { };

	for (size_t y = 0; y < in.height; y++)
		for (size_t x = 0; x < in.width; x++)
			pixels_count[in.pixels[y][x]]++;

	size_t max_count = 0;
	for (size_t c = 0; c <= MAX_COLOR; c++)
		if (pixels_count[c] > max_count)
			max_count = pixels_count[c];

	double k = HIST_HEIGHT / (double) max_count;

	for (size_t y = 0; y < HIST_HEIGHT; y++)
		for (size_t x = 0; x < HIST_WIDTH; x++)
			out.pixels[y][x] = 0;

	for (size_t c = 0; c <= MAX_COLOR; c++)
	{
		size_t x = c * 2;
		size_t h = pixels_count[c] * k + 0.5;
		if (h > HIST_HEIGHT) h = HIST_HEIGHT;

		for (size_t y = 0; y < h; y++)
		{
			out.pixels[HIST_HEIGHT - y - 1][x] = 255;
			out.pixels[HIST_HEIGHT - y - 1][x + 1] = 222;
		}
	}
}
