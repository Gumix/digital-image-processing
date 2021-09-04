#include <stdlib.h>
#include "png_wrapper.h"
#include "histogram.h"

static void process_image(struct Image img)
{
	size_t hist[MAX_COLOR + 1] = { };
	histogram_calc(hist, img.pixels, 0, img.height, 0, img.width);

	// Cumulative distribution function
	double cdf[MAX_COLOR + 1];
	size_t total_pixels = img.height * img.width;
	cdf[0] = (double) hist[0] / total_pixels;

	for (size_t i = 1; i <= MAX_COLOR; i++)
		cdf[i] = cdf[i - 1] + (double) hist[i] / total_pixels;

	uint8_t lut[MAX_COLOR + 1];
	for (size_t i = 0; i <= MAX_COLOR; i++)
		lut[i] = MIN(cdf[i] * MAX_COLOR, 255);

	for (size_t y = 0; y < img.height; y++)
		for (size_t x = 0; x < img.width; x++)
			img.pixels[y][x] = lut[img.pixels[y][x]];
}

int main(int argc, char * const argv[])
{
	if (argc != 5)
		error("usage: %s <in_file> <out_file> <hist1_out_file> "
			  "<hist2_out_file>", argv[0]);

	char *input_filename  = argv[1];
	char *output_filename = argv[2];
	char *hist1_filename  = argv[3];
	char *hist2_filename  = argv[4];

	struct Image img = read_grayscale_png(input_filename);
	printf("Input file \"%s\" opened (width = %u, height = %u)\n",
		   input_filename, img.width, img.height);

	struct Image hist = { HIST_WIDTH, HIST_HEIGHT, NULL };
	alloc_pixels(&hist);
	histogram_draw(img, hist);
	write_grayscale_png(hist, hist1_filename);

	process_image(img);
	write_grayscale_png(img, output_filename);

	histogram_draw(img, hist);
	free_pixels(img);
	write_grayscale_png(hist, hist2_filename);
	free_pixels(hist);

	return 0;
}
