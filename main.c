static volatile unsigned short *const vga = (volatile unsigned *) 0xB8000;


void clear_screen() {
	for (int i = 0; i < 80 * 25; i++) {
		vga[i] = (0x0F << 8) | 'A';  // white space
	}
}
void bootmain(void){
    clear_screen();
    vga[0] = (0x0F << 8) | 'A';

    while(1);
}

