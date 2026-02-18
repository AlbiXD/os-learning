static inline void* get_eip() {
    void* addr;
    __asm__ volatile (
        "call 1f\n"
        "1: pop %0\n"
        : "=r"(addr)
    );
    return addr;
}
