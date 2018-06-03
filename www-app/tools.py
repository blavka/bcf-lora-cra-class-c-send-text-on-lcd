#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import math

def hex_to_buffer(hex_string):
    buffer = [0] * math.ceil(len(hex_string) / 2)

    for i, x in enumerate(range(0, len(hex_string), 2)):
        buffer[i] = int(hex_string[x:x+2], 16)

    return buffer


def decompress(buffer):
    data = [0] * math.ceil(len(buffer) * 8 / 6)

    bit = 0
    for i in range(64):

        b = bit // 8
        offset = bit % 8
        # print(bit, b, offset)

        if offset > 2:
            val = buffer[b] >> offset | buffer[b+1] << (8-offset)
        else:
            val = buffer[b] >> offset

        data[i] = val & 0x3F

        bit += 6

    return data


def compress(data):
    buffer = [0] * math.ceil(len(data) * 6 / 8)

    bit = 0
    for val in data:

        b = bit // 8
        offset = bit % 8

        if offset > 2:
            buffer[b] |= (val << offset) & 0xff
            buffer[b+1] |= (val >> (8 - offset)) & 0xff
        else:
            buffer[b] |= (val << offset) & 0xff

        bit += 6

    return buffer


if __name__ == '__main__':

    hex_string = '877118c9821cc7611cc88220887228cab22048822c8ab2244ab22c8ab228ccb22ccab22c8bb22ccab228cbc230cbb22c000000'

    buffer = hex_to_buffer(hex_string)
    print(buffer)

    data = decompress(buffer)

    print(data)

    buffer_b = compress(data)

    print(buffer_b == buffer)
