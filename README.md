# SoalShift_modul3_E01

### Soal 1

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
### Soal 2
```c

```
### Soal 3
```c

```
### Soal 4
```c

```
### Soal 5
```c

```
