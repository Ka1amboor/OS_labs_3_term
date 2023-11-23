#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

enum status_code
{
    SUCCESS = 0,
    INVALID_INPUT = 1,
    ERROR_WITH_OPENING_FILE = 2,
    NO_SUBSTRUCTION = 3,
    NO = 4,
    YES = 5
};

enum status_code search_substruction(char* substruction, FILE** file_1)
{
    char c;
    int len = strlen(substruction);
    
    while((c = fgetc(*file_1))!= EOF)
    {
        int i = 0;

        while(c == substruction[i] && i < len)
        {
            c = fgetc(*file_1);
            i++;
        }

        if (i == len)
        {
            return YES;
        }
    }

    return NO;

    
}

enum status_code search_files(char* substruction, char* file)
{
    FILE* file_with_names = fopen(file, "r");

    if(!file_with_names)
    {
        return ERROR_WITH_OPENING_FILE;
    }

    FILE* file_1;
    char names_file[256] = {0}; //имена файлов
    char c;
    int i, flag = 0;
    pid_t pid;

    while ((c = fgetc(file_with_names)) != EOF && flag == 0)
    {
        i = 0;

        while(c != '\n' && c != '\t' && c != ' ' && c != EOF)
        {
            names_file[i] = c;
            i++;
            c = fgetc(file_with_names);
        }
        names_file[i] = '\0';

        if(i != 0)
        {
            pid = fork();
            
            if (pid == 0)
            {
                if((file_1 = fopen(names_file, "r")) != NULL)
                {
                    if(search_substruction(substruction, &file_1) == NO)
                    {
                        printf("No matches in file: %s \n", names_file);
                    }

                    else
                    {
                        printf("%s\n", names_file);

                    }
                    fclose(file_1);
                }
                else
                {
                    printf("file not open\n");
                }
                flag = 1;
                break;
            }
            else if (pid == -1)
            {
                printf("pid error\n");
            }
        }
    }

    fclose(file_with_names);
    return SUCCESS;

}


int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        printf("Invalid input\n");
        return INVALID_INPUT;
    }

    char* substruction = argv[1];
    char* file = argv[2];
    

    if (search_files(substruction, file) == ERROR_WITH_OPENING_FILE)
    {
        printf("error with opening input file\n");
        return ERROR_WITH_OPENING_FILE;
    }

    return 0;
}