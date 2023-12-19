#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <sys/wait.h>


#define COUNT_PHIL 4
#define KEY 1234
int sem_id;

void free_forks(int philosopher)
{
    int left = philosopher;
    int right = (philosopher + 1) % COUNT_PHIL;

    struct sembuf sem_lock_left = {left, +1, 0};
    struct sembuf sem_lock_right = {right, +1, 0};

    semop(sem_id, &sem_lock_left, 1);
    semop(sem_id, &sem_lock_right, 1);
}
void dining_philosophers(int philosopher)
{
    int left = philosopher;
    int right = (philosopher + 1) % COUNT_PHIL;
    int eat = 0;

    struct sembuf sem_lock_left = {left, -1, 0};
    struct sembuf sem_lock_right = {right, -1, IPC_NOWAIT};
    struct sembuf sem_unlock_left = {left, +1, 0};
    printf("Философ %d хочет кушать\n", philosopher + 1);

    while(!eat)
    {   
        printf("Философ %d пытается взять левую вилку\n", philosopher + 1);
        if (semop(sem_id, &sem_lock_left, 1) == 0)
        {   
            printf("Философ %d взял левую вилку\n", philosopher + 1);
            printf("Философ %d пытается взять правую вилку\n", philosopher + 1);

            if (semop(sem_id, &sem_lock_right, 1) == 0)
            {   
                printf("Философ %d взял правую вилку\n", philosopher + 1);
                printf("Философ %d кушает\n", philosopher + 1);
                sleep(3);
                free_forks(philosopher);
                eat = 1;
                printf("Философ %d покушал\n", philosopher + 1);

            }
            else 
            {   
                
                semop(sem_id, &sem_unlock_left, 1);
                printf("Философ %d думает\n", philosopher + 1);
                sleep(3);
            }
        }
        else
        {   
            
            printf("Философ %d думает\n", philosopher + 1);
            sleep(3); 
        }
    }
    printf("Философ %d думает\n", philosopher + 1);
}


int main()
{

    pid_t philosopher[COUNT_PHIL];
    sem_id = semget(KEY, COUNT_PHIL, 0666 | IPC_CREAT);

    for (int i = 0; i < COUNT_PHIL; i++) 
    {
        semctl(sem_id, i, SETVAL, 1); 
    }

    for (int i = 0; i < COUNT_PHIL; i++)
    {
        philosopher[i] = fork();

        if (philosopher[i] == -1)
        {
            printf("Error");
            return 1;
        }

        if (philosopher[i] == 0)
        {
            dining_philosophers(i);
            return 0;
        }
    }

    for (int i = 0; i < COUNT_PHIL; i++)
    {
        waitpid(philosopher[i], NULL, 0); 
    }

    semctl(sem_id, 0, IPC_RMID); 
    return 0;
    
}
