#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "assignment1.h"

typedef struct one_byte
{
    char bits[8];
} one_byte;

int bits_to_char(char *str);
int bits_to_uchar(char *str);

void print_char(one_byte *byte);
void print_ascii(one_byte *byte);
void print_uchar(one_byte *byte);
void print_int(one_byte *byte);
void print_uint(one_byte *byte);
void print_float(one_byte *byte);
void print_double(one_byte *byte);

int main(){
    FILE *fptr = fopen( "input.txt", "r");   
    size_t size = sizeof(one_byte);
    int byte_num = 0;
    one_byte *byte;
    while (!feof(fptr)){
        byte = (one_byte *) realloc(byte, ++byte_num * size + 1); //corrupted size vs. prev_size
        fscanf(fptr, "%8s", (byte + byte_num - 1)->bits);
    }
    fclose(fptr);
    byte_num = byte_num - 1;
    int chunk = byte_num / 8;

    for (size_t i = 0; i < chunk; i++)
    {
        printf("-------- Chunk %ld --------\n", i + 1);
        printf("Signed Char: ");
        print_char(byte + 8 * i);
        printf("ASCII Codes: ");
        print_ascii(byte + 8 * i);
        printf("Unsigned Int: ");
        print_uchar(byte + 8 * i);
        printf("Signed Int: ");
        print_int(byte + 8 * i);
        printf("Unsigned Int: ");
        print_uint(byte + 8 * i);
        printf("Signed Float: ");
        print_float(byte + 8 * i);
        printf("Signed Double: ");
        print_double(byte + 8 * i);
    }
    free(byte);
    return 0;
}

int bits_to_char(char *str)
{
    int data = 0;
    if (*str == '1')
        data += -(1 << 7);
    for (size_t i = 1; i < 8; i++)
    {
        if (*(str + i) == '1')
            data += (1 << (7-i));
    }    
    return data; 
}

int bits_to_uchar(char * str)
{
    int data = 0;
    for (size_t i = 0; i < 8; i++)
    {
        if (*(str + i) == '1')
            data += (1 << (7-i));
    }
    return data;
}

void print_char(one_byte *byte)
{
    for (size_t i = 0; i < 8; i++){
        printf("%d ", bits_to_char((byte + i) -> bits));
    }
    printf("\n");
}           

void print_ascii(one_byte *byte)
{
    for (size_t i = 0; i < 8; i++){
        if (bits_to_uchar((byte + i) -> bits) > 127)
            printf(". ");
        else printf("%c ", (char) bits_to_uchar((byte + i) -> bits));
    }       
    printf("\n");
}

void print_uchar(one_byte *byte)
{
    for (size_t i = 0; i < 8; i++){
        printf("%d ", bits_to_uchar((byte + i) -> bits));
    }
    printf("\n");
}

void print_int(one_byte *byte)
{   
    int result;
    for (size_t i = 0; i < 2; i++){
        result = 0;
        for (size_t j = 0; j < 3; j++){
            result += bits_to_uchar((byte + 4 * i + (3-j)) -> bits) << (8 * j);
        }
        result += bits_to_char((byte + 4 * i) -> bits) << 24;
        printf("%d ", result);
    }
    printf("\n");
}

void print_uint(one_byte *byte)
{
    int result;
    for (size_t i = 0; i < 2; i++){
        result = 0;
        for (size_t j = 0; j < 4; j++){
            result += bits_to_uchar((byte + 4 * i + (3-j)) -> bits) << (8 * j);
        }
        printf("%u ", result);
    }
    printf("\n");
}

void print_float(one_byte *byte)
{   
    float result;
    int i, j;
    for (i = 0; i < 2; i++){
        int sign = 1;
        if (((byte + 4 * i) -> bits)[0] == '1') sign = -1;

        int exponential_bit = 0;
        for (j = 0; j < 8; j++){
            if (((byte + 4 * i) -> bits)[8-j] == '1')
                exponential_bit += 1 << j;
        }
        float exponent = pow(2, (exponential_bit - 127));

        float mantissa_bit = 0;
        for (j = 0; j < 23; j++){
            if (((byte + 4 * i) -> bits)[9+j] == '1')
                mantissa_bit += 1/pow(2, (j+1));
        }
        float mantissa = 1 + mantissa_bit;
    
        result = sign * mantissa * exponent;
        printf("%.4f ", result);
    }
    printf("\n");
}

void print_double(one_byte *byte)
{
    double result;
    int i;
    int sign = 1;
    if ((byte -> bits)[0] == '1') sign = -1;

    int exponential_bit = 0;
    for (i = 0; i < 11; i++){
        if ((byte -> bits)[11-i] == '1')
            exponential_bit += 1 << i;
    }
    double exponent = pow(2, (exponential_bit - 1023));

    double mantissa_bit = 0;
    for (i = 0; i < 52; i++){
        if ((byte -> bits)[12+i] == '1')
            mantissa_bit +=  1/pow(2, (i+1));
    }
    double mantissa = 1 + mantissa_bit;
    
    result = sign * mantissa * exponent;
    printf("%.4e\n", result);
}

