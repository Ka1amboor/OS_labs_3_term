#include <stdio.h>
#include <string.h>

#define MAX_USERS 10

enum STATUS_CODE
{
    succcess = 0,
    error = 1,
    invalid_input = 2,

};

struct User
{
    char login[7];
    int pin;
    int requests;
};

int main()
{
    struct User users[MAX_USERS];
    int num_users = 0;
    int current_user_index = -1; // (-1 if nobody has registered yet)
    int num_request = 0;

    do
    {
        if (current_user_index == -1)
        {
            printf("--------------------------------------------------------------------------\n");
            printf("\t\t\t\t WELCOME!\n");
            printf("\t\t\t Please, register, if you have' t go an account, (<LOGIN> <PIN>):\n");
            printf("\t\t\t Or enter <REGISTER> for create a new account\n");
            printf("--------------------------------------------------------------------------\n");
            
            char input[20];
            fgets(input,20,stdin);

            
        }
        else
        {

        }
    } while (current_user_index == -1);
    






return succcess;
}
