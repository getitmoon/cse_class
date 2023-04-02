#include "assignment1.h"

int main(){
    FILE *fptr = fopen( "input.txt", "r");   
    FILE *fptr_w = fopen("output.txt", "w");
    int chunk_num = 0;
    // read file just before the end-of-file
    while (!feof(fptr)){
        // dynamic allocation for 64 sequences
        char *chunk = (char *)malloc(sizeof(char) * 64 + 1); 
        // read 64 sequences of 0 and 1 each time
        fscanf(fptr, "%64s", chunk); 
        if (!feof(fptr)){ 
            fprintf(fptr_w, "-------- Chunk %d --------\n", ++chunk_num);
            fprintf(fptr_w, "Signed Char: ");
            print_chunk(fptr_w, chunk, 1, 1); // Signed char: 1byte, signed
            fprintf(fptr_w, "ASCII Codes: ");
            print_chunk_ascii(fptr_w, chunk, 1); // ASCII: 1byte
            fprintf(fptr_w, "Unsigned Char: ");
            print_chunk(fptr_w, chunk, 1, 0); // Unsigned char: 1byte unsigned
            fprintf(fptr_w, "Signed Int: ");
            print_chunk(fptr_w, chunk, 4, 1); // Signed int: 4byte, signed
            fprintf(fptr_w, "Unsigned Int: ");
            print_chunk(fptr_w, chunk, 4, 0); // Unsigned int: 4byte, unsigned
            fprintf(fptr_w, "Signed Float: ");
            print_chunk_decimal(fptr_w, chunk, 4, 8, 127, 23); 
            // Signed float: 4byte, 8bit and 127 for exponent part, 23bit for mantissa
            fprintf(fptr_w, "Signed Double: ");
            print_chunk_decimal(fptr_w, chunk, 8, 11, 1023, 52);
            // Signed double: 8byte, 11bit and 1023 for exponent part, 52bit for mantissa
        } 
        // deallocate
        free(chunk);
    }
    fclose(fptr_w);
    fclose(fptr);
    return 0;
}



long long from_big_endian(char *chunk, int bit_num, int if_sign){
    /* from_big_endian() transforms binary digits to desired type number
    chunk is a pointer to char value '0' or '1'
    bit_num is a number of digits in binary
    if_sign is whether the type of transformed type is signed or not */
    long long result = 0;
    if (*chunk++ == '1')
        result += pow(2, --bit_num) * ((if_sign)? -1 : 1);
    else --bit_num;
    while (--bit_num >= 0)
        if (*chunk++ == '1') result += pow(2, bit_num);
    return result;
}

void print_chunk(FILE *file, char *chunk, int byte_num, int if_sign){
    /* print_chunk writes the result of from_big_endian for every chunk to given file
    file is a pointer to a file to be written on
    byte_num is size in byte of desired type*/
    int num = CHUNK_BYTE / byte_num; // There are (num) of target in every chunk
    for (int i = 0; i < num; i++){
        fprintf(file, "%lld ", from_big_endian(chunk, byte_num*8, if_sign));
        chunk += 8*byte_num;
    }
    fprintf(file, "\n");
}

void print_chunk_ascii(FILE *file, char * chunk, int byte_num){
    /* print_chunk_ascii writes the result of from_big_endian, 
    in ascii, for every chunk to given file */
    int num = CHUNK_BYTE / byte_num;
    int val;
    for (int i = 0; i < num; i++){
        val = from_big_endian(chunk, byte_num * 8, 0);
        if (val > 126 || val < 32) fprintf(file, ". ");
        // only consider value between 32 and 126 as ascii 
        else fprintf(file, "%c ", (char) val);
        chunk += 8*byte_num;
    }
    fprintf(file, "\n");
}

double to_mantissa(char * chunk, int bit_num){
    /* to_mantissa transforms binary digits to mantissa
    bit_num is number of digits*/
    double result = 0;
    for (int i = 1; i <= bit_num; i++){
        if (*chunk++ == '1') result += 1/pow(2, i);
    }
    return result;
}

void print_chunk_decimal(FILE *file, char *chunk, int byte_num, int exp_bit, int E, int man_bit){
    /* print_chunk_decimal transforms binary digits to double and write it to given file */
    double result;
    int num = CHUNK_BYTE / byte_num;
    for (int i = 0; i < num; i++){
        int sign = 1;
        if (*chunk++ == '1') sign = -1;

        int exponential_bit = (int) from_big_endian(chunk, exp_bit, 0);
        double exponent = pow(2, (exponential_bit - E));
        chunk += exp_bit;

        double mantissa = 1 + (double) to_mantissa(chunk, man_bit);
        chunk += man_bit;
    
        result = sign * mantissa * exponent;
        fprintf(file, "%.4lf ", result);
    }
    fprintf(file, "\n");
}

