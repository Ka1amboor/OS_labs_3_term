#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

sem_t women_mutex; 
sem_t men_mutex;
sem_t count_mutex;
int count = 0; // Количество людей в ванной комнате  отрицатльное - в ваной женщины(их кол-во равно модулю)  если положительное - в ванной мужчины
int max_count;

void woman_wants_to_enter()
{
    int enter = 0;
    sem_wait(&women_mutex); 
    sem_wait(&count_mutex);
    printf("Женщина хочет зайти\n");
    if (count <= 0 && abs(count) + 1 <= max_count)
    {
        count--;
        printf("Женщина вошла в ванную комнату\n");
        printf("Количество женщин в ванной комнате: %d\n", abs(count));
        sem_post(&count_mutex);
        sem_post(&women_mutex);
        enter = 1;
    }
    else
    {
        if (count > 0) 
        {
            printf("Женщина ожидает пока все мужчины выйдут из ванной комнаты\n");
        }
        else 
        {
            printf("Ванная переполнена, женщина ожидает\n");
        }
        sem_post(&count_mutex);
        sem_post(&women_mutex);
        while (!enter)
        {
            sem_wait(&women_mutex);
            sem_wait(&count_mutex);
            if (count <= 0 && abs(count) + 1 <= max_count)
            {
                count--;
                printf("Женщина вошла в ванную комнату\n");
                printf("Количество женщин в ванной комнате: %d\n", abs(count));
                sem_post(&count_mutex);
                sem_post(&women_mutex);
                enter = 1;
            }
            else
            {
                sem_post(&count_mutex);
                sem_post(&women_mutex);
            }
        }
    }
}

void man_wants_to_enter()
{
    int enter = 0;
    sem_wait(&men_mutex);
    sem_wait(&count_mutex);
    printf("Мужчина хочет зайти\n");
    if (count >= 0 && count + 1 <= max_count)
    {
        count++;
        printf("Мужчина вошел в ванную комнату\n");
        printf("Количество мужчин в ванной комнате: %d\n", count);
        enter = 1;
        sem_post(&count_mutex);
        sem_post(&men_mutex);
    }
    else
    {
        if (count < 0) 
        {
            printf("Мужчина ждёт пока все женщины выйдут из ванной комнаты\n");
        }
        else 
        {
            printf("Ванная переполнена, мужчина ожидает\n");
        }
        sem_post(&count_mutex);
        sem_post(&men_mutex);
        while (!enter)
        {
            sem_wait(&men_mutex);
            sem_wait(&count_mutex);

            if (count >= 0)
            {
                count++;
                printf("Мужчина вошел в ванную комнату\n");
                printf("Количество мужчин в ванной комнате: %d\n", count);
                enter = 1;
                sem_post(&count_mutex);
                sem_post(&men_mutex);
            }
            else
            {
                sem_post(&count_mutex);
                sem_post(&men_mutex);
            }
        }
    }
}

void woman_leaves()
{
    sem_wait(&count_mutex);
    count++;
    printf("Женщина покинула ванную комнату\n");
    printf("Количество женщин в ванной комнате: %d\n", abs(count));
    sem_post(&count_mutex);
}

void man_leaves()
{
    sem_wait(&count_mutex);
    count--;
    printf("Мужчина покинул ванную комнату\n");
    printf("Количество мужчин в ванной комнате: %d\n", count);
    sem_post(&count_mutex);
}
void* woman(void* arg)
{
    woman_wants_to_enter();
    sleep(3);
    woman_leaves();
    return NULL;
}

void* man(void* arg)
{
    man_wants_to_enter();
    sleep(3);
    man_leaves();
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Укажите максимальное количество людей в ванной комнате\n");
        return 1;
    }

    int n = atoi(argv[1]);
    max_count = n;
    

    sem_init(&women_mutex, 0, 1);
    sem_init(&men_mutex, 0, 1);
    sem_init(&count_mutex, 0, 1);

    pthread_t women[n];
    pthread_t men[n];

    for (int i = 0; i < n; i++)
    {
        pthread_create(&women[i], NULL, woman, NULL);
        pthread_create(&men[i], NULL, man, NULL);
    }

    for (int i = 0; i < n; i++)
    {
        pthread_join(women[i], NULL);
        pthread_join(men[i], NULL);
    }

    sem_destroy(&women_mutex);
    sem_destroy(&men_mutex);
    sem_destroy(&count_mutex);

    return 0;
}