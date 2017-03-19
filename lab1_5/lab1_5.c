#include <stdlib.h>
#include <string.h>
#include "png_wrapper.h"

static void process_image(struct Image img)
{
	const unsigned char M22[2][2] = {
		{ 0, 2 },
		{ 3, 1 }
	};

	const int N = 4;
	unsigned char M44[N][N];
	for (int y = 0; y < N; y += 2)
		for (int x = 0; x < N; x += 2)
		{
			unsigned char m = M22[y / 2][x / 2];
			M44[y + 0][x + 0] = 4 * M22[0][0] + m;
			M44[y + 0][x + 1] = 4 * M22[0][1] + m;
			M44[y + 1][x + 0] = 4 * M22[1][0] + m;
			M44[y + 1][x + 1] = 4 * M22[1][1] + m;
		}

	unsigned char T[N][N];
	for (int y = 0; y < N; y++)
		for (int x = 0; x < N; x++)
			T[y][x] = 255 * (M44[y][x] + 0.5) / (N * N);

	for (int y = 0; y < img.height; y++)
		for (int x = 0; x < img.width; x++)
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
