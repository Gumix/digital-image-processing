/*
 * Inspired by:
 *
 * S.M. Pizer, R.E. Johnston, J.P. Ericksen, B.C. Yankaskas, K.E. Muller
 * Contrast-Limited Adaptive Histogram Equalization: Speed and Effectiveness
 * Proceedings of the First Conference on Visualization in Biomedical Computing (1990)
 */

#include <math.h>
#include <stdlib.h>
#include "png_wrapper.h"
#include "histogram.h"

static void
histogram_clip(size_t *hist, size_t limit)
{
	size_t excess_cnt = 0;
	for (size_t i = 0; i < NUM_COLORS; i++)
		if (hist[i] > limit)
		{
			excess_cnt += hist[i] - limit;
			hist[i] = limit;
		}
	size_t avg_incr = excess_cnt / NUM_COLORS;

	for (size_t i = 0; i < NUM_COLORS; i++)
	{
		size_t incr = hist[i] + avg_incr > limit
					? limit - hist[i]
					: avg_incr;
		hist[i] += incr;
		excess_cnt -= incr;
	}

	while (1)
		for (size_t i = 0; i < NUM_COLORS; i++)
		{
			if (hist[i] < limit)
			{
				hist[i]++;
				excess_cnt--;
			}

			if (!excess_cnt)
				return;
		}
}

static uint8_t
interpolate(double y, double x,
			uint8_t lu, uint8_t ru,
			uint8_t lb, uint8_t rb)
{
	int res = lround(lu * (1.0 - x) * (1.0 - y)
					 + ru * x * (1.0 - y)
					 + lb * (1.0 - x) * y
					 + rb * x * y);

	if (res < 0) res = 0;
	if (res > 255) res = 255;
	return res;
}

static void
process_image(struct Image img, size_t block_h, size_t block_w, double clip_limit_f)
{
	size_t block_ycnt = ceil((double) img.height / block_h);
	size_t block_xcnt = ceil((double) img.width / block_w);
	size_t clip_limit = clip_limit_f * block_h * block_w / NUM_COLORS;

	uint8_t luts[block_ycnt][block_xcnt][NUM_COLORS];

	for (size_t by = 0, y = 0; by < block_ycnt; by++, y += block_h)
		for (size_t bx = 0, x = 0; bx < block_xcnt; bx++, x += block_w)
		{
			size_t hist[NUM_COLORS] = { };
			size_t y_end = MIN(y + block_h, img.height);
			size_t x_end = MIN(x + block_w, img.width);
			histogram_calc(hist, img.pixels, y, y_end, x, x_end);
		    histogram_clip(hist, clip_limit);
			cdf_lut_calc_from_hist(luts[by][bx], hist, y, y_end, x, x_end);
		}

	for (size_t j = 0; j < block_ycnt; j++)
		for (size_t i = 0; i < block_xcnt; i++)
		{
			size_t y_end = MIN((j + 1) * block_h, img.height);
			size_t x_end = MIN((i + 1) * block_w, img.width);

			for (size_t y = j * block_h; y < y_end; y++)
				for (size_t x = i * block_w; x < x_end; x++)
				{
					size_t j0 = (y % block_h < block_h / 2)
								? MAX(0, (int) j - 1)
								: j;
					size_t i0 = (x % block_w < block_w / 2)
								? MAX(0, (int) i - 1)
								: i;

					size_t j1 = MIN(j0 + 1, block_ycnt - 1);
					size_t i1 = MIN(i0 + 1, block_xcnt - 1);
					double by = (double) y / block_h - j1 + 0.5;
					double bx = (double) x / block_w - i1 + 0.5;

					uint8_t p = img.pixels[y][x];
					uint8_t lu = luts[j0][i0][p];
					uint8_t ru = luts[j0][i1][p];
					uint8_t lb = luts[j1][i0][p];
					uint8_t rb = luts[j1][i1][p];
					img.pixels[y][x] = interpolate(by, bx, lu, ru, lb, rb);
				}
		}
}

int
main(int argc, char * const argv[])
{
	if (argc != 7)
		error("usage: %s <in_file> <out_file> <hist1_out_file> "
			  "<hist2_out_file> <block_size> <clip_limit>", argv[0]);

	char *input_filename  = argv[1];
	char *output_filename = argv[2];
	char *hist1_filename  = argv[3];
	char *hist2_filename  = argv[4];
	char *block_size_str  = argv[5];
	char *clip_limit_str  = argv[6];

	size_t block_size = strtoul(block_size_str, NULL, 10);

	double clip_limit = atof(clip_limit_str);
	if (clip_limit < 1.0 || clip_limit > 10.0)
		error("wrong clip limit (%f)", clip_limit);

	struct Image img = read_grayscale_png(input_filename);
	printf("Input file \"%s\" opened (width = %u, height = %u)\n",
		   input_filename, img.width, img.height);

	struct Image hist = { HIST_WIDTH, HIST_HEIGHT, NULL };
	alloc_pixels(&hist);
	histogram_draw(img, hist);
	write_grayscale_png(hist, hist1_filename);

	process_image(img, block_size, block_size, clip_limit);
	write_grayscale_png(img, output_filename);

	histogram_draw(img, hist);
	free_pixels(img);
	write_grayscale_png(hist, hist2_filename);
	free_pixels(hist);

	return EXIT_SUCCESS;
}
