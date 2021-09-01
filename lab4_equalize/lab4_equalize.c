#include <stdlib.h>
#include "png_wrapper.h"
#include "histogram.h"

static void process_image(struct Image img)
{
	uint8_t lut[NUM_COLORS];
	cdf_lut_calc(lut, img.pixels, 0, img.height, 0, img.width);

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
