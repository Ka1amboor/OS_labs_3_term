#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum STATUS_CODE
{
    success = 0,
    invalid_input = 1,
    file_problem = 2
};

long int xor_8(FILE* input_file)
{
    unsigned char byte;
    long int result = 0;
    while(fread(&byte, sizeof(unsigned char), 1, input_file) > 0)
    {   
        result ^= byte;
    }
    return result;
}

int xor_32(FILE* input_file)
{
    unsigned int bytes;
    long int result = 0;
    while(fread(&bytes, sizeof(unsigned int), 1, input_file) > 0)
    {   
        result ^= bytes;
    }
    return result;
}
int convert_to_decimal(char* number, int base)
{
    int result = 0;
    char* ptr = number - 1;

    while(*++ptr)
    {
        result = result * base + (*ptr > '9' ? *ptr - 'A' + 10 : *ptr - '0');
    }
    return result;
}

long int mask(FILE* input_file, char* argv)
{
    unsigned char read_mask[4] = {0, 0, 0, 0};
    long int result = 0;
    unsigned long int mask = convert_to_decimal(argv, 16);
    while (fread(read_mask, sizeof(unsigned char), 4, input_file) > 0)
    {
        if(memcmp(read_mask, &mask, 4) == 0)
        {
            result += 1;
        }
    }
    return result;

}

int main(int argc, char* argv[])
{
    if(argc != 3 && argc != 4)
    {
        printf("Invalid input!\n");
        return invalid_input;
    }

    FILE* input_file = fopen(argv[1], "rb");
    if (input_file == NULL)
    {
        printf("Can't open file!\n");
        return file_problem;
    }
    
    if (strcmp(argv[2], "xor8") == 0)
    {   
        long int xor_8_result = xor_8(input_file);
        printf("result of xor8: %ld\n", xor_8_result);
        return success;
    }

    if (strcmp(argv[2], "xor32") == 0)
    {   
        long int xor_32_result = xor_32(input_file);
        printf("result of xor32: %ld\n", xor_32_result);
        return success;
    }

    if (strcmp(argv[2], "mask") == 0 && argc == 4)
    {   
        long int mask_result = mask(input_file, argv[3]);
        printf("%ld\n", mask_result);
        return success;
    }

    printf("Invalid input!\n");
    return invalid_input;
}