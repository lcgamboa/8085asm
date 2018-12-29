/* ########################################################################

   8085asm - Simple 8085 assembler and disassembler

   ########################################################################

   Copyright (c) : 2008-2018  Luis Claudio Gamboa Lopes

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
int pass;
int memc;


int 
parsearg(char* arg, char * line)
{
  int i;
  int ret=-1;

  if(arg == NULL)
  {
    printf("line %i error!!!!\n%s\n",lc+1,line);
    exit(-1);
  };
  

  if(arg[0] == '#')
  {
     sscanf(arg+1,"%i",&ret);
  }  
  else if(isdigit(arg[0]))
  {
     sscanf(arg,"%X",&ret);  
  }
  else
  {
    for(i=0;i<labelsc;i++)
    { 
       if(strcmp(labels[i].nome,arg)==0)
       {
         ret= labels[i].value;
       }
    }
  }
  
  //printf("%s = %i\n",arg,ret);

  if(ret < 0)
  {
    printf("Undefined Argument (%s) in line %i: %s\n",arg,lc+1,line);
    exit(-1);
  }
  
  return ret;
};


int parse(char * line)
{
int i=0;
int t;
char sline[256];
char* label;
char* men;
char* arg1;
char* arg2;
int err;

  addi=0;
  if((line[0]=='\n')||(line[0]=='\r')) return 0;

  label= strtok(line,";\n\r");
  strcpy(sline,label);
  if((sline[0] != ' ')&&(sline[0]!='\t'))
  {
    label= strtok(sline," \t:,\n");
    men= strtok(NULL," \t:,\n");
    
    if(pass==1)
    {
      //printf("label=%s  %04X\n",label,addr);
      ucase(label);
      strcpy(labels[labelsc].nome,label);
      labels[labelsc].value=addr;
      labelsc++;
    }

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
 if(arg1 != NULL) ucase(arg1);	
 if(arg2 != NULL) ucase(arg2);	
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
	  if(pass ==2)
          switch(addi)
	  {
	    case 0:
              return 1;
	    case 1:
              prg[1]=parsearg(arg1,line);
              return 1;
            case 2:
	      t=parsearg(arg1,line);
	      prg[1]=0x00FF&t;
	      prg[2]=(0xFF00&t)>>8;
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
          if(strcmp(opcode[i].arg1,arg1)==0)
	  {
	    if(opcode[i].nargs == 1)
	    {
	      addi=opcode[i].uargs;
	      prg[0]=opcode[i].opc;
	      if(pass ==2)
              switch(addi)
	      {
	       case 0:
                  return 1;
	        case 1:
                  prg[1]=parsearg(arg2,line);
                  return 1;
                case 2:
	          t=parsearg(arg2,line);
	          prg[1]=0x00FF&t;
	          prg[2]=(0xFF00&t)>>8;
                  return 1;
	      }
	      return 1;
	    }
	    else
	    {
	      if(strcmp(opcode[i].arg2,arg2)==0)
	      {
	        addi=opcode[i].uargs;
		prg[0]=opcode[i].opc;
	        return 1;
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



//busca pseudo
    if(strcmp("ORG",men)==0)
    {
       addr=parsearg(arg1,line);
//       printf("org ===> %04X\n",i);
       return 0;
    };
    
    if((label != NULL )&&(pass==1))
    {
    if(strcmp("EQU",men) == 0)
    {
      sscanf(arg1,"%x",&i);
      labels[labelsc-1].value=i;
      return 0;
    }
    }
    else
    {
       if(strcmp("EQU",men) == 0)return 0;
    }

    if((strcmp("DB",men)==0) || (strcmp(".DB",men)==0))
    {  
      addi=0;
      prg[addi]=parsearg(arg1,line);
      
      if(arg2 != NULL)
      {
       addi++;
       prg[addi]=parsearg(arg2,line);
      }
 
      arg1= strtok(NULL," \t:,\n");
      while(arg1 != NULL)
      {
       ucase(arg1);	
       addi++;
       prg[addi]=parsearg(arg1,line);
       arg1= strtok(NULL," \t:,\n");
      }
 	 
  
      return 1;
    }

    if(strcmp("DS",men)==0)
    {
       i=parsearg(arg1,line);
      addr+=i;
      return 0;
    }
    
    if(strcmp("END",men)==0)return 0;
    
    //only for compatiblity, without real function
    if(strcmp(".MODULE",men)==0)return 0;
    if(strcmp(".LIST",men)==0)return 0;
    if(strcmp(".NLIST",men)==0)return 0;
    if(strcmp(".AREA",men)==0)return 0;
    if(strcmp(".GLOBL",men)==0)return 0;


  //unknown 
  if(strcmp(opcode[i-1].men,"ENDO") == 0)
  {
          printf("line %i error!!!!\n%s   (Unknown Instruction)\n",lc,line);
          exit(-1);
  }
 
  return 0;
}

int
main(int argc,char** argv)
{
char fname[256];
char fname2[256];
FILE* fin;
char line[256];
int i;
unsigned char sum;
unsigned char nb;
unsigned short iaddr;
char values[100]; 
char *fnamep;
FILE *fout;
FILE *fout2;


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
  

  printf("LCGamboa 8085 assembler 2008-2016\n\n");

  strcpy(fname2,fname);
  fnamep=strrchr(fname2,'.');
  fnamep[0]=0;
  strcat(fname2,".map");
  fout=fopen(fname2,"w");
  
  if(!fout)
  {
     printf("Error open file:%s\n",fname2);
     return -1;
  }

  fprintf(fout,"LCGamboa 8085 assembler 2008-2018\n\n");
  pass=1;
  while(fgets(line,256,fin))
  {

     if(line[0] == ';')
     {
//       printf("%5i                           ==> %s",lc,line);
     }
     else
     {
       
       if(parse(line))
       {
//         printf("%5i     %04X    ",lc,addr);
//	 for(i=0;i<=addi;i++)printf("%02X ",prg[i]);
//	 for(i=addi;i<3;i++)printf("   ");
//	 printf("  ==> %s\n",line);
         addr+=1+addi;
       }
       else
       {
//         if(strtok(line,"\n\r") != NULL)
//           printf("%5i                           ==> %s\n",lc,line);
//         else
//           printf("%5i                           ==> \n",lc);
       }
     }

     lc++;
  }

  rewind(fin);
  pass=2;
  addr=0;
  lc=0;
  while(fgets(line,256,fin))
  {

     if(line[0] == ';')
     {
       fprintf(fout,"%5i                           ==> %s",lc,line);
     }
     else
     {
       
       if(parse(line))
       {
         fprintf(fout,"%5i     %04X    ",lc,addr);
	 for(i=0;i<=addi;i++)
	 {
	   fprintf(fout,"%02X ",prg[i]);
	   mem[memc].addr=addr+i;
	   mem[memc].value=prg[i];
	   memc++;
	}
	 for(i=addi;i<3;i++)fprintf(fout,"   ");
	 fprintf(fout,"  ==> %s\n",line);
         addr+=1+addi;
       }
       else
       {
         if(strtok(line,"\n\r") != NULL)
           fprintf(fout,"%5i                           ==> %s\n",lc,line);
         else
           fprintf(fout,"%5i                           ==> \n",lc);
       }
     }

     lc++;
  }

  fprintf(fout,"\n\nSYMBOLIC TABLE:\n\n");
  for(i=0;i<labelsc;i++)
  {
    fprintf(fout," %-10s  %04XH\n",labels[i].nome,labels[i].value);
  }
  
  
  fprintf(fout,"\n\nMEM  (%i bytes):\n\n",memc);
  
  printf("MEM  (%i bytes):\n\n",memc);


  strcpy(fname2,fname);
  fnamep=strrchr(fname2,'.');
  fnamep[0]=0;
  strcat(fname2,".hex");
  fout2=fopen(fname2,"w");
  
  if(!fout2)
  {
     printf("Error open file:%s\n",fname2);
     return -1;
  }

  nb=0;
  sum=0;
  for(i=0;i<memc;i++)
  {
//    printf(" %04XH  %02XH\n",mem[i].addr,mem[i].value);
    
    if(nb==0)
    {
      iaddr=mem[i].addr;
      sprintf(values,"%02X",mem[i].value);
    }
    else
    {
      sprintf(values,"%s%02X",values,mem[i].value);
    }

    nb++;
    sum+=mem[i].value;
    if((mem[i+1].addr != (mem[i].addr+1))||(nb==16))
    {
      sum+=nb;
      sum+=(iaddr&0x00FF);
      sum+=((iaddr&0xFF00)>>8);
      
      //printf("sum=%02X %02X %02X\n",sum,~sum,(~sum)+1);
      sum=(~sum)+1; 
      fprintf(fout2,":%02X%04X00%s%02X\n",nb,iaddr,values,sum);
      fprintf(fout,":%02X%04X00%s%02X\n",nb,iaddr,values,sum);
      nb=0;
      sum=0;
    }
  }
  fprintf(fout2,":00000001FF\n");
  fprintf(fout,":00000001FF\n");

  fclose(fout);
  fclose(fout2);

  printf("Type enter to exit\n");
//  getchar();
return 0;
}
