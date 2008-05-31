#include <stdlib.h>
#include <stdarg.h>
#include "png_wrapper.h"

void error(const char *msg, ...)
{
	fprintf(stderr, "Error: ");
	va_list argptr;
	va_start(argptr, msg);
	vfprintf(stderr, msg, argptr);
	va_end(argptr);
	fprintf(stderr, "\n");
	exit(1);
}

void alloc_pixels(struct Image *img)
{
	img->pixels = malloc(img->height * sizeof(png_bytep));
	if (!img->pixels)
		error("can not allocate memory");

	for (size_t i = 0; i < img->height; i++)
	{
		img->pixels[i] = malloc(img->width);
		if (img->pixels[i] == NULL)
			error("can not allocate memory");
	}
}

void free_pixels(struct Image img)
{
	for (size_t i = 0; i < img.height; i++)
		free(img.pixels[i]);

	free(img.pixels);
}

struct Image read_grayscale_png(const char *file_name)
{
	FILE *fp = fopen(file_name, "rb");
	if (!fp)
		error("can not open input file: %s", file_name);

	const size_t header_size = 8;
	png_byte header[header_size];
	if (fread(header, 1, header_size, fp) != header_size)
		error("can not read png header");

	if (!png_check_sig(header, header_size))
		error("wrong png signature");

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
												 NULL, NULL, NULL);
	if (!png_ptr)
		error("can not create png read struct");

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
	    png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
		error("can not create png info struct");
	}

	if (setjmp(png_jmpbuf(png_ptr)))
	{
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);
		error("can not read png file");
	}

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, header_size);
	png_read_info(png_ptr, info_ptr);

	uint32_t width, height;
	int bit_depth, color_type;
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);
	if (bit_depth != 8 || color_type != PNG_COLOR_TYPE_GRAY)
		error("only 8-bit grayscale images are supported");

	struct Image result = { width, height, NULL };
	alloc_pixels(&result);

	png_read_image(png_ptr, result.pixels);

	png_read_end(png_ptr, NULL);
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	fclose(fp);

	return result;
}

void write_grayscale_png(struct Image img, const char *file_name)
{
	FILE *fp = fopen(file_name, "wb");
	if (!fp)
		error("can not open output file: %s", file_name);

	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
												  NULL, NULL, NULL);
	if (!png_ptr)
		error("can not create png write struct");

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
	    png_destroy_write_struct(&png_ptr, (png_infopp) NULL);
		error("can not create png info struct");
	}

	if (setjmp(png_jmpbuf(png_ptr)))
	{
        png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(fp);
		error("can not write png file");
	}

	png_init_io(png_ptr, fp);

	const int bit_depth = 8;
	png_set_IHDR(png_ptr, info_ptr, img.width, img.height, bit_depth,
				 PNG_COLOR_TYPE_GRAY, PNG_INTERLACE_NONE,
				 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	png_write_info(png_ptr, info_ptr);
	png_set_packing(png_ptr);

	png_write_image(png_ptr, img.pixels);

	png_write_end(png_ptr, info_ptr);
	png_destroy_write_struct(&png_ptr, &info_ptr);
	fclose(fp);
}
