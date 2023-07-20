#pragma once
#include "checkML.h"
#include <cstdint>
#define CHANGEENDIAN(val) (val << 24) |((val << 8) & 0x00ff0000) |((val >> 8) & 0x0000ff00) |((val >> 24) & 0x000000ff); // Cambio de endian ya sea de big a little o viceversa

// Definir el tipo de endian según SDL
#if SDL_BYTEORDER == SDL_LIL_ENDIAN 
#define ENDIANNESS_IS_LE
#else 
#define ENDIANNESS_IS_BE
#endif 

class EndianessPC {};