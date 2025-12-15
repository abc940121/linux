struct my_thread_info_record {
    unsigned long pid;
    unsigned long tgid;
    unsigned long process_descriptor_address;
    unsigned long kernel_mode_stack_address;
    unsigned long pgd_table_address;
};

SYSCALL_DEFINE1(my_get_thread_kernel_info, struct my_thread_info_record __user *, user_record){

    struct my_thread_info_record k_record;
    struct task_struct *task = current;

    k_record.pid = task->pid;
    k_record.tgid = task->tgid;
    k_record.process_descriptor_address = (unsigned long)task;
    k_record.kernel_mode_stack_address = (unsigned long)task->stack;

    if (task->mm){
        k_record.pgd_table_address = (unsigned long)task->mm->pgd;
    }else if (task->active_mm){
        k_record.pgd_table_address = (unsigned long)task->active_mm->pgd;
    }else{
        k_record.pgd_table_address = 0;
    }

    if(copy_to_user(user_record, &k_record, sizeof(struct my_thread_info_record))){
        return 0;
    }
    return 1;
}
