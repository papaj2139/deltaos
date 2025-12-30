unsigned int strlen(const char *str) {
    unsigned int size = 0;
    while (*str++) size++;
    return size;
}

void puts(const char *str) {
    asm volatile(
        "mov $4, %%rax\n\t"
        "mov %0, %%rdi\n\t"
        "mov %1, %%rsi\n\t"
        "syscall\n\t"
        :
        : "r"(str), "r"((unsigned long)(strlen(str)))
        : "rax","rdi","rsi","rcx","r11","memory"
    );
}

__attribute__((noreturn)) void _start(void) {
    /* write hello */
    puts("[user] hello from userspace!\n");

    /* getpid (result ignored) */
    asm volatile("mov $1, %%rax\n\tsyscall\n\t" ::: "rax","rcx","r11","memory");

    /* write done */
    puts("[user] syscall test complete, exiting\n");

    /* exit(0) */
    asm volatile(
        "mov $0, %%rdi\n\t"
        "mov $0, %%rax\n\t"
        "syscall\n\t"
        :
        :
        : "rax","rdi","rcx","r11","memory"
    );

    // unreachable hopefully
    for(;;) asm volatile("hlt");

}
