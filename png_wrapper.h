#ifndef PNG_WRAPPER_H
#define PNG_WRAPPER_H

#include <png.h>

struct Image {
	uint32_t width, height;
	unsigned char **pixels;
};

void error(const char *msg, ...) __attribute__((format(printf, 1, 2)));

void alloc_pixels(struct Image *img);
void free_pixels(struct Image img);

struct Image read_grayscale_png(const char *file_name);
void write_grayscale_png(struct Image img, const char *file_name);

#endif
