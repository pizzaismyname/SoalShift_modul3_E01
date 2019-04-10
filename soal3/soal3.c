#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

int agmal_WakeUp_Status = 0;
int iraj_Spirit_Status = 100;

int cmd_agmal = -1;
int cmd_iraj = -1;

int exit_agmal = 0;
int exit_iraj = 0;

void *agmal()
{
    int strike = 0;
    exit_agmal = 0;
    while (1)
    {
        while (cmd_agmal == -1);
        switch (cmd_agmal)
        {
        case 1:
            cmd_agmal = -1;
            strike++;
            agmal_WakeUp_Status += 15;
            break;
        case 2:
            cmd_agmal = 0;
            printf("Agmal Ayo Bangun disabled 10 s\n");
            sleep(10);
            cmd_agmal = -1;
            break;
        }
        if (strike >= 3)
        {
            while (cmd_iraj > 0);
            if(cmd_iraj == -1){
                cmd_iraj = 2;
                strike = 0;
            }
        }
        if(agmal_WakeUp_Status >= 100){
            printf("Agmal Terbangun,mereka bangun pagi dan berolahraga\n");
            exit(0);
        }
    }
}

void *iraj()
{
    int strike = 0;
    exit_iraj = 0;
    while (1)
    {
        while (cmd_iraj == -1);
        switch (cmd_iraj)
        {
        case 1:
            cmd_iraj = -1;
            strike++;
            iraj_Spirit_Status -= 20;
            break;
        case 2:
            cmd_iraj = 0;
            printf("Fitur Iraj Ayo Tidur disabled 10 s\n");
            sleep(10);
            cmd_iraj = -1;
            break;
        }
        if (strike >= 3)
        {
            while (cmd_agmal > 0);
            if(cmd_agmal == -1){
                cmd_agmal = 2;
                strike = 0;
            }
        }
        if(iraj_Spirit_Status <= 0){
            printf("Iraj ikut tidur, dan bangun kesiangan bersama Agmal\n");
            exit(0);
        }
    }
}

int main()
{
    pthread_t agmalt, irajt;

    printf("AGMAL&IRAJ\n");
    pthread_create(&agmalt, NULL, &agmal, NULL);
    pthread_create(&irajt, NULL, &iraj, NULL);

    while (1)
    {
		char str[17]; 
		gets(str); 
		int i;
		for(i = 0; str[i] != '\0'; i++) str[i] = tolower(str[i]);
		str[i] = '\0';
		if(strcmp(str,"agmal ayo bangun") == 0){
			while (cmd_agmal > 0);
            if(cmd_agmal == -1) cmd_agmal = 1;
		}else if(strcmp(str,"iraj ayo tidur") == 0){
			while (cmd_iraj > 0);
            if(cmd_iraj == -1) cmd_iraj = 1;
		}else if(strcmp(str,"all status") == 0){
			printf("Agmal WakeUp_Status = %d\nIraj Spirit_Status = %d\n", agmal_WakeUp_Status, iraj_Spirit_Status);
		}else{
			printf("%s", str);
		}
    }
}