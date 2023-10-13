#include <stdio.h>

enum STATUS_CODE
{
    success = 0,
    invalid_input = 1,
    files_problems = 2


};
int check_empty(FILE* input_file)
{
    if (fseek(input_file, 0L, SEEK_END) != success)
    {
        return files_problems;
    }
    if(ftell(input_file) == success)
        {
            return files_problems;
        }
    fseek(input_file, 0L, SEEK_SET);
    return success;

}

int copy_file(FILE* input_file, FILE *output_file)
{   
    char buffer[1];
    char byte;
    while ((byte = fread(buffer, sizeof(char), sizeof(buffer), input_file)) > 0)
    {
        if (fwrite(buffer,sizeof(char),sizeof(byte),output_file) != byte)
        {
            return files_problems;
        }
    }
    return success;
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {   
        printf("Invalid input!\n");
        return invalid_input;
    }

    FILE* input_file = fopen(argv[1], "rb");
    if (input_file == NULL)
    {
        printf("Can't open input file!\n");
        return files_problems;
    }

    FILE* output_file = fopen(argv[2], "wb");
    if (output_file == NULL)
    {
        printf("Can't open output file!\n");
        fclose(input_file);
        return files_problems;
    }

    int result_check_empty = check_empty(input_file);
    if (result_check_empty != 0)
    {
        printf("input file is empty!\n");
        fclose(input_file);
        fclose(output_file);
        return files_problems;
    }

    int result = copy_file(input_file, output_file);
    if (result != success)
    {
        printf("Can't copy file!");
        fclose(input_file);
        fclose(output_file);
        return files_problems;
        
    }
    else
    {
        printf("file has been successfully copied!\n");
        fclose(input_file);
        fclose(output_file);
        return success;
    }
    return success;
}
