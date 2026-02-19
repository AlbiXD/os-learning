
void main(void){
    volatile unsigned short* vga = (unsigned short*)0xB8000;

    vga[0] = (0x0F << 8) | 'H';
    vga[1] = (0x0F << 8) | 'e';
    vga[2] = (0x0F << 8) | 'l';
    vga[3] = (0x0F << 8) | 'l';
    vga[4] = (0x0F << 8) | 'o';

    for(;;);
}