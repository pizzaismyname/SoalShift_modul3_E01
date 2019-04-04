# SoalShift_modul3_E01

### Soal 1
```c

```
### Soal 2
- 1 server hanya bisa terkoneksi dengan 1 client
```c
listen(server_fd, 1)
```
- Server penjual dan server pembeli memiliki stok barang yang selalu sama (menggunakan _shared memory_)
```c
int *stok;

int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
stok = shmat(shmid, NULL, 0);

shmdt(stok);
shmctl(shmid, IPC_RMID, NULL);
```
- Penambahan stok pada server penjual
```c
while(1){        
    char buffer[1024] = {0};
    char *msg;
    int valread = read( new_socket , buffer, 1024);
    if(strcmp(buffer,"tambah")==0){
        *stok += 1;
    }
}

```
- Pengurangan stok pada server pembeli disertai pengiriman info ketersediaan stok ke client
```c
while(1){
    char *msg;
    char buffer[1024] = {0};
    int valread = read( new_socket , buffer, 1024);
    if(strcmp(buffer,"beli")==0){            
        if (*stok == 0){                            //stok habis
            msg = "0";
        } 
        else {                                      //stok masih ada
            *stok -= 1;
            msg = "1";
        }
        send(new_socket , msg, sizeof(msg) , 0 );   //info ketersediaan stok
    }
}
```
- Client (melakukan input serta jika terkoneksi dengan server pembeli, akan mencetak status transaksi)
```c
while(1){
    char *msg;
    char buffer[1024] = {0};
    scanf("%s",msg);
    send(sock , msg , sizeof(msg) , 0 );
    if(strcmp(msg,"beli")==0){
        int valread = read( sock , buffer, sizeof(buffer));
        printf("transaksi ");
        if(strcmp(buffer,"0")==0) printf("gagal\n");
        else printf("berhasil\n");
    }
}
```

### Soal 3
- 
```c
```

### Soal 4
- Menyimpan file SimpanProses1.txt di direktori /home/pristiz/Documents/FolderProses1/
```c
void* simpan1(void *arg)
{
    status1 = 0;
    system("mkdir /home/pristiz/Documents/FolderProses1/");
    system("ps -aux | head -11 | tail -10 > /home/pristiz/Documents/FolderProses1/SimpanProses1.txt");
    status1 = 1;
 
    return NULL;
}
```
- Menyimpan file SimpanProses2.txt di direktori /home/pristiz/Documents/FolderProses2/
```c
void* simpan2(void *arg)
{
    status2 = 0;
    system("mkdir /home/pristiz/Documents/FolderProses2/");
    system("ps -aux | head -11 | tail -10 > /home/pristiz/Documents/FolderProses2/SimpanProses2.txt");
    status2 = 1;
 
    return NULL;
}
```
- Mengkompres zip file SimpanProses1.txt dengan format nama file KompresProses1.zip dan menghapus file SimpanProses1.txt
```c
void* zip1(void *arg)
{
    while(status1 != 1);
    rm1 = 0;
    system("zip -j /home/pristiz/Documents/FolderProses1/KompresProses1.zip /home/pristiz/Documents/FolderProses1/SimpanProses1.txt");
    system("rm -f /home/pristiz/Documents/FolderProses1/SimpanProses1.txt");
    rm1 = 1;
    
    return NULL;
}
```
- Mengkompres zip file SimpanProses2.txt dengan format nama file KompresProses2.zip dan menghapus file SimpanProses2.txt
```c
void* zip2(void *arg)
{
    while(status2 != 1);
    rm2 = 0;
    system("zip -j /home/pristiz/Documents/FolderProses2/KompresProses2.zip /home/pristiz/Documents/FolderProses2/SimpanProses2.txt");
    system("rm -f /home/pristiz/Documents/FolderProses2/SimpanProses2.txt");
    rm2 = 1;
    
    return NULL;
}
```
- Mengekstrak file KompresProses1.zip
```c
void* unzip1(void *arg)
{
    while(rm1 != 1);
    system("unzip /home/pristiz/Documents/FolderProses1/KompresProses1.zip -d /home/pristiz/Documents/FolderProses1/");
}
```
- Mengekstrak file KompresProses2.zip
```c
void* unzip2(void *arg)
{
    while(rm2 != 1);
    system("unzip /home/pristiz/Documents/FolderProses2/KompresProses2.zip -d /home/pristiz/Documents/FolderProses2/");
}
```
- Menunggu selama 15 detik lalu mengekstrak kembali file KompresProses1.zip dan KompresProses2.zip
```c
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
```

### Soal 5
```c

```
