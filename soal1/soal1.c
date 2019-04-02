#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<math.h>

int num[1000];
int id;

void *thread() {
    int i, fact = 1;
    for (i=1; i<=num[id-1]; i++) {
        fact *= i;
    }
    printf("%d! = %d\n", num[id-1], fact);
}

int cmp (const void * a, const void * b) {
  return ( *(int*)a - *(int*)b );
}

int main (int argc, char* argv[]) {
    
    int arg;
    for (arg = 1; arg < argc; ++arg) {
        num[arg - 1] = atoi(argv[arg]);
    }
    
    int n = argc - 1;
    pthread_t tid[n];
    
    qsort(num, n, sizeof(int), cmp);

    id = 0;
    while (id++ < n)
    {
        pthread_create(&(tid[id]), NULL, &thread, NULL);
        pthread_join(tid[id], NULL);
    }
    
    return 0;
}
