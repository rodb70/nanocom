#Makefile for nanocom
#Only file not based on microcom!
#****************************************************************************
#** This program is free software; you can redistribute it and/or
#** modify it under the terms of the GNU General Public License
#** as published by the Free Software Foundation; either version 2
#** of the License, or (at your option) any later version.
#**
#** This program is distributed in the hope that it will be useful,
#** but WITHOUT ANY WARRANTY; without even the implied warranty of
#** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#** GNU General Public License for more details at www.gnu.org
#****************************************************************************/

CC=gcc

all: clean nanocom

nanocom: 
	$(CC) mux.c nanocom.c menu.c -o nanocom
    
clean:
	rm -rf *.o nanocom
    