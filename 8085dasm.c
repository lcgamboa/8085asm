/* ########################################################################

   8085asm - Simple 8085 assembler and disassembler

   ########################################################################

   Copyright (c) : 2009  Luis Claudio Gambôa Lopes

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   For e-mail suggestions :  lcgamboa@yahoo.com
   ######################################################################## */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<stddef.h>
#include<ctype.h>

#include"8085asm.h"
#include"opcodes.h"

extern int read_ihx(const char * fname);
extern int mc;

int
main(int argc,char** argv)
{
char fname[256];
int i,j,k;
unsigned short paddr;

  if(argc == 2)
  {
    strcpy(fname,argv[1]);
  }  
  else
  {
  printf("enter the file name: ");
  scanf("%s",fname);
  }

  printf(";LCGamboa 8085 disassembler 2008\n\n");

  read_ihx(fname);


  printf("         ORG %04X\n",mem[0].addr); 
  paddr=mem[0].addr; 

  for(i=0;i<mc;i++)
  {
    //printf("%04X  %02X\n",mem[i].addr,mem[i].value);
      
    if(mem[i].addr != paddr)
    {
      printf("         ORG %04X\n",mem[i].addr); 
      paddr=mem[i].addr; 
    }
    
    j=0;
    do
    {

      if(opcode[j].opc == mem[i].value)
      {
        printf("L%04X:   %s ",mem[i].addr,opcode[j].men); 
	
	if( opcode[j].nargs == 1)
	{
	  if(opcode[j].uargs ==0)
	    printf("%s ",opcode[j].arg1);
	  else
	    printf("%s, ",opcode[j].arg1);
	}
	
	if( opcode[j].nargs == 2)
	{
	  printf("%s, %s ",opcode[j].arg1, opcode[j].arg2); 
	}
        
	for(k=opcode[j].uargs; k >0 ; k--)
	{
	  if(k==1)	
	    printf("%02Xh",mem[i+k].value); 
	  else
	    printf("%02X",mem[i+k].value); 
	}
	
	printf("\n");
        
	i+=opcode[j].uargs;
	paddr+=opcode[j].uargs+1;
	break;
      }
    }
    while(strcmp(opcode[j++].men,"ENDO") != 0);
  }


return 1;
}
