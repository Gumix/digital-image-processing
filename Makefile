IDIR = /opt/local/include/libpng16/
LDIR = /opt/local/lib/
LIBS = -lc -lpng16
CFLAGS = -Wall -Wextra -Werror -O0 -g
OUT_DIR = build
LABS = lab1_threshold lab1_bitplane lab1_random lab1_bayer lab1_cluster		\
	   lab1_floyd lab1_jarvis lab1_atkinson	lab2_nearest lab2_bilinear lab3	\
	   lab4_gamma lab4_contrast lab4_equalize lab5 lab6 lab7
BINS = $(addprefix $(OUT_DIR)/,${LABS})
OBJS = $(addsuffix .o,${BINS})

.PHONY: all
all: $(OUT_DIR) ${BINS}

${BINS}: %: $(OUT_DIR)/png_wrapper.o $(OUT_DIR)/histogram.o %.o
	$(CC) -o $@ $^ -L $(LDIR) $(LIBS)

.SECONDEXPANSION:
${OBJS}: $(OUT_DIR)/%.o: %/$$*.c
	$(CC) -c -o $@ $< -I utils -I $(IDIR) $(CFLAGS)

$(OUT_DIR)/png_wrapper.o: utils/png_wrapper.c
	$(CC) -c -o $@ $< -I utils -I $(IDIR) $(CFLAGS)

$(OUT_DIR)/histogram.o: utils/histogram.c
	$(CC) -c -o $@ $< -I utils -I $(IDIR) $(CFLAGS)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

.PHONY: clean
clean:
	rm -rf $(OUT_DIR)
