#ifndef ASSIGNMENT1_H
#define ASSIGNMENT1_H
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define CHUNK_BYTE 8
void print_chunk(FILE *file, char *chunk, int byte_num, int if_sign);
void print_chunk_ascii(FILE *file, char *chunk, int byte_num);
void print_chunk_decimal(FILE *file, char *chunk, int byte_num, int exp_bit, int E, int man_bit);
#endif 