#include <stdint.h>

#include "interrupts.h"
#include "util.h"


static interrupt_handler_f handlers[NUM_IRQS];
static interrupt_clearer_f clearers[NUM_IRQS];


static interrupt_registers_t *interrupt_regs;
extern void move_exception_vector(void);

void interrupts_init(void) {
  interrupt_regs = (interrupt_registers_t *)INTERRUPTS_PENDING;
  mem_set(handlers, sizeof(interrupt_handler_f) * NUM_IRQS);
  mem_set(clearers, sizeof(interrupt_clearer_f) * NUM_IRQS);
  interrupt_regs->irq_basic_disable = 0xffffffff; // disable all interrupts
  interrupt_regs->irq_gpu_disable1 = 0xffffffff;
  interrupt_regs->irq_gpu_disable2 = 0xffffffff;
  move_exception_vector();
  ENABLE_INTERRUPTS();
}



void register_irq_handler(irq_number_t irq_num, interrupt_handler_f handler, interrupt_clearer_f clearer) {
    int irq_pos;
    handlers[irq_num] = handler;
    clearers[irq_num] = clearer;
    if (IRQ_IS_BASIC(irq_num)) {
        irq_pos = irq_num - 64;
        interrupt_regs->irq_basic_enable |= (1 << irq_pos);
    }
    else if (IRQ_IS_GPU2(irq_num)) {
        irq_pos = irq_num - 32;
        interrupt_regs->irq_gpu_enable2 |= (1 << irq_pos);
    }
    else if (IRQ_IS_GPU1(irq_num)) {
        irq_pos = irq_num;
        interrupt_regs->irq_gpu_enable1 |= (1 << irq_pos);
    }
}




void irq_handler(){ //pas de attribute parce que cf assembleur askip
  int j;
    for (j = 0; j < NUM_IRQS; j++) {
        // If the interrupt is pending and there is a handler, run the handler
        if (IRQ_IS_PENDING(interrupt_regs, j)  && (handlers[j] != 0)) {
            clearers[j]();
            ENABLE_INTERRUPTS();
            handlers[j]();
            DISABLE_INTERRUPTS();
            return;
        }
    }
  
}


void __attribute__((interrupt ("FIQ"))) fiq_handler() {
  uart_puts("FIQ INTERRUPT\n");
  while(1);
}

void __attribute__((interrupt ("ABORT"))) reset_handler() {
  uart_puts("RESET\n");
  while(1);
}

void __attribute__((interrupt ("UNDEF"))) undef_instr() {
  uart_puts("UNDEFINED INSTRUCTION");
  while(1);
}

void __attribute__((interrupt ("SWI"))) software_interrupt() {
  uart_puts("SOFTWARE INTERRUPT");
  while(1);
}

void __attribute__((interrupt ("ABORT"))) prefetch_abort() {
  uart_puts("PREFETCH ABORT");
  while(1);
}

void __attribute__((interrupt ("ABORT"))) data_abort() {
  uart_puts("DATA ABORT");
  while(1);
}

