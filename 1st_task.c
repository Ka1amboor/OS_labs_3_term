#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>


enum ERRORS
{
    ok = 0,
    error = 1,

};

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("ERROR! few arguments, please enter name file\n");
        return error;
    }

    FILE* file;
    if  (!(file = fopen(argv[1], "wb+"))) // wb+ create binary file for read and write
    {
        printf("Cant't open file\n");
        return error;
    }

    char bytes[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};

    fwrite(bytes, sizeof(bytes), sizeof(bytes)/sizeof(char), file);

    printf("byte-by-byte output of content to the console: \n");
    int size = sizeof(bytes)/sizeof(char);
    printf("setting the position to the beginning of the file: \n");
    if (fseek (file, 0, SEEK_SET) == ok) //return o if success
    {
        printf("complete\n");
    }
    else
    {
        printf("error\n");
    }

    char* buffer = (char*)malloc(sizeof(char) * size);

    if (!buffer)
    {
        printf("failed to allocate memory\n");
        return error;
    }

    fread(buffer, sizeof(char), sizeof(bytes)/sizeof(char), file);
    for (int i = 0; i < size; i ++)
    {
        printf("%d\n", buffer[i]);
        printf("_fileno: %d\n", file ->_fileno);
        printf("_flags: %d\n", file ->_flags);//the state of the stream
        printf("_IO_buf_base: %u\n", *(file ->_IO_buf_base));
        printf("_IO_buf_end: %u\n", *(file ->_IO_buf_end));
        printf("_freeres_buf: %p\n", file ->_freeres_buf);
        printf("\n");
    }

    fclose(file);
    free(buffer);

    FILE* file_for_read = fopen(argv[1], "rb"); //open the binary file for read and write
    printf("After  mooving the pointer to 3: ");
    if (fseek(file_for_read, 3, SEEK_SET) != ok)
    {
        printf("error mooving pointer\n");
        return error;
        fclose(file_for_read);
        return ok;
    }

    char new_buffer[4];
    if(fread(new_buffer, sizeof(char), 4, file_for_read) != 4)
    {
        printf("erro with read file");
        fclose(file_for_read);
        return error;
    }

    for (int i = 0; i < 4; i++)
    {
        printf("%d ", new_buffer[i]);

    }
    printf("\n");
    fclose(file_for_read);
    return ok;
}
