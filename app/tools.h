#ifndef _TOOLS_H
#define _TOOLS_H

#include <bc_common.h>

size_t compress(uint8_t *in, size_t in_size, uint8_t *buffer, size_t buffer_size);

size_t decompress(uint8_t *out, size_t out_size, uint8_t *buffer, size_t buffer_size);

#endif // _TOOLS_H



