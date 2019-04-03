/*
 * mkdir /home/pristiz/Documents/FolderProses1/
 * mkdir /home/pristiz/Documents/FolderProses2/
 * ps -aux | head -11 | tail -10 > /home/pristiz/Documents/FolderProses1/SimpanProses1.txt
 * ps -aux | head -11 | tail -10 > /home/pristiz/Documents/FolderProses2/SimpanProses2.txt
 * zip -j /home/pristiz/Documents/FolderProses1/KompresProses1.zip /home/pristiz/Documents/FolderProses1/SimpanProses1.txt
 * zip -j /home/pristiz/Documents/FolderProses2/KompresProses2.zip /home/pristiz/Documents/FolderProses2/SimpanProses2.txt
 * rm -f /home/pristiz/Documents/FolderProses1/SimpanProses1.txt
 * rm -f /home/pristiz/Documents/FolderProses2/SimpanProses2.txt
 * unzip /home/pristiz/Documents/FolderProses1/KompresProses1.zip -d /home/pristiz/Documents/FolderProses1/
 * unzip /home/pristiz/Documents/FolderProses2/KompresProses2.zip -d /home/pristiz/Documents/FolderProses2/
*/

#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

pthread_t tid1, tid2, tid3, tid4, tid5, tid6;
int status1, status2, rm1, rm2;

void* simpan1(void *arg)
{
    status1 = 0;
    system("mkdir /home/pristiz/Documents/FolderProses1/");
    system("ps -aux | head -11 | tail -10 > /home/pristiz/Documents/FolderProses1/SimpanProses1.txt");
    status1 = 1;
 
    return NULL;
}

void* simpan2(void *arg)
{
    status2 = 0;
    system("mkdir /home/pristiz/Documents/FolderProses2/");
    system("ps -aux | head -11 | tail -10 > /home/pristiz/Documents/FolderProses2/SimpanProses2.txt");
    status2 = 1;
 
    return NULL;
}

void* zip1(void *arg)
{
    while(status1 != 1);
    rm1 = 0;
    system("zip -j /home/pristiz/Documents/FolderProses1/KompresProses1.zip /home/pristiz/Documents/FolderProses1/SimpanProses1.txt");
    system("rm -f /home/pristiz/Documents/FolderProses1/SimpanProses1.txt");
    rm1 = 1;
    
    return NULL;
}

void* zip2(void *arg)
{
    while(status2 != 1);
    rm2 = 0;
    system("zip -j /home/pristiz/Documents/FolderProses2/KompresProses2.zip /home/pristiz/Documents/FolderProses2/SimpanProses2.txt");
    system("rm -f /home/pristiz/Documents/FolderProses2/SimpanProses2.txt");
    rm2 = 1;
    
    return NULL;
}

void* unzip1(void *arg)
{
    while(rm1 != 1);
    system("unzip /home/pristiz/Documents/FolderProses1/KompresProses1.zip -d /home/pristiz/Documents/FolderProses1/");
}

void* unzip2(void *arg)
{
    while(rm2 != 1);
    system("unzip /home/pristiz/Documents/FolderProses2/KompresProses2.zip -d /home/pristiz/Documents/FolderProses2/");
}

int main(void)
{
    
    pthread_create(&(tid1), NULL, simpan1, NULL);
    pthread_create(&(tid2), NULL, simpan2, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    pthread_create(&(tid3), NULL, zip1, NULL);
    pthread_create(&(tid4), NULL, zip2, NULL);
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);
    
    printf("Menunggu 15 detik untuk mengekstrak kembali\n");
    sleep(15);
    
    pthread_create(&(tid5), NULL, unzip1, NULL);
    pthread_create(&(tid6), NULL, unzip2, NULL);    
    pthread_join(tid5, NULL);
    pthread_join(tid6, NULL);
    
    return 0;
}
