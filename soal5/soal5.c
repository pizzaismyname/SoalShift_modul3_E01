#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <termios.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#pragma region Terminos
static struct termios g_old_kbd_mode;
static int kbhit(void)
{
    struct timeval timeout;
    fd_set read_handles;
    int status;

    FD_ZERO(&read_handles);
    FD_SET(0, &read_handles);
    timeout.tv_sec = timeout.tv_usec = 0;
    status = select(0 + 1, &read_handles, NULL, NULL, &timeout);
    return status;
}
static void old_attr(void)
{
    tcsetattr(0, TCSANOW, &g_old_kbd_mode);
}
static void init()
{
    //Set terminal supaya bisa nerima input keyboard tanpa enter
    //Emulator fungsi kbhit()
    struct termios new_kbd_mode;
    tcgetattr(0, &g_old_kbd_mode);
    memcpy(&new_kbd_mode, &g_old_kbd_mode, sizeof(struct termios));
    new_kbd_mode.c_lflag &= ~(ICANON | ECHO);
    new_kbd_mode.c_cc[VTIME] = 0;
    new_kbd_mode.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &new_kbd_mode);
    atexit(old_attr);
}
#pragma endregion Terminos

int monster_hunger = 200; //5/10, 0 kalah
int monster_hygine = 100; //10/30, 0 kalah
int monster_health = 300; //+5/10, 0 kalah

int npc_health = 100; //init 100

int food_stock = 0; //Pakai shared memory
int bath_remaining = 20;

int shmid;
int *store_food;

int f_stop = 0;
key_t key = 5678;
pthread_t timert, displayt;

int view = 0; //0:Standby, 1:Battle Mode, 2: Shop

void *display()
{
    while (1)
    {
        system("clear");
        if (view == 0)
        {
            printf("Standby Mode\n");
            printf("Health : %d\n", monster_health);
            printf("Hunger : %d\n", monster_hunger);
            printf("Hygiene : %d\n", monster_hygine);
            printf("Food left : %d\n", food_stock);
            if (bath_remaining < 20)
            {
                printf("Bath will be ready in %ds\n", (20 - bath_remaining));
            }

            printf("Choices\n[1] Eat\n[2] Bath\n[3] Battle\n[4] Shop\n[5] Exit\n");
        }
        else if (view == 1)
        {
            printf("Battle Mod\n");
            printf("Monster’s Health : %d\n", monster_health);
            printf("Enemy’s Health : %d\n", npc_health);

            printf("Choices\n[1] Attack\n[2] Run\n");
        }
        else if (view == 2)
        {
            printf("Shop Mode\n");
            printf("Shop food stock : %d\n", *store_food);
            printf("Your food stock : %d\n", food_stock);

            printf("Choices\n[1] Buy\n[2] Back\n");
        }
        sleep(1);
    }
}

void max()
{
    if (monster_hunger > 200)
        monster_hunger = 200;
    if (monster_hygine > 100)
        monster_hygine = 100;
    if (monster_health > 300)
        monster_health = 300;
}

void *timer()
{
    int c = 0;
    while (1)
    {
        sleep(1);
        c++;
        if (f_stop == 0)
        {
            if (c % 30 == 0)
            {
                //per 30 detik
                monster_hygine -= 10;
            }
            if (c % 10 == 0)
            {
                //per 10 detik
                monster_hunger -= 5;
                monster_health += 5;
            }
        }
        if (bath_remaining < 20)
        {
            bath_remaining++;
        }
        if (view == 1)
        {
            int a = rand() % 1 + 1;
            if (a == 1)
            {
                monster_health -= 20;
                printf("NPC: ATTACK!!!\n");
            }
        }
        max();

        if (monster_health == 0 || monster_hygine == 0 || monster_health == 0)
        {
            printf("Ugh, I'm dead!\n");
            exit(0);
        }
    }
}

void setTimer()
{
    //Timer per 10 detik -> per 30 detik, display
    pthread_create(&timert, NULL, &timer, NULL);
    pthread_create(&displayt, NULL, &display, NULL);
}

int main(void)
{
    init();
    setTimer();
    //Main thread akan jadi input
    while (1)
    {
        if (kbhit())
        {
            int a = (int)getchar() - 48;
            if (view == 0)
            {
                switch (a)
                {
                case 1:
                    if (food_stock <= 0)
                    {
                        printf("Nothing to eat!\n");
                    }
                    else
                    {
                        food_stock--;
                        monster_hunger += 15;
                        printf("NYUM NYUMM!\n");
                    }
                    break;
                case 2:
                    if (bath_remaining >= 20)
                    {
                        monster_hygine += 30;
                        bath_remaining = 0;
                        printf("Bathing!\n");
                    }
                    else
                    {
                        printf("The water is not ready!\n");
                    }
                    break;
                case 3:
                    npc_health = 100;
                    f_stop = 1;
                    view = 1;
                    break;
                case 4:
                    view = 2;
                    //init shm
                    shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
                    store_food = shmat(shmid, NULL, 0);
                    break;
                case 5:
                    printf("Bye-bye!\n");
                    exit(0);
                }
            }
            else if (view == 1)
            {
                if (a == 1)
                {
                    npc_health -= 20;
                    printf("Monster: ATTACK!!!\n");
                    if (npc_health <= 0)
                    {
                        printf("Monster: VICTORY!!!\n");
                        view = 0;
                        f_stop = 0;
                    }
                }
                else if (a == 2)
                {
                    printf("Monster: Later lah bro!!!\n");
                    view = 0;
                    f_stop = 0;
                }
            }
            else if (view == 2)
            {
                if (a == 1)
                {
                    if (*store_food > 0)
                    {
                        (*store_food)--;
                        food_stock++;
                        printf("$ $ $\n");
                    }
                    else
                    {
                        printf("X X X\n");
                    }
                }
                else if (a == 2)
                {
                    view = 0;
                    f_stop = 0;
                    shmdt(store_food);
                    shmctl(shmid, IPC_RMID, NULL);
                }
            }
        }
        max();
        usleep(100);
    }
}