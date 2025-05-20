#ifndef ROM_HEADER
#define ROM_HEADER

#include <stdint.h>

#include "mmu.h"

void log_header(MMU *mmu);
void load_rom(MMU *mmu, const char *filepath);

#endif
