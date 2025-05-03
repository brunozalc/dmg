# ---------- toolchain ----------
CC ?= clang
CSTD = -std=c18
WARNINGS = -Wall -Wextra -Wshadow -pedantic
OPT = -O3
CFLAGS = $(CSTD) $(WARNINGS) $(OPT) $(shell pkg-config --cflags raylib) -Isrc -Iinclude
LDFLAGS = $(shell pkg-config --libs raylib) \
			-framework CoreVideo -framework IOKit -framework Cocoa \
			-framework OpenGL -framework GLUT 	# for macOS

# ---------- file lists ----------
SRC_DIRS := src lib
SRC := $(foreach d,$(SRC_DIRS),$(wildcard $(d)/*.c))
OBJ := $(patsubst %.c,build/%.o,$(SRC))
BIN := gb

# ---------- build rules ----------
.PHONY: all clean run
.DEFAULT_GOAL := run

all: $(BIN)

$(BIN): $(OBJ)
	@echo "LD  $@"
	@$(CC) $^ $(LDFLAGS) -o $@

# pattern rule: each .c → build/<same‑path>.o
build/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "CC  $<"
	@$(CC) $(CFLAGS) -c $< -o $@

run: $(BIN)
	./$(BIN)

clean:
	rm -rf build $(BIN)
