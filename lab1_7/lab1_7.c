#include <string.h>
#include <stdlib.h>
#include "png_wrapper.h"

static void process_image(struct Image img)
{
	enum { N = 8 };
	const unsigned char M[N][N] = {
		{ 24, 10, 12, 26, 35, 47, 49, 37 },
		{  8,  0,  2, 14, 45, 59, 61, 51 },
		{ 22,  6,  4, 16, 43, 57, 63, 53 },
		{ 30, 20, 18, 28, 33, 41, 55, 39 },
		{ 34, 46, 48, 36, 25, 11, 13, 27 },
		{ 44, 58, 60, 50,  9,  1,  3, 15 },
		{ 42, 56, 62, 52, 23,  7,  5, 17 },
		{ 32, 40, 54, 38, 31, 21, 19, 29 }
	};

	unsigned char T[N][N];
	for (int y = 0; y < N; y++)
		for (int x = 0; x < N; x++)
			T[y][x] = 255 * (M[y][x] + 0.5) / (N * N);

	for (size_t y = 0; y < img.height; y++)
		for (size_t x = 0; x < img.width; x++)
			img.pixels[y][x] = img.pixels[y][x] > T[y % N][x % N] ? 255 : 0;
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
