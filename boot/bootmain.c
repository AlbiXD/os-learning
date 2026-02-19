#include "../header/disk.h"
#include "../header/elf.h"
#include "../header/tools.h"
static volatile unsigned short *const vga = (volatile unsigned short *)0xB8000;

void clear_screen();
void bootmain(void)
{
    clear_screen();
    // Requests the HDD to read the first 4096 bytes of the kernel ELF file
    waitdisk();
    outb(ATA_SECTOR_COUNT, 8);
    outb(ATA_LBA_LOW, 3);
    outb(ATA_LBA_MID, 0);
    outb(ATA_LBA_HIGH, 0);
    outb(ATA_PRIMARY_DRIVE_HEAD, 0xE0 | ((3 >> 24) & 0x0F));
    outb(ATA_COMMAND_STATUS, 0x20);

    // Temporary location to hold the kernel headers
    uint16_t *kernel_header = (uint16_t *)KERNEL_TEMP;

    // Read from HDD Data Register into memory address 0x5E8
    uint16_t *p = kernel_header;

    for (int i = 0; i < 8; i++){
        ata_wait_drq();
        for(int s = 0; s < 256; s++)
            *p++ = inw(ATA_DATA);
    }

    // Parse the kernel files
    struct elfhdr *elf = (struct elfhdr *)kernel_header;
    if (elf->magic != ELF_MAGIC)
        while (1)
            ;

    struct proghdr *program_header = (struct proghdr *)((uint8_t *)kernel_header + elf->phoff);


    uint64_t total_bytes_to_read = 0;
    struct proghdr *ptr = program_header;

    for (unsigned int i = 0; i < elf->phnum; i++)
    {
        if (ptr->type == ELF_PROG_LOAD)
        {
            uint64_t temp = (ptr->filesz + ptr->off);
            if (temp > total_bytes_to_read)
                total_bytes_to_read = temp;
        }
        ptr++;
    }

    uint32_t sectors = (total_bytes_to_read + SECTOR_SIZE - 1) / SECTOR_SIZE;

    uint32_t header_sectors = 8; // 4096 / 512
    uint32_t total_sectors = sectors;

    uint32_t remaining = 0;
    if (total_sectors > header_sectors)
        remaining = total_sectors - header_sectors;

    uint32_t lba = 3 + header_sectors; // start right AFTER the 4096 bytes you already read

    uint16_t *pointer = (uint16_t *)0x10000;


    
    while (remaining)
    {
        uint32_t chunk = remaining > 255 ? 256 : remaining;
        
        waitdisk();
        ata_issue_read28(chunk, lba);

        // read sector into disk
        for (uint32_t s = 0; s < chunk; s++)
        {
            ata_wait_drq();
            for (int i = 0; i < 256; i++ )
            {
                *pointer++ = inw(ATA_DATA);

            }
        }

        lba += chunk;
        remaining -= chunk;
    }

    
    uint32_t reoffset = 0x1000;
    ptr = program_header;
    uint8_t* segment_ptr = 0;

    uint8_t *tempKernel = (uint8_t *) 0x10000;
    for(uint16_t pentry = 0; pentry < elf->phnum; pentry++){
        if(ptr->type == ELF_PROG_LOAD){
            uint32_t offset = (ptr->off) - reoffset;
            uint8_t *address =(uint8_t*) ptr->vaddr;
            uint32_t size = ptr->filesz;

            for(uint32_t s =  0 ; s < size; s++){
                // *address++ = *((((uint8_t * ) 0x10000) + offset)++); 

                *address++ = *(tempKernel+offset+s);
            }
        }

        ptr++;
    }

    void (*entry_pointer)(void);

    entry_pointer = ( void (*)(void)) elf->entry;

    entry_pointer();

    while (1)
        ;
}

void clear_screen()
{
    for (int i = 0; i < 80 * 25; i++)
    {
        vga[i] = 0;
    }
}
