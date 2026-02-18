#include "../header/disk.h"
#include "../header/elf.h"
#include "../header/tools.h"
static volatile unsigned short *const vga = (volatile unsigned short *)0xB8000;

void clear_screen();
void bootmain(void)
{
    clear_screen();
    // Requests the HDD to read the first 4096 bytes of the kernel ELF file
    outb(ATA_SECTOR_COUNT, 8);
    outb(ATA_LBA_LOW, 2);
    outb(ATA_LBA_MID, 0);
    outb(ATA_LBA_HIGH, 0);
    outb(ATA_PRIMARY_DRIVE_HEAD, 0xE0 | ((2 >> 24) & 0x0F));
    outb(ATA_COMMAND_STATUS, 0x20);

    // Temporary location to hold the kernel headers
    uint16_t *kernel_header = (uint16_t *)0x8000;

    // Read from HDD Data Register into memory address 0x5E8
    uint16_t *p = kernel_header;
    if (ata_wait_drq() < 0)
        while (1)
            ;
    for (int i = 0; i < 2048; i++)
        *p++ = inw(ATA_DATA);

    // Parse the kernel files
    struct elfhdr *elf = (struct elfhdr *)kernel_header;
    if (elf->magic != ELF_MAGIC)
        while (1)
            ;

    struct proghdr *program_header = (struct proghdr *)((uint8_t *)kernel_header + elf->phoff);

    // We must load each entry of the program header
    // elf->phnum # number of entries in the program header
    // elf->phentsize size of one program table entry
    // elf->phoff points to the offset where program header starts

    // for each program table entry we will load segments into memory? then call entry()
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

    uint32_t lba = 2 + header_sectors; // start right AFTER the 4096 bytes you already read

    uint16_t *elf_ptr = (uint16_t *)KERNEL_TEMP;

    // We aren't doing that at the moment
    //  Read in 256 chunks and load into memory logic...?

    // sectors = 553 - 256
    while (remaining)
    {

        if (remaining <= 256)
        {
            // polling for drive to be ready
            if (ata_wait_drq() < 0)
                while (1)
                    ;
            // hdd is ready lets issue a read
            ata_issue_read28(remaining, lba);

            // polling for drive to say you can read
            if (ata_wait_drq() < 0)
                while (1)
                    ;
            // data is ready to be put in memory

            for (int s = 0; s < remaining; s++)
            {
                if (ata_wait_drq() < 0)
                    while (1)
                        ;
                for (int i = 0; i < 256; i++)
                {
                    *elf_ptr++ = inw(ATA_DATA);
                }
            }

            lba += remaining;
            remaining = 0;
        }
        else
        {
            // Issue read for 256 sectors
            ata_issue_read28(256, lba);

            // Read EXACTLY 256 sectors (not 'remaining')
            for (int s = 0; s < 256; s++)
            {
                if (ata_wait_drq() < 0)
                    while (1)
                        ;

                for (int i = 0; i < 256; i++)
                {
                    *elf_ptr++ = inw(ATA_DATA);
                }
            }

            lba += 256;
            remaining -= 256;
        }
    }

    uint8_t *ep = (uint8_t *)KERNEL_TEMP;
    // We know each program header entry is 20h bytes
    ptr = program_header;

    // Now

    // READ THE SECTORS

    // void (*entry)(void) = (void (*)(void))elf->entry;
    // entry();

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
