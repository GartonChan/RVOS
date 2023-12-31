# RVOS
## dev route
### 1. Hello RVOS
In this section, I have simulate the boot process according to the course and QEMU platform. Starting from `start.S`, firstly it parks the harts except hart-0, then it initializes the stacks and jumps to `start_kernel()` for loading the kernel image.
In `start_kernel()`, load a kernel with busy-waiting (just as a simple demo).
When the kernel is loaded into memory and running, it is blocked in the while(1).

### 2. UART
In this section, It introduces the `Universal Asynchronous Receiver/Transmitter(UART)`. With the help of UART, it is much helpful to print log for debugging. 
Learn about the universal protocol, then implement uart_putc, uart_puts by understanding the reference and manual.
Moreover, implement `printf()` for better usage.
Notice: `<stddef.h>` and `<stdarg.h>` are included here.

### 3. Memory Management
Manage the Heap Memory by maintaining a seqList structure with reserved pages (8 pages are reserved for storing the metadata here) in the front of the heap. And the other pages are used to allocate. 
The seqList structure only contains flags, use `bit-0` for indicating its availability and `bit-1` for indicating whether it is the last page.

### 4. Multitask
With the understanding of the concept of `context` and getting familiar with the process of `context switch`.
Then maintain a context structure and implement its saving and restoring procedure for context switch. Finally implements task management and multitask based on context switch.

### 5. Trap
In this section, some necessary m-mode CSRs such as `mtvec`, `mepc`, `mcause`, `mtval` and `mstatus` are introduced. 

Learn the `trap flow` of Trap in RISC-V.
In the course, the process include four parts: `trap_init`, `trap_top_half`, `trap_low_half` and finally return from trap.
In the all flow, top_half is automatically handled by hardware and low_half is what the software should do. In brief, to save ctx, then call trap_handler, restore ctx after handler and finally return to the state before trap.

### 6. External Interrupt
In this section, it firstly introduces the `Platform-Level Interrupt Controllor (PLIC)` to handle multiple External Interrupts.
With the memory-map strategy, PLIC_BASE is set to 0x0C000000 according to QEMU platform. 
Each interrupt is assigned with a source id, that is `Interrupt (source) ID`, using for setting the Priority, Pending attributions and handling the corresponding interrupt.
For each Hart, it has `Enable`, `Threshold`, `Claim/Complete` Registers insides.
Initialize the plic, and the claim/complete operation should be called before/after the external interrupts.
Implement uart_getc and uart_irq and external_exception_handler.
When there is an external exception raised by the keyboard, it will trap into the `trap_handler()`, then the trap_handler will call `external_interrupt_handler()` according to the trap information.

### 7. Hardware Timer
Introduce `Core Local INTerruptor (CLINT)` to RVOS. By configuring the CLINT, hardware timer can be easily implemented. So in this section, what need to do is just to cofigure the CLINT to enble timer and implement such a `timer_interrupt_handler`.
When if `time >= timecmp` in one hart, there will be an timer_interrupt in this hart, and `trap_handler` will be called then branch to `timer_interrupt_handler`. 

### 8. Preemptive
In order to save the pc value of a task as its part of context, add a `pc` field in the `struct context`. Then the process of context switching need to be reorganize and rewrite for the sake of introducing a timer interrupt to schedule for tasks. 
First, save the context when trap occurs, then call the schedule in the timer_interrupt_handler() and finally it will return to whatever the task should do.

### 9. MutexAndSync
With the understanding of mutex and synchronization, for example, `concorrency` and `critical region.` it is necessary to introduce some helpful mechanism such as lock, mutex, semaphore, conditional variable and so on, in order to make sure the system can work well as designers and developers expect. 
Here we simply implement one kind of unsleeping locks, called `spin-lock`, with a busy-waiting insides. However, an undesirable side effect called deadlock may occur as we introduce and use the lock mechanism.

### 10. Software Timer
Based on the `Hardware Timer`, We can further implement the `Software Timer`, which can break out the limitation of number, though with a worse degree of precision. The precision level must be multiples of the `tick` (provided by hardware timer). The timers can be classified into two kind according to how they works: triggle only single time or periodically. The contexts are devided into two kind: `Interrupt context` and `Task context`.
As for the implementation, define the `timer_t` including handler, args and timeout_tick. Then allocate the fixed-length timer array and initialize it. Provide `timer_create()` and `timer_deldete()` for users as the interface to use software timer. Each time it raises an hardware timer interrupt, the handler will check the software timer setting, if there is any timer time out, then the coresponding handler will be called.

### 11. Syscall
For the purpose of protections, we need to introduce some approachs or mechanisms to make the user task/process not simply do whatever it wants. So, we introduce two new processor modes, known as user mode and kernel mode. Code runs in user mode is restricted in what it can do, while code runs in kernel mode can do what it likes. Illegal operations would result in processor raising an exception, and the OS would then likely kill the process.

The RISC-V Manual Part II indicates that "The simplest RISC-V implementations may provide only M-mode, though this will provide no protection against incorrect or malicious application code. Many RISC-V implementations will also support at least U-mode to protect the rest of the system from application code. S-mode can be added to provide isolation between a supervisor-level operating system and the SEE." 
For the detail of such a protection and isolation mechanism, "A hart normally runs application code in U-mode until some trap (e.g., a supervisor call or a timer interrupt) forces a switch to a trap handler, which usually runs in a more privileged mode. The hart will then execute the trap handler, which will eventually resume execution at or after the original trapped instruction in U-mode. Traps that increase privilege level are termed vertical traps, while traps that remain at the same privilege level are termed horizontal traps."

### Reference:
1. RISC-V Manual Volume I: Unprivileged ISA
2. RISC-V Manual Volume II: Privileged Architecture
3. OperatingSystem-ThreeEasyPieces
4. https://www.bilibili.com/video/BV1Q5411w7z5
5. https://github.com/plctlab/riscv-operating-system-mooc 