typedef struct file
{
    char isFolder;
    char* name;
    struct file* next;
    struct file* prev;
    struct file* parent;
    void* addr;
    uint32_t size;
} file;

file* last(file* folder);

file* find(file* folder,const char* name);

void pwd(char* ans,const char* params);

void ls(char* ans,const char* params);

void touch(char* ans,const char* params);

void mkdir(char* ans,const char* params);

void cd(char* ans,const char* params);

void rm(char* ans,const char* params);

void rmdir(char* ans,const char* params);

void cat(char* ans,const char* params);

void write(char* ans,const char* params);
