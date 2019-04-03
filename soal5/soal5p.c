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

key_t key = 5678;
int shmid;
int *store_food;

void main()
{
    init();

    shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    store_food = shmat(shmid, NULL, 0);

    while (1)
    {
        system("clear");
        printf("Shop\n");
        printf("Food stock : %d\n", *store_food);

        printf("Choices\n[1] Restock\n[2] Exit\n");

        if (kbhit())
        {
            int a = (int)getchar() - 48;
            if (a == 1)
            {
                (*store_food)++;
                printf("+1\n");
            }
            else if (a == 2)
            {   
                break;
            }
        }
        usleep(700);
    }

    shmdt(store_food);
    shmctl(shmid, IPC_RMID, NULL);
}