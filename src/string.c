#include <stddef.h>
#include <stdint.h>
#include "string.h"

//returns a null pointer if s is not a valid instance of command cmd, returns a pointer to the parameters of the command if it is
char* strcmd(const char* cmd,const char* s)
{
    int i=0;
    while(1)
    {
        if(cmd[i]==s[i])
        {
            if(s[i]=='\0')
            {
                return (char*)s+i;
            }
            i++;
        }
        else if(cmd[i]=='\0')
        {
            if(s[i]==' ')
            {
                return (char*)s+i+1;
            }
            else
            {
                return NULL;
            }
        }
        else
        {
            return NULL;
        }
    }
}

//returns -1 if s1 is before s2 in lexicographical order, 1 if s2 is before s1 and 0 if they are the same
char strcmp(const char* s1,const char* s2)
{
    int i=0;
    while(1)
    {
        if(s1[i]==s2[i])
        {
            if(s1[i]=='\0')
            {
                return 0;
            }
            i++;
        }
        else if(s1[i]<s2[i])
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }
}

//returns the length of the string s
uint32_t strlen(const char* s)
{
    uint32_t i=0;
    while(1)
    {
        if(s[i]=='\0')
        {
            return i;
        }
        i++;
    }
}

//copies s2 into s1
char* strcpy(char* s1,const char* s2)
{
    uint32_t i=0;
    while(1)
    {
        s1[i]=s2[i];
        if(s2[i]=='\0')
        {
            return s1;
        }
        i++;
    }
}

//appends s2 to s1
void append(char* s1,const char* s2)
{
    strcpy(s1+strlen(s1),s2);
}

//the echo command
void echo(char* ans,const char* params)
{
    strcpy(ans,params);
}
