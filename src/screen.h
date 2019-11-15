void* framebuffer_init(uint32_t width,uint32_t height,uint32_t depth);

void disp_char(int x,int y,int width,int height,char c,uint16_t* fb);

void disp_large_char(int x,int y,int size,int width,int height,char c,uint16_t* fb);

void disp_string(int x,int y,int width,int height,const char* str,uint16_t* fb);

void disp_large_string(int x,int y,int size,int width,int height,const char* str,uint16_t* fb);

void clear_screen(int width,int height,uint16_t* fb);
