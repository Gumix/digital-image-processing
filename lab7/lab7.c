#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "png_wrapper.h"

struct Point {
	uint32_t x, y;
};

static void binarization(struct Image img, unsigned char threshold)
{
	for (size_t y = 0; y < img.height; y++)
		for (size_t x = 0; x < img.width; x++)
			img.pixels[y][x] = img.pixels[y][x] >= threshold ? 255 : 0;
}

static bool find_white_pixel(struct Image img, struct Point *pixel)
{
	for (size_t y = 0; y < img.height; y++)
		for (size_t x = 0; x < img.width; x++)
			if (img.pixels[y][x])
			{
				pixel->x = x;
				pixel->y = y;
				return true;
			}

	return false;
}

static size_t segmentation(struct Image in, struct Image out)
{
	size_t count = 0;
	unsigned char prev_color = 0;

	struct Point pixel;
	while (find_white_pixel(in, &pixel))
	{
		unsigned char new_color = prev_color + 50 <= 255 ? prev_color + 50 : 50;

		struct Image front = { in.width, in.height };
		alloc_pixels(&front);

		for (size_t y = 0; y < front.height; y++)
			for (size_t x = 0; x < front.width; x++)
				front.pixels[y][x] = 0;
		front.pixels[pixel.y][pixel.x] = 1;

		while (true)
		{
			struct Point pix;
			if (!find_white_pixel(front, &pix))
				break;

			front.pixels[pix.y][pix.x] = 0;

			if (in.pixels[pix.y][pix.x])
			{
				in.pixels[pix.y][pix.x] = 0;
				out.pixels[pix.y][pix.x] = new_color;

				if (pix.y > 0
					&& in.pixels[pix.y - 1][pix.x    ])
					front.pixels[pix.y - 1][pix.x    ] = 1;
				if (pix.x > 0
					&& in.pixels[pix.y    ][pix.x - 1])
					front.pixels[pix.y    ][pix.x - 1] = 1;
				if (pix.y < in.height - 1
					&& in.pixels[pix.y + 1][pix.x    ])
					front.pixels[pix.y + 1][pix.x    ] = 1;
				if (pix.x < in.width - 1
					&& in.pixels[pix.y    ][pix.x + 1])
					front.pixels[pix.y    ][pix.x + 1] = 1;
			}
		}

		prev_color = new_color;
		count++;
		free_pixels(front);
	}

	return count;
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

	struct Image in = read_grayscale_png(input_filename);
	printf("Input file \"%s\" opened (width = %u, height = %u)\n",
		   input_filename, in.width, in.height);

	binarization(in, (unsigned char) threshold);

	struct Image out = { in.width, in.height };
	alloc_pixels(&out);

	size_t s = segmentation(in, out);
	free_pixels(in);
	printf("%lu segments\n", s);

	write_grayscale_png(out, output_filename);
	free_pixels(out);

	return 0;
}
