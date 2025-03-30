#include "chunk.h"
#include <stdio.h>
#include <stdlib.h>

char*** chunk_place_block(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block) {
    if (x < width && y < height && z < depth && x > 0 && y > 0 && z > 0) {
        chunk[x][y][z] = block;
    }
    return chunk;
}

char*** chunk_fill_cuboid(
    char*** chunk, int width, int height, int depth,
    int x0, int y0, int z0, int x1, int y1, int z1, char block) {
    int i = 0, j = 0, k = 0;
    if (x0 > x1) {
        int temp = 0;
        temp = x0;
        x0 = x1;
        x1 = temp;
    }
    if (y0 > y1) {
        int temp = 0;
        temp = y0;
        y0 = y1;
        y1 = temp;
    }
    if (z0 > z1) {
        int temp = 0;
        temp = z0;
        z0 = z1;
        z1 = temp;
    }
    if (x1 > width) {
        x1 = width;
    }
    if (y1 > height) {
        y1 = height;
    }
    if (z1 > depth) {
        z1 = depth;
    }
    if (x0 < 0) {
        x0 = 0;
    }
    if (y0 < 0) {
        y0 = 0;
    }
    if (z0 < 0) {
        z0 = 0;
    }
    for (i = x0; i <= x1; i++) {
        for (j = y0; j <= y1; j++) {
            for (k = z0; k <= z1; k++) {
                chunk[i][j][k] = block;
            }
        }
    }
    return chunk;
}

char*** chunk_fill_sphere(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, double radius, char block) {
    int i = 0, j = 0, k = 0;
    for (i = 0 ; i < width; i++) {
        for (j = 0; j < height; j++) {
            for (k = 0; k < depth; k++) {
                if ((x - i) * (x - i) + (y - j) * (y - j) + (z - k) * (z - k) <= radius * radius) {
                    chunk[i][j][k] = block;
                }
            }
        }
    }
    if (x >= 0 && x <= width && y >= 0 && y <= height
    && z >= 0 && z <= depth && x * x + y * y + z * z == 0) {
        chunk[x][y][z] = block;
    }
    return chunk;
}
