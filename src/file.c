#include <stddef.h>
#include <stdint.h>
#include "file.h"
#include "string.h"
#include "mem.h"

file root={.isFolder=1,.name="",.next=NULL,.prev=NULL,.parent=&root,.addr=NULL,.size=0};

file* dir=&root;

//Returns a pointer to the last file of the given folder
file* last(file* folder)
{
    file* f=(file*)folder->addr;
    if(f==NULL)
    {
        return NULL;
    }
    while(f->next!=NULL)
    {
        f=f->next;
    }
    return f;
}

//Returns a pointer to the file/folder with the given name, in the given folder (or NULL if it doesn't exist)
file* find(file* folder,const char* name)
{
    file* f=(file*)folder->addr;
    while(f!=NULL)
    {
        if(strcmp(f->name,name)==0)
        {
            return f;
        }
        f=f->next;
    }
    return NULL;
}

//The pwd command
void pwd(char* ans,const char* params)
{
    (void) params;
    strcpy(ans,"");
    file* f=dir;
    char old[420];
    while(f->parent!=f)
    {
        strcpy(old,ans);
        strcpy(ans,"/");
        append(ans,f->name);
        append(ans,old);
        f=f->parent;
    }
    if(strlen(ans)==0)
    {
        strcpy(ans,"/");
    }
}

//The ls command
void ls(char* ans,const char* params)
{
    (void) params;
    strcpy(ans,"");
    file* f=(file*)dir->addr;
    while(f!=NULL)
    {
        append(ans,f->name);
        append(ans," ");
        f=f->next;
    }
}

//Creates a file with the given name. Not quite the same as the unix command of the same name.
void touch(char* ans,const char* params)
{
    if(strlen(params)==0)
    {
        strcpy(ans,"Missing parameter : file name");
    }
    else
    {
        strcpy(ans,"");
        char* name=(char*)malloc(strlen(params)+1);
        strcpy(name,params);
        file* f=(file*)malloc(sizeof(file));
        f->isFolder=0;
        f->name=name;
        f->next=NULL;
        f->prev=last(dir);
        f->parent=dir;
        f->addr=NULL;
        f->size=0;
        if(f->prev==NULL)
        {
            dir->addr=f;
        }
        else
        {
            f->prev->next=f;
        }
    }
}

//The mkdir command
void mkdir(char* ans,const char* params)
{
    if(strlen(params)==0)
    {
        strcpy(ans,"Missing parameter : directory name");
    }
    else
    {
        strcpy(ans,"");
        char* name=(char*)malloc(strlen(params)+1);
        strcpy(name,params);
        file* f=(file*)malloc(sizeof(file));
        f->isFolder=1;
        f->name=name;
        f->next=NULL;
        f->prev=last(dir);
        f->parent=dir;
        f->addr=NULL;
        f->size=0;
        if(f->prev==NULL)
        {
            dir->addr=f;
        }
        else
        {
            f->prev->next=f;
        }
    }
}

//The cd command
void cd(char* ans,const char* params)
{
    strcpy(ans,"");
    if(strcmp(params,".")==0)
    {
        return;
    }
    if(strcmp(params,"..")==0)
    {
        dir=dir->parent;
        return;
    }
    file* f=find(dir,params);
    if(f==NULL)
    {
        strcpy(ans,"Directory not found");
        return;
    }
    if(f->isFolder)
    {
        dir=f;
    }
    else
    {
        strcpy(ans,"This is not a directory");
    }
}

//the rm command
void rm(char* ans,const char* params)
{
    file* f=find(dir,params);
    if(f==NULL)
    {
        strcpy(ans,"File not found");
        return;
    }
    if(f->isFolder)
    {
        strcpy(ans,"This is a directory");
    }
    else
    {
        strcpy(ans,"");
        if(f->prev==NULL)
        {
            f->parent->addr=f->next;
        }
        else
        {
            f->prev->next=f->next;
        }
        if(f->next!=NULL)
        {
            f->next->prev=f->prev;
        }
        //free(f->addr);
        //free(f);
    }
}

//the rmdir command
void rmdir(char* ans,const char* params)
{
    file* f=find(dir,params);
    if(f==NULL)
    {
        strcpy(ans,"Directory not found");
        return;
    }
    if(f->isFolder)
    {
        if(f->addr!=NULL)
        {
            strcpy(ans,"Directory not empty");
            return;
        }
        strcpy(ans,"");
        if(f->prev==NULL)
        {
            f->parent->addr=f->next;
        }
        else
        {
            f->prev->next=f->next;
        }
        if(f->next!=NULL)
        {
            f->next->prev=f->prev;
        }
        //free(f->addr);
        //free(f);
    }
    else
    {
        strcpy(ans,"This is not a directory");
    }
}

//The cat command
void cat(char* ans,const char* params)
{
    file* f=find(dir,params);
    if(f==NULL)
    {
        strcpy(ans,"File not found");
        return;
    }
    if(f->isFolder)
    {
        strcpy(ans,"This is a directory");
    }
    else
    {
        uint32_t i;
        for(i=0;i<f->size;i++)
        {
            ans[i]=((char*)(f->addr))[i];
        }
        ans[f->size]='\0';
    }
}

void write(char* ans,const char* params)
{
    uint32_t i=0,j;
    while(1)
    {
        if(params[i]=='\0')
        {
            strcpy(ans,"Not enough parameters");
            return;
        }
        if(params[i]==' ')
        {
            strcpy(ans,"");
            char* name=(char*)malloc(i+1);
            for(j=0;j<i;j++)
            {
                name[j]=params[j];
            }
            name[i]='\0';
            uint32_t size=strlen(params+i+1)+1;
            file* f=(file*)malloc(sizeof(file));
            f->isFolder=0;
            f->name=name;
            f->next=NULL;
            f->prev=last(dir);
            f->parent=dir;
            f->addr=malloc(size);
            f->size=size;
            strcpy(f->addr,params+i+1);
            if(f->prev==NULL)
            {
                dir->addr=f;
            }
            else
            {
                f->prev->next=f;
            }
            return;
        }
        i++;
    }
}
