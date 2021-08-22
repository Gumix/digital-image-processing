#pragma once

#include <stdlib.h>
#include "png_wrapper.h"

enum { HIST_WIDTH = 512, HIST_HEIGHT = 256, MAX_COLOR = 255 };

void histogram(struct Image in, struct Image out);
