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

#include"8085asm.h"

int
parse_hex(char *line,int bytes)
{
  char snum[200];
  int num;
    
  strncpy(snum,line,bytes);
  snum[bytes]='\0';
  sscanf(snum,"%X",&num);

  return num; 
};


unsigned char
checksum(char* str)
{
  unsigned int i;
  unsigned char acum=0;
  unsigned int size=strlen(str);
  
  if((str[size-1] == '\n')||(str[size-1] == '\r'))size--;
  if((str[size-1] == '\n')||(str[size-1] == '\r'))size--;

  for(i=0; i < ((size)/2);i++)
  {
    acum+=parse_hex(str+1+(i*2),2);
  }
  
  return acum;
}

int mc=0;

int
read_ihx(const char * fname)
{
  FILE* fin;
  int  lc=0;
  unsigned int bc;
  char line[256];
  unsigned int nbytes,addr,type;  

  unsigned short addrh=0;
  unsigned short addrl=0;

  fin=fopen(fname,"r");

  if(fin)
  {
    do
    {
      fgets(line,256,fin);
      lc++;
      if(checksum(line) == 0)
      {
        nbytes=parse_hex(line+1,2);
        addr=parse_hex(line+3,4);
        type=parse_hex(line+7,2);

	switch(type)
	{
	  case 0:
	    addrl=addr;

              for(bc=0;bc < nbytes;bc++)
	      {
	        mem[mc].addr=((addrh<<16)|addrl)+(bc);
		mem[mc].value=parse_hex(line+9+(bc*2),2);
	        mc++;
	      }
	  break;
	  case 1:
	    return 1;
	  break;
	  case 4:
	    addrh=addr/2;
	  break;
	}
      }
      else
      {
        printf("ERRO: File bad checksum line %i!(%s)\n",lc,fname);
        return 0;
      }
     }
     while(!feof(fin)); 
  }
  else
  {
    printf("ERRO: File not found!(%s)\n",fname);
    return 0;
  }
  return 1;
};

