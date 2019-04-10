# SoalShift_modul3_E01

### Soal 1
- Menyimpan argumen input
```c
int num[1000];
for(i=1; i<argc; i++){
    num[i-1] = atoi(argv[i]);
}
```
- Mengurutkan argumen-argumen input secara _ascending_
```c
int cmp (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

qsort(num,n,sizeof(int),cmp);
```

- Thread untuk penghitungan faktorial (penghitungan dilakukan secara paralel)
```c
int ind = -1;
void *thread() {
    int this = ++ind;

    int i, fact = 1;
    for (i=1; i<=num[this]; i++) {
        fact *= i;
    }

    sleep(num[this]);
    printf("%d! = %d\n", num[this], fact);
}
```
- Membuat dan menggabungkan _thread_
```c
int n = argc - 1;
pthread_t tid[n];

for(i=0; i<n; i++){
    pthread_create(&(tid[i]), NULL, &thread, NULL);
}

for(i=0; i<n; i++){
    pthread_join(tid[i], NULL);
}
```

### Soal 2
- 1 server hanya bisa terkoneksi dengan 1 client
```c
listen(server_fd, 1)
```
- Membedakan port penjual dengan port pembeli

Port penjual (client dan server)
```c
#define PORT 8000
```
Port pembeli (client dan server)
```c
#define PORT 8080
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
void *add() {
    while(1){        
        char buffer[1024] = {0};
        char *msg;
        int valread = read( new_socket , buffer, 1024);
        if(strcmp(buffer,"tambah")==0){
            *stok += 1;
        }
    }
}
```
- Pencetakan stok saat ini setiap 5 detik sekali pada server penjual
```c
void *now() {
    while(1){
        printf("stok saat ini: %d\n",*stok);
        sleep(5);
    }
}
```
- Penggunaan _thread_ pada server penjual sehingga kedua fungsi di atas dapat dijalankan secara bersamaan
```c
pthread_t tid1, tid2;

pthread_create(&(tid1), NULL, now, NULL);
pthread_create(&(tid2), NULL, add, NULL);
pthread_join(tid1, NULL);
pthread_join(tid2, NULL);
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
- Client melakukan input (dan akan mencetak status transaksi jika terkoneksi dengan server pembeli)
```c
while(1){
    char *msg;
    char buffer[1024] = {0};
    gets(msg);
    send(sock , msg , sizeof(msg) , 0 );

    // bagian ini hanya ditulis pada client beli
    if(strcmp(msg,"beli")==0){
        int valread = read( sock , buffer, sizeof(buffer));
        printf("transaksi ");
        if(strcmp(buffer,"0")==0) printf("gagal\n");
        else printf("berhasil\n");
    }

}
```

### Soal 3
- Awal program
```c
int agmal_WakeUp_Status = 0;
int iraj_Spirit_Status = 100;
```
- Fitur All Status
```c
printf("Agmal WakeUp_Status = %d\nIraj Spirit_Status = %d\n", agmal_WakeUp_Status, iraj_Spirit_Status);
```
- Fitur “Agmal Ayo Bangun”
```c
cmd_agmal = -1;
strike++;
agmal_WakeUp_Status += 15;
```
- Fitur “Iraj Ayo Tidur”
```c
cmd_iraj = -1;
strike++;
iraj_Spirit_Status -= 20;
```
- Jika fitur “Agmal Ayo Bangun” dijalankan sebanyak 3 kali,
```c
if (strike >= 3)
{
    while (cmd_iraj > 0);
    if(cmd_iraj == -1){
        cmd_iraj = 2;
        strike = 0;
    }
}
```

Maka fitur “Iraj Ayo Tidur” tidak bisa dijalankan selama 10 detik.
```c
cmd_iraj = 0;
printf("Fitur Iraj Ayo Tidur disabled 10 s\n");
sleep(10);
cmd_iraj = -1;
```
- Jika fitur “Iraj Ayo Tidur” dijalankan sebanyak 3 kali,
```c
if (strike >= 3)
{
    while (cmd_agmal > 0);
    if(cmd_agmal == -1){
        cmd_agmal = 2;
        strike = 0;
    }
}
```

Maka fitur “Agmal Ayo Bangun” tidak bisa dijalankan selama 10 detik.
```c
cmd_agmal = 0;
printf("Agmal Ayo Bangun disabled 10 s\n");
sleep(10);
cmd_agmal = -1;
```
- Program berhenti

WakeUp_Status Agmal >= 100
```c
if(agmal_WakeUp_Status >= 100){
    printf("Agmal Terbangun,mereka bangun pagi dan berolahraga\n");
    exit(0);
}
```

Spirit_Status Iraj <= 0
```c
if(iraj_Spirit_Status <= 0){
    printf("Iraj ikut tidur, dan bangun kesiangan bersama Agmal\n");
    exit(0);
}
```
- Penggunaan _thread_
```c
pthread_create(&agmalt, NULL, &agmal, NULL);
pthread_create(&irajt, NULL, &iraj, NULL);
```
- Membaca input berupa _string_
```c
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
}
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
- Memberi nama pada monster
```c
char name[100];
printf("Nama monster: ");
scanf("%s",name);
```
- Hunger status

Nilai awal 200 dan akan berkurang 5 tiap 10 detik.
```c
int monster_hunger = 200;
while(1){
    sleep(1);
    if (c % 10 == 0)
    {
        //per 10 detik
        monster_hunger -= 5;
    }
}
```

Hunger status bertambah 15 apabila pemain memberi makan kepada monster.
```c
monster_hunger += 15;
```
- Hygiene status

Nilai awal 100 dan akan berkurang 10 tiap 30 detik.
```c
int monster_hygine = 100;
while(1){
    sleep(1);
    if (c % 30 == 0)
    {
        //per 30 detik
        monster_hygine -= 10;
    }
}
```

Hygiene status bertambah 30 apabila pemain memandikan monster.
```c
monster_hygine += 30;
```

Pemain dapat memandikan monster setiap 20 detik.
```c
while(1){
    sleep(1);
    if (bath_remaining < 20)
    {
        bath_remaining++;
    }
}

if (bath_remaining >= 20)
{
    monster_hygine += 30;
    bath_remaining = 0;
    printf("Bathing!\n");
}
```
- Health status

Nilai awal 100 dan akan berkurang 5 tiap 10 detik ketika monster dalam keadaan standby.
```c
int monster_hygine = 100;
while(1){
    sleep(1);
    if (c % 10 == 0)
    {
        //per 10 detik
        monster_hygine -= 5;
    }
}
```
- Fitur battle
```c
int a = (int)getchar() - 48;
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
    monster_health -= 20;
    printf("NPC: ATTACK!!!\n");
}
else if (a == 2)
{
    printf("Monster: Later lah bro!!!\n");
    view = 0;
    f_stop = 0;
}
```
- Fitur shop (pembeli)

Mengecek stok makanan yang ada di toko.
```c
while(1){
    system("clear");
    printf("Shop Mode\n");
    printf("Shop food stock : %d\n", *store_food);
    printf("Your food stock : %d\n", food_stock);
    printf("Choices\n[1] Buy\n[2] Back\n");
    sleep(1);
}
```

Jika stok ada, pembeli dapat membeli makanan.
```c
int a = (int)getchar() - 48;
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
```
- Fitur shop (penjual)

Mengecek stok makanan yang ada di toko.
```c
while (1)
{
    system("clear");
    printf("Shop\n");
    printf("Food stock : %d\n", *store_food);
    printf("Choices\n[1] Restock\n[2] Exit\n");
}
```

Penjual dapat menambah stok makanan.
```c
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
```
- Mendeteksi input berupa key press.
```c
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
```
- Scene program

Hanya menampilkan status detik ini menggunakan `system("clear")`.
```c
void *display()
{
    while (1)
    {
        system("clear");
        printf("Nama monster: %s\n", name);
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
```
