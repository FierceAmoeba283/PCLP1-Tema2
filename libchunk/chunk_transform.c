#include "chunk.h"
#include <stdio.h>
#include <stdlib.h>

char*** chunk_rotate_y(
    char*** chunk, int width, int height, int depth) {
    // inversez depth cu width chunk_nou[depth][height][width]
    int i = 0, j = 0, k = 0, new_width = depth, new_depth = width;
    char*** chunk_nou = malloc(depth * sizeof(char**));
    for (i = 0; i < depth; i++) {
        chunk_nou[i] = malloc(height * sizeof(char*));
        for (j = 0; j < height; j++) {
            chunk_nou[i][j] = malloc(width * sizeof(char));
        }
    }
    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            for (k = 0; k < depth; k++) {
                chunk_nou[k][j][i] = chunk[i][j][depth - k - 1];
            }
        }
    }
    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            free(chunk[i][j]);
        }
        free(chunk[i]);
    }
    free(chunk);
    return chunk_nou;
}

int*** alloc_corpuri(int width, int height, int depth) {
    int i = 0, j = 0;
    // folosesc calloc pentru a initializa toate elementele pointerului cu 0
    int*** corpuri = calloc(width, sizeof(int**));
    for (i = 0; i < width; i++) {
        corpuri[i] = calloc(height, sizeof(int*));
        for (j = 0; j < height; j++) {
            corpuri[i][j] = calloc(depth, sizeof(int));
        }
    }
    return corpuri;
}

void swapp(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void add(int*** corpuri, int width, int height, int depth, int x, int y, int z,
 char*** chunk, char block, int nr_corp) {
    if (x >= width || y >= height || z >= depth || x < 0 || y < 0 || z < 0 || chunk[x][y][z] != block) {
        return;
    }
    // tot ce e mai jos in functie da seg-fault
    corpuri[x][y][z] = nr_corp;
    // printf("%d", corpuri[x][y][z]);
    add(corpuri, width, height, depth, x + 1, y, z, chunk, block, nr_corp);
    add(corpuri, width, height, depth, x - 1, y, z, chunk, block, nr_corp);
    add(corpuri, width, height, depth, x, y + 1, z, chunk, block, nr_corp);
    add(corpuri, width, height, depth, x, y - 1, z, chunk, block, nr_corp);
    add(corpuri, width, height, depth, x, y, z + 1, chunk, block, nr_corp);
    add(corpuri, width, height, depth, x, y, z - 1, chunk, block, nr_corp);
}

void cadere(int*** corpuri, int width, int height, int depth, int* pozitii) {
    int x = 0, y = 0, z = 0;
    for (x = 0; x < width; x++) {
        for (y = 0; y < height; y++) {
            for (z = 0; z < depth; z++) {
                if (corpuri[x][y][z] != 0) {
                    // daca blocul apartine unui corp
                    int i = y - 1;
                    int nr = 0;
                    if (corpuri[x][y - 1][z] != corpuri[x][y][z]) {
                        while (corpuri[x][i][z] == 0 || i > 0) {
                            nr++;
                            i--;
                        }
                        if (nr < pozitii[corpuri[x][y][z]]) {
                            pozitii[corpuri[x][y][z]] = nr;
                        }  // acum stim cate blocuri poate cadea fiecare corp
                    }
                }
            }
        }
    }
}

int cadere_diferit(int*** corpuri, int width, int height, int depth) {
    int x = 0, y = 0, z = 0, pozitii_total = height;
    for (x = 0; x < width; x++) {
        for (y = 0; y < height; y++) {
            for (z = 0; z < depth; z++) {
                if (corpuri[x][y][z] != 0 && corpuri[x][y - 1][z] == 0) {
                    // daca blocul apartinie unui corp si dedesubtul sau este aer
                    int i = y - 1;
                    int nr = 0;
                    while (corpuri[x][i][z] == 0 || i > 0) {
                        i--;
                        nr++;
                    }
                    if (nr < pozitii_total) {
                        pozitii_total = nr;
                    }
                }
            }
        }
    }
    return pozitii_total;
}

int eliminare_aer(int*** corpuri, int width, int height, int depth) {
    int x = 0, y = 0, z = 0;
    for (y = height - 1; y >= 0; y--) {
        for (x = 0; x < width; x++) {
            for (z = 0; z < depth; z++) {
                if (corpuri[x][y][z] != 0) {
                    return y + 1;
                }
            }
        }
    }
}

void free_chunk(char*** chunk, int width, int height, int depth) {
    int i = 0, j = 0;
    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            free(chunk[i][j]);
        }
        free(chunk[i]);
    }
    free(chunk);
}

char*** chunk_apply_gravity(
    char*** chunk, int width, int height, int depth, int* new_height) {
    int x = 0, y = 0, z = 0, i = 0, j = 0, k = 0, nr_corp = 0;
    // definim un nou chunk care sa tina minte numarul corpului de care apartine fiecare bloc
    // definim un vector care sa inregstreze numarul corpului si tipul de bloc aferent acestuia
    // definim un vector care tine minte cate blocuri poate cadea fiecare corp
    int*** corpuri = calloc(width, sizeof(int**));
    for (i = 0; i < width; i++) {
        corpuri[i] = calloc(height, sizeof(int*));
        for (j = 0; j < height; j++) {
            corpuri[i][j] = calloc(depth, sizeof(int));
        }
    }
    char* tip_corp = calloc(NR_CORPURI, sizeof(char));
    int *pozitii = calloc(NR_CORPURI, sizeof(int));
    // numerotez corpurile intr-un chunk separat si memorez intr-un vector blocurile asociate acestora
    for (x = 0; x < width; x++) {
        for (y = 0; y < height; y++) {
            for (z = 0; z < depth; z++) {
                if (chunk[x][y][z] != 0 && corpuri[x][y][z] == 0) {
                    nr_corp++;
                    add(corpuri, width, height, depth, x, y, z, chunk, chunk[x][y][z], nr_corp);
                    // add da seg-fault
                    tip_corp[nr_corp] = chunk[x][y][z];
                    // printf("%d", nr_corp);
                }
            }
        }
    }
    for (i = 0; i < nr_corp; i++) {
        pozitii[i] = height;
    }
    cadere(corpuri, width, height, depth, pozitii);
    // cobor corpurile individual
    for (x = 0; x < width; x++) {
        for (y = 0; y < height; y++) {
            for (z = 0; z < depth; z++) {
                if (corpuri[x][y][z] != 0) {
                    int poz = y - 1, gravitatie = pozitii[corpuri[x][y][z]];
                    // gravitatie = numarul de coborari pe care le are blocul de facut
                    // poz = pozitia acgtuala pe inaltimes
                    while (gravitatie != 0) {
                        gravitatie--;
                        swapp(&corpuri[x][poz + 1][z], &corpuri[x][poz][z]);  // !!
                        poz--;
                    }
                }
            }
        }
    }
    // declar o variabile care va tine minte cate pozitii pot cobori toate blocurile deodata
    int pozitii_total = cadere_diferit(corpuri, width, height, depth);
    for (x = 0; x < width; x++) {
        for (y = 0; y < height; y++) {
            for (z = 0; z < depth; z++) {
                if (corpuri[x][y][z] != 0) {
                    int poz = y - 1, gravitatie = pozitii_total;
                    // gravitatie = numarul de coborari pe care le are blocul de facut
                    // poz = pozitia acgtuala pe inaltimes
                    while (gravitatie != 0) {
                        gravitatie--;
                        swapp(&corpuri[x][poz + 1][z], &corpuri[x][poz][z]);
                        poz--;
                    }
                }
            }
        }
    }
    *new_height = eliminare_aer(corpuri, width, height, depth);
    free_chunk(chunk, width, height, depth);
    chunk = calloc(width, sizeof(char**));
    for (i = 0; i < width; i++) {
        chunk[i] = calloc(*new_height, sizeof(char*));
        for (j = 0; j < *new_height; j++) {
            chunk[i][j] = calloc(depth, sizeof(char));
        }
    }
    for (x = 0; x < width; x++) {
        for (y = 0; y < *new_height; y++) {
            for (z = 0; z < depth; z++) {
                if (corpuri[x][y][z] != 0) {
                    chunk[x][y][z] = tip_corp[corpuri[x][y][z]];
                } else {
                    chunk[x][y][z] = 0;
                }
            }
        }
    }
    return chunk;
}

