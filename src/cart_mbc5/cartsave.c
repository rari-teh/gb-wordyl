#include <gbdk/platform.h>
#include <stdint.h>
#include <stdbool.h>

#include "../stats.h"

// Map a secondary stats struct to the beginning of SRAM (0xA000) when using MBC5
stats_rec __at (0xA000) sram_stats;



