#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

#define SYS_MY_GET_THREAD_INFO 549

struct my_thread_info_record {
    unsigned long pid;
    unsigned long tgid;
    unsigned long process_descriptor_address;
    unsigned long kernel_mode_stack_address;
    unsigned long pgd_table_address;
};

int main() {
    struct my_thread_info_record data;

    if (syscall(SYS_MY_GET_THREAD_INFO, &data)) {
        printf("pid: %lu\n", data.pid);
        printf("tgid: %lu\n", data.tgid);
        printf("process descriptor address: 0x%lx\n", data.process_descriptor_address);
        printf("kernel mode stack address: 0x%lx\n", data.kernel_mode_stack_address);
        printf("pgd table address: 0x%lx\n", data.pgd_table_address);
    } else {
        printf("Cannot execute the new system call correctly\n");
    }
    return 0;
}

