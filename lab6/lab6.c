#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "png_wrapper.h"

enum { MAX_AP_SIZE = 16 };

struct Aperture {
	uint32_t size;
	bool a[MAX_AP_SIZE][MAX_AP_SIZE];
};

static int cmp(const void *a, const void *b)
{
	return *(unsigned char *) a - *(unsigned char *) b;
}

static void process_image(struct Image in, struct Aperture aperture,
						  struct Image out)
{
	for (size_t y = 0; y < out.height; y++)
		for (size_t x = 0; x < out.width; x++)
		{
			size_t max_size = aperture.size * aperture.size;
			size_t size = 0;
			unsigned char pixels[max_size];

			for (size_t j = 0; j < aperture.size; j++)
				for (size_t i = 0; i < aperture.size; i++)
					if (aperture.a[j][i])
						pixels[size++] = in.pixels[y + j][x + i];

			qsort(pixels, size, sizeof(pixels[0]), cmp);

			size_t median = size / 2;
			out.pixels[y][x] = pixels[median];
		}
}

struct Aperture read_aperture(const char *file_name)
{
	FILE *fp = fopen(file_name, "r");
	if (!fp)
		error("can not open aperture file: %s", file_name);

	struct Aperture res;
	if (fscanf(fp, "%u", &res.size) != 1)
		error("can not read aperture size");

	for (size_t y = 0; y < res.size; y++)
		for (size_t x = 0; x < res.size; x++)
		{
			int element;
			if (fscanf(fp, "%d", &element) != 1)
				error("can not read aperture element");
			res.a[y][x] = element;
		}

	fclose(fp);
	return res;
}

int main(int argc, char * const argv[])
{
	if (argc < 3 || argc > 4)
		error("usage: %s <input_file> <aperture_file> [<output_file>]", argv[0]);

	char *input_filename  = argv[1];
	char *aperture_filename = argv[2];
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

	struct Aperture aperture = read_aperture(aperture_filename);
	printf("Aperture file \"%s\" opened (size = %ux%u)\n",
		   aperture_filename, aperture.size, aperture.size);

	struct Image out = { in.width  - aperture.size + 1,
						 in.height - aperture.size + 1, NULL };
	alloc_pixels(&out);

	process_image(in, aperture, out);
	free_pixels(in);

	write_grayscale_png(out, output_filename);
	free_pixels(out);

	return 0;
}
