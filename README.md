# os-learning
<p align="center">
  <img src="https://github.com/user-attachments/assets/5293bfc1-bdd5-4c71-9d05-dd78adc9d65f" width="600" />
  <br>
  <sub>Screenshot of just a few characters from the kernel</sub>
</p>

<p> Exploring low level system programming on 32 bit x86 to better understand how computers handle booting, memory, and hardware interaction. </p>

<p>At the moment I have parsed the kernel's elf files and loaded them into memory, my next phase will be to clean up and map the memory of the OS and make it less sloppy</p>

## Build & Run
<p>In order to compile this you will need to set up the cross compiler toolchain that OSDev wiki recommends</p>

- https://wiki.osdev.org/GCC_Cross-Compiler


<p> In order to actually load it and simulate the OS you will need qemu, on Debian/Ubuntu linux you can just run </p>

```
sudo apt install qemu-system
```

<p> Then finally run and compile the simulator with </p>

```
make output   # builds disk image + boots it in QEMU
``` 
## Memory Map (Boot)

This is the physical memory layout I’m using during boot (stage1 → stage2 → ELF load).

<p align="center">
  <img src="https://github.com/user-attachments/assets/3b4bb93f-32fb-4080-befa-c0521d0e6f14" width="260" />
</p>

**Key regions**
- `0x00000–0x003FF` IVT (real-mode interrupt vectors)
- `0x00400–0x004FF` BIOS Data Area
- `0x07C00` Boot Sector (stage 1)
- `0x08000` Stage 2 loader (`bootmain.bin`)
- `0x10000` `KERNEL_HEADER_TEMP` (ELF header / phdr scratch)
- `0x20000` `KERNEL_SEGMENT_TEMP` (temporary segment load buffer)
- `0x100000` `KERNEL_PT_LOADS` (kernel loaded at 1MB+)

## OS Roadmap (no particular order, just things I want to build)
- TTY Printing ✔
- GDT ✔
- IDT (WIP)
- Kernel Allocator
- Paging
- PIC (or APIC not sure which yet)
- Keyboard Queue
- File System
- Scheduler
- Shell

## Notes
- https://docs.google.com/document/d/e/2PACX-1vQJCw4J2pqfRT8Nx3MKygpaCR207wlwMCYMSm3vlpFCwhH9OWkJGUleK1QsQUatDJ8rFJBRUh6MCOyc/pub


## Resources
- https://users.cs.utah.edu/~aburtsev/238P/2020spring/
- https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html
- https://wiki.osdev.org/Expanded_Main_Page
- https://github.com/mit-pdos/xv6-public/
- https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf
