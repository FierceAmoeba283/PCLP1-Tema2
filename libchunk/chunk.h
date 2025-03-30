#ifndef CHUNK_H
#define CHUNK_H

#define BLOCK_AIR           0
#define BLOCK_GRASS         1
#define BLOCK_WOOD          2
#define BLOCK_STONE         3
#define memory              600
#define grass               64
#define wood                128
#define stone               192
#define first_char          32
#define max_size            4095
#define eight               8
#define six                 6
#define five                5
#define mask_block_def      0b11000000
#define mask_number_def     0b00001111
#define mask_third_bit_def  0b00100000
#define mask_mask_def       0b00011111
#define NR_CORPURI          4500

// Partea A

char*** chunk_place_block(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block);

char*** chunk_fill_cuboid(
    char*** chunk, int width, int height, int depth,
    int x0, int y0, int z0, int x1, int y1, int z1, char block);

char*** chunk_fill_sphere(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, double radius, char block);

// Partea B

char*** chunk_fill_sphere_modified(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, double radius, char block, char target_block);
    
char*** chunk_shell(
    char*** chunk, int width, int height, int depth,
    char target_block, char shell_block);

char*** chunk_fill_xz(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block);

char*** chunk_fill(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block);

// Partea C

char*** chunk_rotate_y(
    char*** chunk, int width, int height, int depth);

char*** chunk_apply_gravity(
    char*** chunk, int width, int height, int depth, int* new_height);

// Partea D

unsigned char* chunk_encode(
    char*** chunk, int width, int height, int depth,
    int* length);

char*** chunk_decode(
    unsigned char* code, int width, int height, int depth);
    
#endif  // CHUNK_H
