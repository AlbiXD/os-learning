static volatile unsigned short *const vga = (volatile unsigned short *) 0xB8000;


void clear_screen();

void bootmain(void){
    clear_screen();
    vga[0] = (0xF << 8) | 'A';
    
    // Load in the first part of the kernel
    // Parse elf
    // Pass control to kernel

    while(1);
}

void clear_screen(){
    for(int i = 0; i < 80 * 25; i++){
        vga[i] = 0;
    }
}



