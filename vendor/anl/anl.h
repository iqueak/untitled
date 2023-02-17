#ifndef ANL_H
#define ANL_H

// Define ANL_IMPLEMENTATION in exactly 1 source (.cpp) file before including this header.

#include "include/VM/coordinate.h"
#include "include/VM/instruction.h"
#include "include/VM/hashing.h"
#include "include/VM/utility.h"
#include "include/VM/noise_gen.h"
#include "include/VM/kernel.h"
#include "include/VM/vm.h"
#include "include/VM/random_gen.h"

#include "include/Expression/parsing.h"
#include "include/Expression/expressionbuilder.h"

#include "include/Imaging/imaging.h"

#include "include/Processing/erosion.h"

#ifdef ANL_IMPLEMENTATION
#include "include/VM/noise_lut.inl"
#include "include/VM/noise_gen.inl"
#include "include/VM/coordinate.inl"
#include "include/VM/hashing.inl"
#include "include/VM/kernel.inl"
#include "include/VM/vm.inl"

#include "include/Expression/parsing.inl"
#include "include/Expression/expressionbuilder.inl"

#include "include/Imaging/imaging.inl"

#ifdef IMPLEMENT_STB
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/Imaging/stb_image.h"
#include "include/Imaging/stb_image_write.h"
#undef STB_IMAGE_IMPLEMENTATION
#undef STB_IMAGE_WRITE_IMPLEMENTATION
#endif

#include "include/Processing/erosion.inl"
#endif

#endif
