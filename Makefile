CC = gcc
IDIR = /opt/local/include/libpng16/
LDIR = /opt/local/lib/
LIBS = -lc -lpng16
CFLAGS = -Wall -march=native -Ofast -Wa,-q
OUT_DIR = build
LABS = lab1_1 lab1_2 lab1_3 lab1_4 lab1_5 lab1_6 lab1_7 lab1_8 lab2_1 lab2_2 lab3 lab4_1 lab4_2 lab5 lab6 lab7
BINS = $(addprefix $(OUT_DIR)/,${LABS})
OBJS = $(addsuffix .o,${BINS})

.PHONY: all
all: $(OUT_DIR) ${BINS}

${BINS}: %: $(OUT_DIR)/png_wrapper.o $(OUT_DIR)/histogram.o %.o
	$(CC) -o $@ $^ -L $(LDIR) $(LIBS)

.SECONDEXPANSION:
${OBJS}: $(OUT_DIR)/%.o: %/$$*.c
	$(CC) -c -o $@ $< -I . -I $(IDIR) $(CFLAGS)

$(OUT_DIR)/png_wrapper.o: png_wrapper.c
	$(CC) -c -o $@ $< -I . -I $(IDIR) $(CFLAGS)

$(OUT_DIR)/histogram.o: histogram.c
	$(CC) -c -o $@ $< -I . -I $(IDIR) $(CFLAGS)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

.PHONY: clean
clean:
	rm -rf $(OUT_DIR)
