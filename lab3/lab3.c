#include <stdlib.h>
#include <string.h>
#include "png_wrapper.h"

static void process_images(struct Image top, struct Image bot,
						   struct Image alpha, struct Image out)
{
	for (int y = 0; y < out.height; y++)
		for (int x = 0; x < out.width; x++)
		{
			double a = alpha.pixels[y][x] / 255.0;

			int res = top.pixels[y][x] * (1 - a)
					+ bot.pixels[y][x] * a + 0.5;

			if (res > 255) res = 255;
			out.pixels[y][x] = res;
		}
}

int main(int argc, char * const argv[])
{
	if (argc < 4 || argc > 5)
		error("usage: %s <top_file> <bottom_file> "
			  "<alpha_channel_file> [<output_file>]", argv[0]);

	char *input1_filename = argv[1];
	char *input2_filename = argv[2];
	char *input3_filename = argv[3];
	char *output_filename;

	if (argc == 5)
		output_filename = argv[4];
	else
	{
		output_filename = alloca(strlen(input1_filename) + sizeof("_out.png"));
		strcpy(output_filename, input1_filename);
		strcat(output_filename, "_out.png");
	}

	struct Image in1 = read_grayscale_png(input1_filename);
	printf("Top input file \"%s\" opened (width = %u, height = %u)\n",
		   input1_filename, in1.width, in1.height);

	struct Image in2 = read_grayscale_png(input2_filename);
	printf("Bottom input file \"%s\" opened (width = %u, height = %u)\n",
		   input2_filename, in2.width, in2.height);

	struct Image in3 = read_grayscale_png(input3_filename);
	printf("Alpha channel input file \"%s\" opened (width = %u, height = %u)\n",
		   input3_filename, in3.width, in3.height);

	struct Image out = { in1.width, in2.height };
	alloc_pixels(&out);

	process_images(in1, in2, in3, out);
	free_pixels(in1);
	free_pixels(in2);
	free_pixels(in3);

	write_grayscale_png(out, output_filename);
	free_pixels(out);

	return 0;
}
