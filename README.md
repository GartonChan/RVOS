# RVOS
## dev route
### 1. Hello RVOS
    In this section, I have simulate the boot process according to the course and QEMU platform. Starting from start.S, firstly it parks the harts except hart-0, then it initializes the stacks and jumps to `start_kernel()` for loading the kernel image.
    In `start_kernel()`, load a kernel with busy-waiting (just as a simple demo).
    When the kernel is loaded into memory and running, it is blocked in the while(1).

### 2. UART
    In this section, It introduces the `Universal Asynchronous Receiver/Transmitter(UART)`. With the help of UART, it is much helpful to print log for debugging. 
    Learn about the universal protocol, then implement uart_putc, uart_puts by understanding the reference and manual.
    Moreover, implement `printf()` for better usage.
    Notice: <stddef.h> and <stdarg.h> are included here.

### 3. Memory Management
    Manage the Heap Memory by maintaining a seqList structure with reserved pages (8 pages are reserved for storing the metadata here) in the front of the heap. And the other pages are used to allocate. 
    The seqList structure only contains flags, use bit-0 for indicating its availability and bit-1 for indicating whether it is the last page.

### 4. Multitask
    With the understanding of the concept of `context` and getting familiar with the process of `context switch`. 
    Then maintain a context structure and implement its saving and restoring procedure for context switch. Finally implements task management and multitask based on context switch.

### 5. Trap
    In this section, some necessary m-mode CSRs such as mtvec, mepc, mcause, mtval and mstatus are introduced. Learn the processing flow of Trap in RISC-V.
    In the course, the process include four parts: `trap_init`, `trap_top_half`, `trap_low_half` and finally return from trap.
    In the processing flow, top_half is automatically handled by hardware and low_half is what the software should do. In brief, to save ctx, then call trap_handler, restore ctx after handler and finally return to the state before trap.

### 6. External Interrupt
    In this section, it firstly introduces the `Platform-Level Interrupt Controllor (PLIC)` to handle multiple External Interrupts.
    With the memory-map strategy, PLIC_BASE is set to 0x0C000000 according to QEMU platform. 
    Each interrupt is assigned with a source id, that is Interrupt ID, using for setting the Priority, Pending attributions and handling the corresponding interrupt.
    For each Hart, it has Enable, Threshold, Claim/Complete Registers insides.
    Initialize the plic, and the claim/complete operation should be called before/after the external interrupts.
    Implement uart_getc and uart_irq and external_exception_handler.
    When there is an external exception raised by the keyboard, it will trap into the `trap_handler`, then the trap_handler will call `external_interrupt_handler` according to the trap information.