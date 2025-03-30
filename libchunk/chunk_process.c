#include "chunk.h"
#include <math.h>

char*** chunk_fill_sphere_modified(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, double radius, char block, char target_block) {
    int i = 0, j = 0, k = 0;
    for (i = 0 ; i < width; i++) {
        for (j = 0; j < height; j++) {
            for (k = 0; k < depth; k++) {
                if (((x - i) * (x - i) + (y - j) * (y - j) + (z - k) * (z - k)) <= radius * radius) {
                    if (chunk[i][j][k] != target_block) {
                        chunk[i][j][k] = block;
                    }
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

char*** chunk_shell(
    char*** chunk, int width, int height, int depth,
    char target_block, char shell_block) {
    int i = 0, j = 0, k = 0;
    if (target_block == shell_block) {
        shell_block = 4;
    }
    for  (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            for (k = 0; k < depth; k++) {
                if (chunk[i][j][k] == target_block) {
                    chunk_fill_sphere_modified(chunk, width, height, depth,
                    i, j, k, sqrt(2), shell_block, target_block);
                }
            }
        }
    }
     for  (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            for (k = 0; k < depth; k++) {
                if (chunk[i][j][k] == 4) {
                    chunk[i][j][k] = target_block;
                }
            }
        }
    }
    return chunk;
}

void search(
    char*** chunk, int width, int height, int depth,
     int x, int y, int z, char block, char replacement) {
    // modific doar x si z
    if (x >= width || z >= depth || x < 0 || z < 0 || chunk[x][y][z] != block) {
        return;
    }
    chunk[x][y][z] = replacement;
    search(chunk, width, height, depth, x - 1, y, z, block, replacement);
    search(chunk, width, height, depth, x + 1, y, z, block, replacement);
    search(chunk, width, height, depth, x, y, z - 1, block, replacement);
    search(chunk, width, height, depth, x, y, z + 1, block, replacement);
    // return;
}

char*** chunk_fill_xz(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block) {
    char original = chunk[x][y][z];
    // chunk[x][y][z] = block;
    search(chunk, width, height, depth, x, y, z, original, block);
    return chunk;
}

void search_3d(char*** chunk, int width, int height, int depth, int x, int y, int z, char block, char replacement) {
    if (x >= width || y >= height || z >= depth ||
     x < 0 || y < 0 || z < 0 || chunk[x][y][z] != block) {
        return;
    }
    chunk[x][y][z] = replacement;
    search_3d(chunk, width, height, depth, x - 1, y, z, block, replacement);
    search_3d(chunk, width, height, depth, x + 1, y, z, block, replacement);
    search_3d(chunk, width, height, depth, x, y, z - 1, block, replacement);
    search_3d(chunk, width, height, depth, x, y, z + 1, block, replacement);
    search_3d(chunk, width, height, depth, x, y - 1, z, block, replacement);
    search_3d(chunk, width, height, depth, x, y + 1, z, block, replacement);
}

char*** chunk_fill(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block) {
    char original = chunk[x][y][z];
    search_3d(chunk, width, height, depth, x, y, z, original, block);
    return chunk;
}
