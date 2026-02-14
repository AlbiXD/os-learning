# os-learning
<img width="716" height="457" alt="Screenshot_from_2026-02-14_02-32-06" src="https://github.com/user-attachments/assets/c95f077c-3076-4791-9bfb-c6740a8c93a4" />
<p> Exploring low level system programming on 32 bit x86 to better understand how computers handle booting, memory, and hardware interaction. </p>

## Build & Run
<p>In order to compile this you will need to set up the cross compiler toolchain that OSDev wiki recommends</p>

- https://wiki.osdev.org/GCC_Cross-Compiler

<p>Once the cross compiler you can compile with</p>

```
make
```

<p> In order to actually load it and simulate the OS you will need qemu, on Debian/Ubuntu linux you can just run </p>

```
sudo apt install qemu-system
``` 
<p> Then finally run the simulator with </p>

```
make output
``` 


## Notes
- https://docs.google.com/document/d/e/2PACX-1vQJCw4J2pqfRT8Nx3MKygpaCR207wlwMCYMSm3vlpFCwhH9OWkJGUleK1QsQUatDJ8rFJBRUh6MCOyc/pub


## Resources
- https://users.cs.utah.edu/~aburtsev/238P/2020spring/
- https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html
- https://wiki.osdev.org/Expanded_Main_Page
- https://github.com/mit-pdos/xv6-public/
- https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf
