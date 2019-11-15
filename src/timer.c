#include "timer.h"
#include "interrupts.h"
#include "util.h"
#include <stdint.h>
static timer_registers_t *timer_regs;

static void timer_irq_handler(void) {
  uart_puts("timeout :/ \n");
  timer_set(3000000);
}

static void timer_irq_clearer(void) {
  timer_regs->control.timer1_matched = 1;
}


void timer_init(void) {
  timer_regs = (timer_registers_t *) 0x40000040;//SYSTEM_TIMER_BASE;
  register_irq_handler(SYSTEM_TIMER, timer_irq_handler, timer_irq_clearer);
}

void timer_set(uint32_t usecs) {
  timer_regs->timer1 = timer_regs->counter_low + usecs;
}
