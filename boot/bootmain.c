#include "../header/disk.h"

static volatile unsigned short *const vga = (volatile unsigned short *) 0xB8000;


void clear_screen();

void bootmain(void){
    clear_screen();

    unsigned char status;
    //Fuck it lets load kernel into memory 0x100000

    
    outb(ATA_SECTOR_COUNT, 8);
    outb(ATA_LBA_LOW, 2);
    outb(ATA_LBA_MID, 0);
    outb(ATA_LBA_HIGH, 0);
    outb(ATA_PRIMARY_DRIVE_HEAD, 0xE0 | ((2 >> 24) & 0x0F));
    outb(ATA_COMMAND_STATUS, 0x20);
        
    uint16_t *kernel_header = (uint16_t *)0x5E8;


    // Kernel header loaded time for parsing
    uint16_t *p = kernel_header;
    for(int i = 0; i < 2048; i++){
        *p++ = inw(ATA_DATA);

    }

    while(1);
}

void clear_screen(){
    for(int i = 0; i < 80 * 25; i++){
        vga[i] = 0;
    }
}



