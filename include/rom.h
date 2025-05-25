#ifndef ROM_HEADER
#define ROM_HEADER

#include <stdint.h>

#include "mmu.h"

void log_header(MMU *mmu);
void load_boot_rom(MMU *mmu, const char *filepath);
void load_rom(MMU *mmu, const char *filepath);

#endif
