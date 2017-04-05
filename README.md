Readme for Nanocom

Background:

    Nanocom is based upon microcom (http://microcom.port5.com/) but removes the scripting and logging features
    while introducing support for setting local/remote echo, parity and stop bits. 

    It also follows a more standard command line style using the getopt function. 
    
    Internally much of the code has been rewritten and reformatted, the menu system in particular is almost entirely different.

Usage:    
        
    Nanocom's options can either be set at the command line or once the program is running by press CTRL+T.
    
    The command line options are all compulsory and are as follows:
    
        nanocom device [options]
        
                device          Specifies which serial port device to use, on most systems this will be /dev/ttyS0 or /dev/ttyS1
        
                [options] All options are required
        
                -b Bit rate, the bit rate to use, valid options are 300, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200.
                -p Parity setting, n for none, e for even or o for odd. Must be the same as the remote system.
                -s Stop bits, the number of stop bits (1 or 2)
                -d Data bits, the nuimber of data bits (7 or 8)
                -f Flow control setting, h for hardware (rts/cts), s for software (Xon/xoff) or n for none.
                -e Echo setting
                        l Local echo (echos everything you type, use if you don't see what you type)
                        r Remote echo (use if remote system doesn't see what is typed OR enable local echo on remote system)
                        n No echoing.
        

 Code:
 
     Nanocom consists of 5 files, mux.c, menu.c, nanocom.c, nanocom.h and Makefile
     
     nanocom.c contains the main function and all command line parameter verfication code. This contains
     some of the code from microcom.c.
     
     nanocom.h contains prototype definitions for nanocom.c and some general definitions for the whole program.
     This is mostly identical to microcom.h.
     
     menu.c contains all the menu related code. This copies some code from microcom's help.c file but reimplements the 
     whole menu system.
     
     mux.c contains the main loop of the program which sends data between the keyboard, users screen and serial port.
     This is virtually identical to the same file in microcom.
     
License

Nanocom is released under the GPL (GNU Public License)

It is based  upon code from microcom by Anca and Lucian Jurubita (C) Copyright 1999

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details at www.gnu.org
  
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
     
                    
           