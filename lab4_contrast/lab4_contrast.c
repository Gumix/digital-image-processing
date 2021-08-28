#include <stdlib.h>
#include "png_wrapper.h"
#include "histogram.h"

void precalculate(unsigned char *lut, size_t s1, size_t s2)
{
	for (size_t c = 0; c <= s1; c++)
		lut[c] = 0;

	for (size_t c = s1 + 1; c < s2; c++)
		lut[c] = (c - s1) * (double) MAX_COLOR / (s2 - s1) + 0.5;

	for (size_t c = s2; c <= MAX_COLOR; c++)
		lut[c] = MAX_COLOR;
}

static void process_image(struct Image in, struct Image out, size_t delta)
{
	size_t pixels_count[MAX_COLOR + 1] = { };

	for (size_t y = 0; y < in.height; y++)
		for (size_t x = 0; x < in.width; x++)
			pixels_count[in.pixels[y][x]]++;

	size_t s1, s2;

	for (s1 = 0; (pixels_count[s1] <= delta) && (s1 <= MAX_COLOR); s1++);

	for (s2 = MAX_COLOR; (pixels_count[s2] <= delta) && (s2 >= 0); s2--);

	unsigned char lut[MAX_COLOR + 1];
	precalculate(lut, s1, s2);

	for (size_t y = 0; y < out.height; y++)
		for (size_t x = 0; x < out.width; x++)
			out.pixels[y][x] = lut[in.pixels[y][x]];
}

int main(int argc, char * const argv[])
{
	if (argc != 6)
		error("usage: %s <in_file> <out_file> <hist1_out_file> "
			  "<hist2_out_file> <delta (0-1000)>", argv[0]);

	char *input_filename  = argv[1];
	char *output_filename = argv[2];
	char *hist1_filename  = argv[3];
	char *hist2_filename  = argv[4];
	char *s_delta = argv[5];

	int delta = atoi(s_delta);
	if (delta < 0 || delta > 1000)
		error("wrong delta (%d)", delta);

	struct Image in = read_grayscale_png(input_filename);
	printf("Input file \"%s\" opened (width = %u, height = %u)\n",
		   input_filename, in.width, in.height);

	struct Image hist1 = { HIST_WIDTH, HIST_HEIGHT, NULL };
	alloc_pixels(&hist1);
	histogram(in, hist1);
	write_grayscale_png(hist1, hist1_filename);
	free_pixels(hist1);

	struct Image out = { in.width, in.height, NULL };
	alloc_pixels(&out);
	process_image(in, out, delta);
	free_pixels(in);
	write_grayscale_png(out, output_filename);

	struct Image hist2 = { HIST_WIDTH, HIST_HEIGHT, NULL };
	alloc_pixels(&hist2);
	histogram(out, hist2);
	free_pixels(out);
	write_grayscale_png(hist2, hist2_filename);
	free_pixels(hist2);

	return 0;
}
