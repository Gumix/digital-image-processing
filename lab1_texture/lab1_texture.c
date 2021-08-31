/*
 * Inspired by:
 *
 * Oleg Verevka, John W. Buchanan
 * Halftoning with image-based dither screens
 * Proceedings of the 1999 conference on Graphics interface
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "png_wrapper.h"
#include "histogram.h"

#define MIN(a, b)	(((a) < (b)) ? (a) : (b))

static double **
alloc_errors(size_t height, size_t width)
{
	double **p = malloc(height * sizeof(double *));
	if (!p)
		error("can not allocate memory");

	for (size_t i = 0; i < height; i++)
	{
		p[i] = malloc(width * sizeof p[0]);
		if (p[i] == NULL)
			error("can not allocate memory");
		memset(p[i], 0, width * sizeof p[i][0]);
	}

	return p;
}

static void
free_errors(double **errors, size_t height)
{
	for (size_t i = 0; i < height; i++)
		free(errors[i]);
	free(errors);
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
add(double **errors, size_t x, size_t y, size_t width, size_t height, double a)
{
	if (x < 0 || y < 0 || x >= width || y >= height)
		return;

	errors[y][x] += a;
}

static void
texture_ahe(struct Image img, size_t block_h, size_t block_w)
{
	size_t block_ycnt = ceil((double) img.height / block_h);
	size_t block_xcnt = ceil((double) img.width / block_w);

	uint8_t luts[block_ycnt][block_xcnt][NUM_COLORS];

	for (size_t by = 0, y = 0; by < block_ycnt; by++, y += block_h)
		for (size_t bx = 0, x = 0; bx < block_xcnt; bx++, x += block_w)
		{
			size_t y_end = MIN(y + block_h, img.height);
			size_t x_end = MIN(x + block_w, img.width);
			cdf_lut_calc(luts[by][bx], img.pixels, y, y_end, x, x_end);
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

static void
process_image(struct Image img, struct Image texture, size_t block_size,
			  double alpha)
{
	texture_ahe(texture, block_size, block_size);
	double **errors = alloc_errors(img.height, img.width);

	for (size_t y = 0; y < img.height; y++)
		for (size_t x = 0; x < img.width; x++)
		{
			double e = errors[y][x];
			uint8_t p = img.pixels[y][x];
			uint8_t t = texture.pixels[y % texture.height][x % texture.width];

			img.pixels[y][x] = p + alpha * e <= t ? 0 : 255;

			int err = p - img.pixels[y][x];
			add(errors, x + 1, y + 0, img.width, img.height, err * 7 / 16.0);
			add(errors, x - 1, y + 1, img.width, img.height, err * 3 / 16.0);
			add(errors, x + 0, y + 1, img.width, img.height, err * 5 / 16.0);
			add(errors, x + 1, y + 1, img.width, img.height, err * 1 / 16.0);
		}

	free_errors(errors, img.height);
}

int
main(int argc, char * const argv[])
{
	if (argc != 6)
		error("usage: %s <input_file> <texture_file> <output_file> "
			  "<block_size> <alpha>", argv[0]);

	const char *input_filename   = argv[1];
	const char *texture_filename = argv[2];
	const char *output_filename  = argv[3];
	const char *block_size_str   = argv[4];
	const char *alpha_str        = argv[5];

	size_t block_size = strtoul(block_size_str, NULL, 10);

	double alpha = atof(alpha_str);
	if (alpha < 0.0 || alpha > 10.0)
		error("wrong alpha (%f)", alpha);

	struct Image img = read_grayscale_png(input_filename);
	printf("Input file \"%s\" opened (width = %u, height = %u)\n",
		   input_filename, img.width, img.height);

	struct Image texture = read_grayscale_png(texture_filename);
	printf("Texture file \"%s\" opened (width = %u, height = %u)\n",
		   texture_filename, texture.width, texture.height);

	process_image(img, texture, block_size, alpha);

	write_grayscale_png(img, output_filename);
	free_pixels(img);
	return EXIT_SUCCESS;
}
