#include <stdint.h>

#define ELF_MAGIC 0x464C457FU
#define KERNEL_TEMP 0x8000
struct __attribute__((packed)) elfhdr {
    uint32_t magic;        
    uint8_t  ident[12];    
    uint16_t type;
    uint16_t machine;
    uint32_t version;      
    uint32_t entry;
    uint32_t phoff;
    uint32_t shoff;
    uint32_t flags;
    uint16_t hsize;
    uint16_t phentsize;
    uint16_t phnum;
    uint16_t shentsize;
    uint16_t shnum;
    uint16_t shstrndx;
};

struct __attribute__((packed)) proghdr {
    uint32_t type;
    uint32_t off;
    uint32_t vaddr;
    uint32_t paddr;
    uint32_t filesz;
    uint32_t memsz;
    uint32_t flags;
    uint32_t align;
};

#define ELF_PROG_LOAD           1

// Flag bits for Proghdr flags
#define ELF_PROG_FLAG_EXEC      1
#define ELF_PROG_FLAG_WRITE     2
#define ELF_PROG_FLAG_READ      4