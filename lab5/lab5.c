#include <stdlib.h>
#include <string.h>
#include "png_wrapper.h"

enum { MAX_KERN_SIZE = 16 };

struct Kernel {
	uint32_t size;
	double k[MAX_KERN_SIZE][MAX_KERN_SIZE];
};

static void process_image(struct Image in, struct Kernel kernel, struct Image out)
{
	for (size_t y = 0; y < out.height; y++)
		for (size_t x = 0; x < out.width; x++)
		{
			double pixel = 0.5;

			for (size_t j = 0; j < kernel.size; j++)
				for (size_t i = 0; i < kernel.size; i++)
				{
					double p = in.pixels[y + j][x + i];
					double k = kernel.k[j][i];
					pixel += p * k;
				}

			int res = pixel;
			if (res < 0) res = 0;
			else if (res > 255) res = 255;
			out.pixels[y][x] = res;
		}
}

struct Kernel read_kernel(const char *file_name)
{
	FILE *fp = fopen(file_name, "r");
	if (!fp)
		error("can not open kernel file: %s", file_name);

	struct Kernel res;
	if (fscanf(fp, "%u", &res.size) != 1)
		error("can not read kernel size");

	uint32_t scale;
	if (fscanf(fp, "%u", &scale) != 1)
		error("can not read kernel scale");

	for (size_t y = 0; y < res.size; y++)
		for (size_t x = 0; x < res.size; x++)
		{
			int element;
			if (fscanf(fp, "%d", &element) != 1)
				error("can not read kernel element");
			res.k[y][x] = element / (double) scale;
		}

	fclose(fp);
	return res;
}

int main(int argc, char * const argv[])
{
	if (argc < 3 || argc > 4)
		error("usage: %s <input_file> <kernel_file> [<output_file>]", argv[0]);

	char *input_filename  = argv[1];
	char *kernel_filename = argv[2];
	char *output_filename;

	if (argc == 4)
		output_filename = argv[3];
	else
	{
		output_filename = alloca(strlen(input_filename) + sizeof("_out.png"));
		strcpy(output_filename, input_filename);
		strcat(output_filename, "_out.png");
	}

	struct Image in = read_grayscale_png(input_filename);
	printf("Input file \"%s\" opened (width = %u, height = %u)\n",
		   input_filename, in.width, in.height);

	struct Kernel kernel = read_kernel(kernel_filename);
	printf("Kernel file \"%s\" opened (size = %ux%u)\n",
		   kernel_filename, kernel.size, kernel.size);

	struct Image out = { in.width  - kernel.size + 1,
						 in.height - kernel.size + 1, NULL };
	alloc_pixels(&out);

	process_image(in, kernel, out);
	free_pixels(in);

	write_grayscale_png(out, output_filename);
	free_pixels(out);

	return 0;
}
