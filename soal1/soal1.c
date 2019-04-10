#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<math.h>

int num[1000];
int ind = -1;

int cmp (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

void *thread() {
    int this = ++ind;

    int i, fact = 1;
    for (i=1; i<=num[this]; i++) {
        fact *= i;
    }

    sleep(num[this]);
    printf("%d! = %d\n", num[this], fact);
}

int main (int argc, char* argv[]) {
    
    int i;

    for(i=1; i<argc; i++){
        num[i-1] = atoi(argv[i]);
    }
    
    int n = argc - 1;
    pthread_t tid[n];
    
    qsort(num,n,sizeof(int),cmp);

    for(i=0; i<n; i++){
        pthread_create(&(tid[i]), NULL, &thread, NULL);
    }
    
    for(i=0; i<n; i++){
        pthread_join(tid[i], NULL);
    }
    
    return 0;
}
