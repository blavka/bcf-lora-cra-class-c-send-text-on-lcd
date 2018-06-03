#include <tools.h>

size_t compress(uint8_t *in, size_t in_size, uint8_t *buffer, size_t buffer_size)
{
    memset(buffer, 0x00, buffer_size);

    uint32_t bit = 0;
    uint8_t val;
    size_t b = 0;
    uint32_t offset;

    for(size_t i = 0; i < in_size; i++)
    {
        val = in[i];

        b = bit / 8;

        if (b + 1 > buffer_size)
        {
            return 0;
        }

        offset = bit % 8;

        if(offset > 2)
        {
            buffer[b] |= val << offset;
            buffer[b+1] |= val >> (8-offset);
        }
        else
        {
            buffer[b] |= val << offset;
        }

        bit += 6;
    }

    return b + 1;
}

size_t decompress(uint8_t *out, size_t out_size, uint8_t *buffer, size_t buffer_size)
{
    uint32_t bit = 0;
    size_t b = 0;
    uint8_t val;
    uint32_t offset;

    for(size_t i = 0; i < out_size; i++)
    {
        b = bit / 8;

        offset = bit % 8;

        if (b + 1 > buffer_size)
        {
            return 0;
        }

        if (offset > 2)
        {
            val = (buffer[b] >> offset) | (buffer[b + 1] << (8 - offset));
        }
        else
        {
            val = buffer[b] >> offset;
        }

        out[i] = val & 0x3F;

        bit += 6;
    }

    return b;
}
