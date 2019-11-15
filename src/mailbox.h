#define MAILBOX_BASE 0x3F00B880

void mailbox_write(uint32_t box,uint32_t data);

uint32_t mailbox_read(uint32_t box);
