#include <stdlib.h>
#include <string.h>
#include "png_wrapper.h"

static void process_image(struct Image img, unsigned char threshold)
{
	for (size_t y = 0; y < img.height; y++)
		for (size_t x = 0; x < img.width; x++)
			img.pixels[y][x] = img.pixels[y][x] >= threshold ? 255 : 0;
}

int main(int argc, char * const argv[])
{
	if (argc < 3 || argc > 4)
		error("usage: %s <input_file> [<output_file>] <threshold (0-255)>",
			  argv[0]);

	char *input_filename = argv[1];
	char *output_filename;
	char *s_threshold;

	if (argc == 4)
	{
		output_filename = argv[2];
		s_threshold = argv[3];
	}
	else
	{
		output_filename = alloca(strlen(input_filename) + sizeof("_out.png"));
		strcpy(output_filename, input_filename);
		strcat(output_filename, "_out.png");
		s_threshold = argv[2];
	}

	int threshold = atoi(s_threshold);
	if (threshold < 0 || threshold > 255)
		error("wrong threshold (%d)", threshold);

	struct Image img = read_grayscale_png(input_filename);
	printf("Input file \"%s\" opened (width = %u, height = %u)\n",
		   input_filename, img.width, img.height);

	process_image(img, (unsigned char) threshold);

	write_grayscale_png(img, output_filename);
	free_pixels(img);

	return 0;
}
