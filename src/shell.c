#include <stddef.h>
#include <stdint.h>
#include "mailbox.h"
#include "util.h"
#include "screen.h"
#include "shell.h"
#include "string.h"
#include "file.h"
#include "vitamin.h"

uint16_t lineCopy[8];

//Underlines the character at position (x,y) and save whatever was erased
void underline(int x,int y,int width,int height,uint16_t* fb)
{
    (void)height;
    for(int ix=0;ix<8;ix++)
    {
        lineCopy[ix]=fb[x+ix+width*(y+15)];
        fb[x+ix+width*(y+15)]=0xFFFF;
    }
}

//Removes the underline underline under the character at position (x,y) and restores what used to be there
void ununderline(int x,int y,int width,int height,uint16_t* fb)
{
    (void)height;
    for(int ix=0;ix<8;ix++)
    {
        fb[x+ix+width*(y+15)]=lineCopy[ix];
    }
}

//A basic shell
void shell(int width,int height,uint16_t* fb)
{
    char c;
    char* param;
    int x=16,y=0,comLen=2,curPos=2,promptLen=2;//curPos=Position of the cursor (including the prompt), promptLen=length of the prompt
    char com[42];//The command
    char ans[420];//Whatever the command answers
    com[0]='$';
    com[1]=' ';
    while(1)
    {
        com[comLen]=' ';
        com[comLen+1]='\0';
        disp_string(0,y,width,height,com,fb);
        underline(x,y,width,height,fb);
        c=uart_getc();
        ununderline(x,y,width,height,fb);
        switch(c)
        {
            case '\r':
            case '\n':
                x=16;
                y+=16;
                com[comLen]='\0';
                strcpy(ans,"This command does not exist");
                if((param=strcmd("cat",com+2)))
                {
                    cat(ans,param);
                }
                if((param=strcmd("cd",com+2)))
                {
                    cd(ans,param);
                }
                if((param=strcmd("clear",com+2)))
                {
                    clear_screen(width,height,fb);
                    strcpy(ans,"");
                    y=0;
                }
                if((param=strcmd("echo",com+2)))
                {
                    echo(ans,param);
                }
                if((param=strcmd("logo",com+2)))
                {
                    disp_large_string(350,250,5,width,height,"SKORBUT",fb);
                    disp_logo(650,258,width,height,fb);
                    strcpy(ans,"");
                }
                if((param=strcmd("ls",com+2)))
                {
                    ls(ans,param);
                }
                if((param=strcmd("mkdir",com+2)))
                {
                    mkdir(ans,param);
                }
                if((param=strcmd("note",com+2)))
                {
                    notepad_demo(width,height,fb);
                }
                if((param=strcmd("pwd",com+2)))
                {
                    pwd(ans,param);
                }
                if((param=strcmd("rm",com+2)))
                {
                    rm(ans,param);
                }
                if((param=strcmd("rmdir",com+2)))
                {
                    rmdir(ans,param);
                }
                if((param=strcmd("touch",com+2)))
                {
                    touch(ans,param);
                }
                if((param=strcmd("write",com+2)))
                {
                    write(ans,param);
                }
                if(strlen(ans)!=0)
                {
                    disp_string(0,y,width,height,ans,fb);
                    y+=16;
                }
                comLen=2;
                curPos=2;
                break;
            case '\t':
                break;
            case 126:
                if(curPos<promptLen)
                {
                    comLen--;
                    for(int i=curPos;i<comLen;i++)
                    {
                        com[i]=com[i+1];
                    }
                }
                break;
            case 127:
                if(curPos>promptLen)
                {
                    comLen--;
                    curPos--;
                    for(int i=curPos;i<comLen;i++)
                    {
                        com[i]=com[i+1];
                    }
                    x-=8;
                }
                break;
            case '\e':
                uart_getc();
                switch(uart_getc())
                {
                    case 'C':
                        if(curPos<comLen)
                        {
                            x+=8;
                            curPos++;
                        }
                        break;
                    case 'D':
                        if(curPos>promptLen)
                        {
                            x-=8;
                            curPos--;
                        }
                        break;
                }
                if(x>=width-8)
                {
                    x=0;
                    y+=16;
                }
                x=max(x,0);
                y=max(y,0);
                break;
            default:
                comLen++;
                for(int i=comLen-1;i>curPos;i--)
                {
                    com[i]=com[i-1];
                }
                com[curPos]=c;
                curPos++;
                x+=8;
                break;
        }
    }
}

//A minimal notepad
void notepad_demo(int width,int height,uint16_t* fb)
{
    clear_screen(width,height,fb);
    char c;
    int x=0,y=0;
    while(1)
    {
        underline(x,y,width,height,fb);
        c=uart_getc();
        switch(c)
        {
            case '\r':
            case '\n':
                ununderline(x,y,width,height,fb);
                x=0;
                y+=16;
                break;
            case '\t':
                ununderline(x,y,width,height,fb);
                x+=32;
                if(x>=width-8)
                {
                    x=0;
                    y+=16;
                }
                break;
            case 127:
                ununderline(x,y,width,height,fb);
                x-=8;
                x=max(x,0);
                disp_char(x,y,width,height,' ',fb);
                break;
            case '\e':
                ununderline(x,y,width,height,fb);
                uart_getc();
                switch(uart_getc())
                {
                    case 'A':
                        y-=16;
                        break;
                    case 'B':
                        y+=16;
                        break;
                    case 'C':
                        x+=8;
                        break;
                    case 'D':
                        x-=8;
                        break;
                }
                if(x>=width-8)
                {
                    x=0;
                    y+=16;
                }
                x=max(x,0);
                y=max(y,0);
                break;
            default:
                disp_char(x,y,width,height,c,fb);
                x+=8;
                if(x>=width-8)
                {
                    x=0;
                    y+=16;
                }
                break;
        }
    }
}
