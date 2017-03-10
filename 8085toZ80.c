/* ########################################################################

   8085asm - Simple 8085 to z80 assembly converter  

   ########################################################################

   Copyright (c) : 2016-2017  Luis Claudio Gamboa Lopes

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


int lc=1;

void
ucase (char * str)
{
int i;
  if(str != NULL)
  {
    for(i=0; i < strlen(str);i++)
      str[i]=toupper(str[i]);
  }    
}

int addr=0;
int addi=0;
unsigned char prg[256];
int labelsc=0;


int 
parsearg(char* arg, char * line)
{
int i;

  if(arg == NULL)
  {
    printf("line %i error!!!!\n%s\n",lc+1,line);
    exit(-1);
  };

  for(i=0;i<labelsc;i++)
  {
     if(strcmp(labels[i].nome,arg)==0)
     {
       return labels[i].value;
     };
  }

  if(arg[0] == '#')
    sscanf(arg+1,"%i",&i);  
  else	  
    sscanf(arg,"%X",&i);  
  return i;
};


void testz80(const char * op, const char * arg, char * ret)
{
	      if(op[0] == 0) 
	      {
	        strcpy(ret,arg);
              }
	      else
	      {
	        if(op[0] == '#')
		{
	          sprintf(ret,op+1,arg);
		}
		else
		{
	          strcpy(ret,op);
		}
	      }
}



char line2[256];

int parse(char * line)
{

int i=0;
int t;
char sline[256];
char* label;
char* men;
char* arg1;
char* arg2;
char  arg1u[100];
char  arg2u[100];
int err;
char stemp[100];
char stemp2[100];

  addi=0;
  if((line[0]=='\n')||(line[0]=='\r')) return 0;

  label= strtok(line,";\n\r");
  strcpy(sline,label);
  if((sline[0] != ' ')&&(sline[0]!='\t'))
  {
    label= strtok(sline," \t:,\n");
    men= strtok(NULL," \t:,\n");
    
  }
  else
  {
    label=NULL;
    men= strtok(sline," \t:,\n");
  }

 if(men == NULL)return 0;
 arg1= strtok(NULL," \t:,\n");
 arg2= strtok(NULL," \t:,\n");

 ucase(men); 
 if(arg1 != NULL) 
 {
   strcpy(arg1u,arg1);
   ucase(arg1u);
 }   
 if(arg2 != NULL)
 {
   strcpy(arg2u,arg2);	
   ucase(arg2u);
 }	 
 //printf("addr=%04X label=%s  opcode=%s  arg1=%s  arg2=%s\n",addr,label,men,arg1,arg2);


  err=0;
//busca opcodes
  do
  {
      if(strcmp(opcode[i].men,men)==0)
      {

	if(opcode[i].nargs == 0)
	{
          addi=opcode[i].uargs;
          prg[0]=opcode[i].opc;
          switch(addi)
	  {
	    case 0:
	      if(label != NULL)      
	        sprintf(line2,"%s:\t%s",label,opcode[i].mz80);
              else 
	        sprintf(line2,"\t%s",opcode[i].mz80);
              return 1;
	    case 1:
              prg[1]=parsearg(arg1,line);

              testz80(opcode[i].a1z80,arg1,stemp);              

	      if(label != NULL)      
	        sprintf(line2,"%s:\t%s %s",label,opcode[i].mz80,stemp);
              else 
	        sprintf(line2,"\t%s %s",opcode[i].mz80,stemp);
              return 1;
            case 2:
	      t=parsearg(arg1,line);
	      prg[1]=0x00FF&t;
	      prg[2]=(0xFF00&t)>>8;

              testz80(opcode[i].a1z80,arg1,stemp); 

	      if(label != NULL)      
	        sprintf(line2,"%s:\t%s %s",label,opcode[i].mz80,stemp);
              else 
	        sprintf(line2,"\t%s %s",opcode[i].mz80,stemp);
              return 1;
	  }
          return 1;
	}
	else
	{
	  if(arg1 == NULL)
	  {
            printf("line %i error!!!!\n%s\n",lc,line);
            exit(-1);
	  }  
          if(strcmp(opcode[i].arg1,arg1u)==0)
	  {
	    if(opcode[i].nargs == 1)
	    {
	      addi=opcode[i].uargs;
	      prg[0]=opcode[i].opc;
              switch(addi)
	      {
	       case 0:
                  testz80(opcode[i].a1z80,arg1,stemp); 
	          
		  if(label != NULL)      
	            sprintf(line2,"%s:\t%s %s",label,opcode[i].mz80,stemp);
                  else 
	            sprintf(line2,"\t%s %s",opcode[i].mz80,stemp);
                  return 1;
	        case 1:
                  prg[1]=parsearg(arg2,line);
                  testz80(opcode[i].a1z80,arg1,stemp); 
                  testz80(opcode[i].a2z80,arg2,stemp2); 
	          
		  if(label != NULL)      
	            sprintf(line2,"%s:\t%s %s, %s",label,opcode[i].mz80,stemp,stemp2);
                  else 
	            sprintf(line2,"\t%s %s, %s",opcode[i].mz80,stemp,stemp2);
                  return 1;
                case 2:
	          t=parsearg(arg2,line);
	          prg[1]=0x00FF&t;
	          prg[2]=(0xFF00&t)>>8;
                  testz80(opcode[i].a1z80,arg1,stemp); 
                  testz80(opcode[i].a2z80,arg2,stemp2); 
	          
		  if(label != NULL)      
	            sprintf(line2,"%s:\t%s %s, %s",label,opcode[i].mz80,stemp,stemp2);
                  else 
	            sprintf(line2,"\t%s %s, %s",opcode[i].mz80,stemp,stemp2);
                  return 1;
	      }
	      return 1;
	    }
	    else
	    {
	      if(strcmp(opcode[i].arg2,arg2u)==0)
	      {
		prg[0]=opcode[i].opc;
                testz80(opcode[i].a1z80,arg1,stemp); 
                testz80(opcode[i].a2z80,arg2,stemp2); 

	        if(label != NULL)      
	          sprintf(line2,"%s:\t%s %s, %s",label,opcode[i].mz80,stemp,stemp2);
                else 
	          sprintf(line2,"\t%s %s, %s",opcode[i].mz80,stemp,stemp2);
	        return i+1;
	      }
	    }
	  }
	  err=1;
	}

      }
      else
      {
        if(err)
	{
          printf("line %i error!!!!\n%s\n",lc,line);
          exit(-1);
	}
      }
  }
  while(strcmp(opcode[i++].men,"ENDO") != 0);

  return 0;

}

int
main(int argc,char** argv)
{
char fname[256];
char fname2[256];
char line3[256];
FILE* fin;
char line[256];
char *fnamep;
FILE *fout;


  if(argc == 2)
  {
    strcpy(fname,argv[1]);
  }  
  else
  {
   printf("enter the file name: ");
   scanf("%s",fname);
   getchar();
  }

  fin=fopen(fname,"r");

  if(!fin)
  {
     printf("Error open file:%s\n",fname);
     return -1;
  }
  

  printf("LCGamboa 8085 to Z80 assembly converter 2008-2017\n\n");

  strcpy(fname2,fname);
  fnamep=strrchr(fname2,'.');
  fnamep[0]=0;
  strcat(fname2,"_z80.asm");
  fout=fopen(fname2,"w");
  
  if(!fout)
  {
     printf("Error open file:%s\n",fname2);
     return -1;
  }

  addr=0;
  lc=0;
  while(fgets(line,256,fin))
  {

     if(line[0] == ';')
     {
       fprintf(fout,"%s",line);
     }
     else
     {
       strcpy(line3,line);
       if(parse(line))
       {
	 fprintf(fout,"%-30s;%s",line2,line3);

       }
       else
       {
         if(strtok(line,"\n\r") != NULL)
           fprintf(fout,"%s\n",line);
         else
           fprintf(fout,"\n");
       }
     }

     lc++;
  }


  fclose(fout);

  printf("Type enter to exit\n");
//  getchar();
return 0;
}
