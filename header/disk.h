/*
    ATA primary bus I/O ports (used to talk to the disk)
    Modern PC's talk over SATA/AHCI/NVMe this is legacy Hard Disk given by the QEMU emulator by default
*/
#include <stdint.h>

#define SECTOR_SIZE 512
#define ATA_DATA 0x1F0
#define ATA_ERROR 0x1F1
#define ATA_SECTOR_COUNT 0x1F2
#define ATA_LBA_LOW 0x1F3
#define ATA_LBA_MID 0x1F4
#define ATA_LBA_HIGH 0x1F5
#define ATA_LBA_HIGH 0x1F5
#define ATA_PRIMARY_DRIVE_HEAD 0x1F6
#define ATA_COMMAND_STATUS 0x1F7
#define ATA_CONTROL 0x3F6

#define BSY 0x80
#define DRQ 0x08
#define ERR 0

static inline void outb(unsigned short port, unsigned char value)
{
    __asm__ volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint16_t inw(uint16_t port)
{
    uint16_t value;
    __asm__ volatile ("inw %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static inline uint8_t inb(unsigned short port)
{
    uint8_t value;
    __asm__ volatile("inb %1, %0" : "=a"(value): "Nd"(port));
    return value;
}

static inline uint8_t ata_read_status(){
    return inb(0x1F7);
}

static inline int ata_wait_drq(void) {
    uint8_t status;
    do {
        status = ata_read_status();
        if (status & ERR) return -1;    
    } while (status & BSY || !(status & DRQ));
    return 0; 
}

static inline void  ata_issue_read28(uint8_t count, uint32_t lba){
    outb(ATA_PRIMARY_DRIVE_HEAD, 0xE0 | ((lba >> 24) & 0x0F));
    outb(ATA_SECTOR_COUNT, count);
    outb(ATA_LBA_LOW,  (uint8_t)(lba));
    outb(ATA_LBA_MID,  (uint8_t)(lba >>  8));
    outb(ATA_LBA_HIGH, (uint8_t)(lba >> 16));
    outb(ATA_COMMAND_STATUS, 0x20);
}
