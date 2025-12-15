#define _GNU_SOURCE  
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/syscall.h> 
#include <unistd.h>

#define SYS_MY_GET_THREAD_KERNEL_INFO 549

// 1. 定義通訊結構體
// 型別統一改為 unsigned long，以符合核心定義 (64-bit 系統)
struct my_thread_info_record {                  
    unsigned long pid;                          
    unsigned long tgid;                         
    unsigned long process_descriptor_address;   
    unsigned long kernel_mode_stack_address;    
    unsigned long pgd_table_address;            
};

// 2. 定義 Thread Local Storage (TLS) 結構
struct data_ {
    int id;
    char name[16];
};
typedef struct data_ sdata;

static __thread sdata tx;  

// 3. Helper Function
void hello(int pid)
{
    struct my_thread_info_record data; 

    long ret = syscall(SYS_MY_GET_THREAD_KERNEL_INFO, &data);

    if(ret != 0) 
    {
        printf("------------------------------------------------\n");
        printf("Thread Name: %s (TID from gettid: %d)\n", tx.name, tx.id);
        printf("  PID (from kernel) = %lu\n", data.pid);
        printf("  TGID (from kernel) = %lu\n", data.tgid);
        printf("  Process Descriptor Address = 0x%lx\n", data.process_descriptor_address);
        printf("  Kernel Mode Stack Address  = 0x%lx\n", data.kernel_mode_stack_address);
        printf("  PGD Table Address          = 0x%lx\n", data.pgd_table_address);
        printf("------------------------------------------------\n");
    }
    else {
        printf("Cannot execute the new system call correctly\n");
    }
}  

void *func1(void *arg)
{
    char *p = (char*) arg;
    int pid = syscall(SYS_gettid);
    
    // 設定 Thread Local Variable
    tx.id = pid;
    strcpy(tx.name, p);
    
    printf("I am thread with ID %d executing func1().\n", pid);
    hello(pid);
    
    while(1)
    {
        //printf("(%d)(%s)\n", tx.id, tx.name);
        sleep(1);
    }
}

void *func2(void *arg)
{
    char *p = (char*) arg;
    int pid = syscall(SYS_gettid);
    
    tx.id = pid;
    strcpy(tx.name, p);
    
    printf("I am thread with ID %d executing func2().\n", pid);
    hello(pid);
    
    while(1)
    {
        sleep(2);
    }
}

int main()
{ 
    pthread_t id[2];
    char p[2][16];
    
    strcpy(p[0], "Thread1");
    pthread_create(&id[0], NULL, func1, (void *)p[0]);
    
    strcpy(p[1], "Thread2");
    pthread_create(&id[1], NULL, func2, (void *)p[1]);

    int pid = syscall(SYS_gettid);
    tx.id = pid;
    strcpy(tx.name, "MAIN");
    
    printf("I am main thread with ID %d.\n", pid);
    hello(pid);
    
    while(1)
    {
        sleep(5);
    }
    
    return 0;
}
