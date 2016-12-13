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

typedef struct
{
unsigned char value;
unsigned short int addr;
}Mem;

Mem mem[65535];

typedef struct
{
 char nome[100];
 int value;
} Symbol;

Symbol labels[250];


typedef struct
{
  int  opc;
  char men[5];
  int  nargs;
  char arg1[4];
  char arg2[4];
  int  uargs;		/* 0 - no arg, 1 - byte, 2 - word */
} Opcode;


