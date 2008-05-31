#include <stdlib.h>
#include <string.h>
#include "png_wrapper.h"

static inline void add_sat(struct Image img, int x, int y, double a)
{
	if (x < 0 || y < 0 || x >= img.width || y >= img.height)
		return;

	int sum = img.pixels[y][x] + a + 0.5;

	if (sum < 0)
		sum = 0;
	else if (sum > 255)
		sum = 255;

	img.pixels[y][x] = sum;
}

static void process_image(struct Image img)
{
	for (int y = 0; y < img.height; y++)
		for (int x = 0; x < img.width; x++)
		{
			unsigned char old_val = img.pixels[y][x];
			unsigned char new_val = (old_val / 128) * 255;
			img.pixels[y][x] = new_val;
			int err = old_val - new_val;

			add_sat(img, x + 1, y + 0, err * 7 / 16.0);
			add_sat(img, x - 1, y + 1, err * 3 / 16.0);
			add_sat(img, x + 0, y + 1, err * 5 / 16.0);
			add_sat(img, x + 1, y + 1, err * 1 / 16.0);
		}
}

int main(int argc, char * const argv[])
{
	if (argc < 2 || argc > 3)
		error("usage: %s <input_file> [<output_file>]", argv[0]);

	char *input_filename = argv[1];
	char *output_filename;

	if (argc == 3)
		output_filename = argv[2];
	else
	{
		output_filename = alloca(strlen(input_filename) + sizeof("_out.png"));
		strcpy(output_filename, input_filename);
		strcat(output_filename, "_out.png");
	}

	struct Image img = read_grayscale_png(input_filename);
	printf("Input file \"%s\" opened (width = %u, height = %u)\n",
		   input_filename, img.width, img.height);

	process_image(img);

	write_grayscale_png(img, output_filename);
	free_pixels(img);

	return 0;
}
