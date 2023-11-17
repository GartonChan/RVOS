# RVOS
## dev route
### 1. Hello_RVOS
    Starting from start.S, firstly park the harts except hart0, then initialize the stacks and jump to start_kernel().
    In start_kernel(), load a kernel with busy-waiting (just simple demo).

### 2. UART
    Using uart for debugging, leart about the UART protocol, then implement uart_c, uart_s by understanding the reference and manual.
    Moreover, implement printf() for better usage.
    Here, <stddef.h> and <stdarg.h> are included.

### 3. Memory_Management
    Manage the heap memory by maintaining a seqList structure in reserved pages(8 pages here) in the front of the heap. And the other pages are used to allocate. 
    The seqList structure only contains flags, use bit0 for indicating available or not and bit1 for indicating whether is the last page.

### 4. Multitask
    Learnt about the concept of context and understand the process of switching. Then maintain a context structure, and implement its saving and restoring procedure in order to schedule to another task from start_kernel(). Finally add the number of tasks and implement multitask.

### 5. Trap
    Learnt some necessary m-mode CSRs such as mtvec, mepc, mcause, mtval and mstatus. Learnt the processing flow of Trap in RISC-V including trap_init, trap_top_half, trap_low_half and finally return from trap. In this process, top_half is automatically operated by hardware and low_half is what the software should do. In brief, save ctx, call trap_handler then restore ctx and finally return to the state before trap.