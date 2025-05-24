# toolchain
CC ?= clang
CSTD = -std=c18
WARNINGS = -Wall -Wextra -Wshadow -pedantic
OPT = -O3
DEBUG_FLAGS = -g -O0 -DDEBUG
ASAN_FLAGS = -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer
ASAN_LDFLAGS = -fsanitize=address -fsanitize=undefined

# flags
CFLAGS = $(CSTD) $(WARNINGS) $(OPT) $(shell pkg-config --cflags raylib) -Iinclude -Isrc
CFLAGS_DEBUG = $(CSTD) $(WARNINGS) $(DEBUG_FLAGS) $(shell pkg-config --cflags raylib) -Iinclude -Isrc
CFLAGS_ASAN = $(CFLAGS_DEBUG) $(ASAN_FLAGS)
LDFLAGS = $(shell pkg-config --libs raylib) \
		  -framework CoreVideo -framework IOKit -framework Cocoa \
		  -framework OpenGL -framework GLUT

# files
SRC_DIRS := src lib
SRC := $(foreach d,$(SRC_DIRS),$(wildcard $(d)/*.c))
OBJ := $(patsubst %.c,build/%.o,$(SRC))
ASM := $(patsubst %.c,build/%.s,$(SRC))

# binaries
BIN := gb
BIN_DEBUG := $(BIN)-debug
BIN_ASAN := $(BIN)-asan

# targets
.PHONY: all debug asan asm clean

all: $(BIN)

# default build
$(BIN): $(OBJ)
	@echo "LD  $@"
	@$(CC) $^ $(LDFLAGS) -o $@

# debug build
$(BIN_DEBUG): CFLAGS := $(CFLAGS_DEBUG)
$(BIN_DEBUG): $(OBJ)
	@echo "LD (debug) $@"
	@$(CC) $^ $(LDFLAGS) -o $@

# asan build
$(BIN_ASAN): CFLAGS := $(CFLAGS_ASAN)
$(BIN_ASAN): LDFLAGS += $(ASAN_LDFLAGS)
$(BIN_ASAN): $(OBJ)
	@echo "LD (asan) $@"
	@$(CC) $^ $(LDFLAGS) -o $@

# object files
build/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "CC  $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# assembly files
build/%.s: %.c
	@mkdir -p $(dir $@)
	@echo "ASM $<"
	@$(CC) $(CFLAGS) -S -g -fverbose-asm -o $@ $<

# convenience targets
debug: $(BIN_DEBUG)
asan: $(BIN_ASAN)
asm: $(ASM)

clean:
	rm -rf build $(BIN) $(BIN_DEBUG) $(BIN_ASAN)
