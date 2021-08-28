#include <stdlib.h>
#include <string.h>
#include "png_wrapper.h"

static void process_image(struct Image in, struct Image out, double N)
{
	for (size_t y = 0; y < out.height; y++)
		for (size_t x = 0; x < out.width; x++)
		{
			size_t in_x = x / N + 0.5,
				   in_y = y / N + 0.5;

			if (in_x > in.width  - 1) in_x = in.width  - 1;
			if (in_y > in.height - 1) in_y = in.height - 1;
	
			out.pixels[y][x] = in.pixels[in_y][in_x];
		}
}

int main(int argc, char * const argv[])
{
	if (argc < 3 || argc > 4)
		error("usage: %s <input_file> [<output_file>] <N (0.0-10.0)>", argv[0]);

	char *input_filename = argv[1];
	char *output_filename;
	char *sN;

	if (argc == 4)
	{
		output_filename = argv[2];
		sN = argv[3];
	}
	else
	{
		output_filename = alloca(strlen(input_filename) + sizeof("_out.png"));
		strcpy(output_filename, input_filename);
		strcat(output_filename, "_out.png");
		sN = argv[2];
	}

	double N = atof(sN);
	if (N <= 0.0 || N > 10.0)
		error("N should be in the range 0.0-10.0");

	struct Image in = read_grayscale_png(input_filename);
	printf("Input file \"%s\" opened (width = %u, height = %u)\n",
		   input_filename, in.width, in.height);

	struct Image out = { in.width * N + 0.5, in.height * N + 0.5, NULL };
	alloc_pixels(&out);

	process_image(in, out, N);
	free_pixels(in);

	write_grayscale_png(out, output_filename);
	free_pixels(out);

	return 0;
}
