#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "png_wrapper.h"
#include "histogram.h"

static void process_image(struct Image img, double c, double gamma)
{
	for (size_t y = 0; y < img.height; y++)
		for (size_t x = 0; x < img.width; x++)
		{
			double in = img.pixels[y][x] / 255.0;
			int out = c * pow(in, gamma) * 255.0 + 0.5;
			if (out > 255) out = 255;
			img.pixels[y][x] = out;
		}
}

int main(int argc, char * const argv[])
{
	if (argc != 7)
		error("usage: %s <in_file> <out_file> <hist1_out_file> "
			  "<hist2_out_file> <c (0.0-50.0)> <gamma (0.0-50.0)>", argv[0]);

	char *input_filename  = argv[1];
	char *output_filename = argv[2];
	char *hist1_filename  = argv[3];
	char *hist2_filename  = argv[4];
	char *s_c             = argv[5];
	char *s_gamma         = argv[6];

	double c = atof(s_c);
	if (c < 0.0 || c > 50.0)
		error("wrong c (%f)", c);

	double gamma = atof(s_gamma);
	if (gamma < 0.0 || gamma > 50.0)
		error("wrong gamma (%f)", gamma);

	struct Image img = read_grayscale_png(input_filename);
	printf("Input file \"%s\" opened (width = %u, height = %u)\n",
		   input_filename, img.width, img.height);

	struct Image hist1 = { HIST_WIDTH, HIST_HEIGHT, NULL };
	alloc_pixels(&hist1);
	histogram(img, hist1);
	write_grayscale_png(hist1, hist1_filename);
	free_pixels(hist1);

	process_image(img, c, gamma);

	struct Image hist2 = { HIST_WIDTH, HIST_HEIGHT, NULL };
	alloc_pixels(&hist2);
	histogram(img, hist2);
	write_grayscale_png(hist2, hist2_filename);
	free_pixels(hist2);

	write_grayscale_png(img, output_filename);
	free_pixels(img);

	return 0;
}
