#include "chunk.h"
#include <stdio.h>
#include <stdlib.h>

unsigned char* chunk_encode(
    char*** chunk, int width, int height, int depth,
    int* length) {
    // x = width, y = height, z = depth
    int x = 0, y = 0, z = 0, nr = 0, alocare = 0;
    // int memory = 600, grass = 64, wood = 128, stone = 192, first_char = 32;
    // int max_size = 4095, eight = 8, six = 6, five = 5;
    char block = chunk[0][0][0];
    unsigned char* encode = calloc(memory, sizeof(unsigned char));
    for (y = 0; y < height; y++) {
        for (z = 0; z < depth; z++) {
            for (x = 0; x < width; x++) {
                if (chunk[x][y][z] == block) {
                    nr++;
                } else {
                    // if (nr >= 32) {
                    //     alocare++;
                    // }
                    switch (block) {  // setez primii doi biti
                        case 0: encode[alocare] = 0;  // air
                        break;
                        case 1: encode[alocare] = grass;  // grass
                        break;
                        case 2: encode[alocare] = wood;  //  wood
                        break;
                        case 3: encode[alocare] = stone;  // stone
                    }
                       if (nr >= first_char) {
                        encode[alocare] = encode[alocare] | (1 << five);
                    }
                    while (nr >= max_size + 1) {  // 4096
                        encode[alocare+1] = max_size;
                        encode[alocare] = encode[alocare] | (max_size >> eight);
                        nr -= max_size;
                        alocare += 2;
                    }
                    if (nr >= first_char  && nr <= max_size) {
                        encode[alocare + 1] = nr;
                        encode[alocare] = encode[alocare] | (nr >> eight);
                        alocare += 2;
                    }
                    if (nr < first_char) {
                        encode[alocare] = encode[alocare] | nr;
                        alocare++;
                    }
                    block = chunk[x][y][z];
                    nr = 1;
                }
            }
        }
    }
    switch (block) {  // setez primii doi biti
        case 0: encode[alocare] = 0;  // air
        break;
        case 1: encode[alocare] = grass;  // grass
        break;
        case 2: encode[alocare] = wood;  //  wood
        break;
        case 3: encode[alocare] = stone;  // stone
    }
    if (nr >= first_char) {
            encode[alocare] = encode[alocare] | (1 << five);
        }
    while (nr >= max_size + 1) {
            encode[alocare+1] = max_size;
            encode[alocare] = encode[alocare] | (max_size >> eight);
            nr -= max_size;
            alocare += 2;
    }
    if (nr >= first_char) {
        encode[alocare] = encode[alocare] | (1 << five);
    }
    if (nr >= first_char  && nr <= max_size) {
        encode[alocare + 1] = nr;
        encode[alocare] = encode[alocare] | (nr >> eight);
        alocare += 2;
    }
    if (nr < first_char) {
        encode[alocare] = encode[alocare] | nr;
        alocare++;
        }
    *length = alocare;
    return encode;
}

char*** chunk_decode(
    unsigned char* code, int width, int height, int depth) {
    // block type = primii doi biti
    // number = numarul de aparitii al blocului, ultimii 5 biti
    unsigned char block_type = 0, third_bit = 0;
    int number = 0;
    unsigned char mask_block = mask_block_def, mask_number = mask_number_def;
    unsigned char mask_third_bit = mask_third_bit_def, mask_mask = mask_mask_def;
    // printf("%d %d %d", mask_number, mask_block, mask_third_bit);
    int x = 0, y = 0, z = 0, i = 0, j = 0;
    // int eight = 8, six = 6, five = 5;
    unsigned char*** chunk = calloc(width, sizeof(char**));
    for (i = 0; i < width; i++) {
        chunk[i] = calloc(height, sizeof(char*));
        for (j = 0; j < height; j++) {
                chunk[i][j] = calloc(depth, sizeof(char));
        }
    }
    i = 0;
    while (y < height) {
        third_bit = (code[i] & mask_third_bit) >> five;
        block_type = (code[i] >> six);
        if (third_bit == 1) {
            number = (code[i] & mask_number);
            number = ((number << eight) | code[i + 1]);
            i += 2;
        } else {
            number = (code[i] & mask_mask);
            i++;
        }
        while (number != 0) {
            chunk[x][y][z] = block_type;
            number--;
            x++;
            if (x == width) {
                x = 0;
                z++;
                if (z == depth) {
                    z = 0;
                    y++;
                }
            }
        }
    }
    return chunk;
}

