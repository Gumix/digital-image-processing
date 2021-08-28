#include <stdlib.h>
#include "png_wrapper.h"

static void process_image(struct Image in, struct Image out[8])
{
	for (size_t y = 0; y < in.height; y++)
		for (size_t x = 0; x < in.width; x++)
			for (int b = 0; b < 8; b++)
				out[b].pixels[y][x] = in.pixels[y][x] & 1 << b ? 255 : 0;
}

int main(int argc, char * const argv[])
{
	if (argc != 10)
		error("usage: %s <in_file> <out0_file> <out1_file> <out2_file> <out3_file> "
			  "<out4_file> <out5_file> <out6_file> <out7_file> ", argv[0]);

	char *input_filename = argv[1];
	char *output_filenames[8];
	for (int i = 0; i < 8; i++)
		output_filenames[i] = argv[i + 2];

	struct Image in = read_grayscale_png(input_filename);
	printf("Input file \"%s\" opened (width = %u, height = %u)\n",
		   input_filename, in.width, in.height);

	struct Image out[8];
	for (int i = 0; i < 8; i++)
	{
		out[i].width  = in.width;
		out[i].height = in.height;
		alloc_pixels(&out[i]);
	}

	process_image(in, out);
	free_pixels(in);

	for (int i = 0; i < 8; i++)
	{
		write_grayscale_png(out[i], output_filenames[i]);
		free_pixels(out[i]);
	}

	return 0;
}
