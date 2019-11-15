	.section ".text"

	.global move_exception_vector

exception_vector:
	ldr pc, reset_handler_abs_addr
	ldr pc, undef_instr_abs_addr
	ldr pc, software_interrupt_abs_addr
	ldr pc, prefetch_abort_abs_addr
	ldr pc, data_abort_abs_addr
	nop				 //reserved
	ldr pc, irq_handler_abs_addr
	ldr pc, fiq_handler_abs_addr

reset_handler_abs_addr:			.word reset_handler
undef_instr_abs_addr:			.word undef_instr
software_interrupt_abs_addr:	 	.word software_interrupt
prefetch_abort_abs_addr:		.word prefetch_abort
data_abort_abs_addr:	 		.word data_abort
irq_handler_abs_addr:	 		.word irq_handler
fiq_handler_abs_addr:			.word fiq_handler

move_exception_vector:
	push	 {r4, r5, r6, r7, r8, r9}
	ldr	 r0, =exception_vector
	mov 	r1, #0x0000
	ldmia 	r0!, {r2, r3, r4, r5, r6, r7, r8, r9}
	stmia 	r1!, {r2, r3, r4, r5, r6, r7, r8, r9}
	ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8}
	stmia   r1!,{r2, r3, r4, r5, r6, r7, r8}
	pop     {r4, r5, r6, r7, r8, r9}
	blx     lr

irq_handler_asm_wrapper:
	sub     lr, lr, #4
	srsdb   sp!, #0x13
	cpsid   if, #0x13
	push    {r0-r3, r12, lr}
	and     r1, sp, #4
	sub     sp, sp, r1
	push    {r1}
	bl      irq_handler
	pop     {r1}
	add     sp, sp, r1
	pop     {r0-r3, r12, lr}
	rfeia   sp!
