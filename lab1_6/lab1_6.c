#include <stdlib.h>
#include <string.h>
#include "png_wrapper.h"

static int rnd(int a)
{
	int min = -a;
	int max =  a;
	double r = rand() / (RAND_MAX + 1.0);
	return r * (max - min + 1) + min;
}

static void process_image(struct Image img,
						  unsigned char threshold, unsigned char noise)
{
	for (int y = 0; y < img.height; y++)
		for (int x = 0; x < img.width; x++)
		{
			int new = img.pixels[y][x] + rnd(noise);
			img.pixels[y][x] = new >= threshold ? 255 : 0;
		}
}

int main(int argc, char * const argv[])
{
	if (argc < 4 || argc > 5)
		error("usage: %s <input_file> [<output_file>] <threshold (0-255)> <noise (0-255)>",
			  argv[0]);

	char *input_filename = argv[1];
	char *output_filename;
	char *s_threshold;
	char *s_noise;

	if (argc == 5)
	{
		output_filename = argv[2];
		s_threshold = argv[3];
		s_noise = argv[4];
	}
	else
	{
		output_filename = alloca(strlen(input_filename) + sizeof("_out.png"));
		strcpy(output_filename, input_filename);
		strcat(output_filename, "_out.png");
		s_threshold = argv[2];
		s_noise = argv[3];
	}

	int threshold = atoi(s_threshold);
	if (threshold < 0 || threshold > 255)
		error("wrong threshold (%d)", threshold);

	int noise = atoi(s_noise);
	if (noise < 0 || noise > 255)
		error("wrong additive noise level (%d)", noise);

	struct Image img = read_grayscale_png(input_filename);
	printf("Input file \"%s\" opened (width = %u, height = %u)\n",
		   input_filename, img.width, img.height);

	srand(time(NULL));

	process_image(img, threshold, noise);

	write_grayscale_png(img, output_filename);
	free_pixels(img);

	return 0;
}
