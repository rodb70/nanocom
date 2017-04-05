/******************************************************************
** Implements the menu system for nanocom
**
** Based on help.c from microcom Copyright (C)1999 Anca and Lucian Jurubita
****************************************************************************
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details at www.gnu.org
**  
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
****************************************************************************/
#include "nanocom.h"

extern int crnl_mapping; //0 - no mapping, 1 mapping
extern int script; /* script active flag */
extern char scr_name[MAX_SCRIPT_NAME]; /* default name of the script */
extern char device[MAX_DEVICE_NAME]; /* serial device name */

static int help_state = 0;
static int in_escape = 0;

/* static functions proto-types */
static void main_menu();
static void baud_menu(void);
static void data_bits_menu(void);
static void parity_menu(void);
static void stop_bits_menu(void);
static void flow_control_menu(void);
static void echo_settings_menu(void);

static void process_main_menu(int fd, char c);
static void process_baud_menu(int fd,char c);
static void process_data_bits_menu(int fd,char c);
static void process_parity_menu(int fd,char c);
static void process_stop_bits_menu(int fd,char c);
static void process_flow_control_menu(int fd,char c);
static void process_echo_settings_menu(int fd,char c);

static void parse_help_state(int fd,char buf);
static void reset_state(void);

char err[] = "\nInvalid option\n";
 char leaving[]="\n\nLeaving menu\n\n";


/******************************************************************
menu functions

all functions of the naming convention xxxx_menu just display a menu
all functions of the naming convention process_xxxx_menu perform the actions for that menu

menu is called by pressing ctrl+s, handled in cook_buf()
*/


/*the main menu*/
void main_menu(void) {

   char str1[] = "**********Menu***********\n"
    "1. Display Current Line Status\n"
    "2. Set Bit Rate\n"
    "3. Set Data Bits\n"
    "4. Set Parity\n"
    "5. Set Stop Bits\n"
    "6. Set Flow Control\n"
    "7. Set Echo Settings\n"
    "8. Exit\n"
    "9. Leave Menu\n"
    "*************************\n";   

  write(STDOUT_FILENO, str1, strlen(str1)); 
}

/*display baud rate menu*/
static void baud_menu(void) {
  char str[] =
    "\n******Set speed *********\n"
    " a - 300\n"
    " b - 1200\n"
    " c - 2400\n"
    " d - 4800\n"
    " e - 9600\n"
    " f - 19200\n"
    " g - 38400\n"
    " h - 57600\n"
    " i - 115200\n"
    "*************************\n"
    "Command: ";

  write(STDOUT_FILENO, str, strlen(str));
}

/*display parity menu*/
static void parity_menu(void) {
  char str[] =
    "\n******Set Pairty*********\n"
    " n - No Parity\n"
    " e - Even Parity\n"
    " o - Odd Parity\n"
    "*************************\n"
    "Command: ";

  write(STDOUT_FILENO, str, strlen(str));
}

/*display data bits menu*/
static void data_bits_menu(void) {
    char str[] =
    "\n******Set Number of Data Bits*********\n"
    " 7 - 7 Data Bits\n"
    " 8 - 8 Data bits\n"
    "*************************\n"
    "Command: ";

  write(STDOUT_FILENO, str, strlen(str));
}

/*display stop bits menu*/
static void stop_bits_menu(void) {
    char str[] =
    "\n******Set Number of Stop Bits*********\n"
    " 1 - 1 Stop Bit\n"
    " 2 - 2 Data Bits\n"
    "*************************\n"
    "Command: ";

  write(STDOUT_FILENO, str, strlen(str));
}

/*display menu for flow control*/
static void flow_control_menu(void) {
    char str[] =
    "\n******Set Flow Control Type*********\n"
    " h - Hardware Flow Control (CTS/RTS)\n"
    " s - Software Flow Control (XON/XOFF)\n"
    " n - No Flow Control\n"
    "*************************\n"
    "Command: ";

  write(STDOUT_FILENO, str, strlen(str));
}

/*display menu for echo settings*/
static void echo_settings_menu(void) {
    char str[] =
    "\n******Set Echo Type*********\n"
    " l - local echo (echo what you type locally)\n"
    " r - remote echo (echo what a remote system sends you, back to that system)\n"
    " n - No Echoing\n"
    "*************************\n"
    "Command: ";

  write(STDOUT_FILENO, str, strlen(str));
}


/* process function for help_state=0 */
static void process_main_menu(int fd, char c) { 
  

  

  help_state = c - 48;
  
  switch (c) {
    /*display current settings*/
    case '1':
        display_state();
        reset_state();
        /*we want to exit the menu*/
        break;
    /*baud rate*/
    case '2':
        /*display baud rate menu*/
        baud_menu();
        break;
    /*data bits*/
    case '3':
        data_bits_menu();
        break;
    /*parity*/
    case '4':
        parity_menu();
        break;
    /*stop bits*/
    case '5':
        stop_bits_menu();
        break;  
   /*flow control*/
    case '6':
        flow_control_menu();
        break;
    /*echo settings*/
    case '7':
        echo_settings_menu();
        break;
    /*quit the program*/
    case '8':
      /* restore termios settings and exit */
      write(STDOUT_FILENO, "\n", 1);
      cleanup_termios(0);
      break;
  case '9': /* quit help */
      reset_state();
      break;

    
  default:
    /* pass the character through */
    /* "C-\ C-\" sends "C-\" */
    write(fd, &c, 1);
    break;
  }



}


/*process responses to select a new baud rate*/
static void process_baud_menu(int fd,char c) {
    switch(c) {
        case 'a':
            curr_state.baud_rate=300;
            init_comm();
            break;
        case 'b':
            curr_state.baud_rate=1200;
            init_comm();
            break;
        case 'c':
            curr_state.baud_rate=2400;
            init_comm();            
            break;
        case 'd':    
            curr_state.baud_rate=4800;
            init_comm();       
            break;
        case 'e':
            curr_state.baud_rate=9600;
            init_comm();  
            break;
        case 'f':         
            curr_state.baud_rate=19200;
            init_comm();  
            break;
        case 'g':
            curr_state.baud_rate=38400;
            init_comm();  
            break;       
        case 'h':
            curr_state.baud_rate=57600;
            init_comm();            
            break;       
        case 'i':
            curr_state.baud_rate=115200;
            init_comm();                        
            break;       
        default:
           write(STDOUT_FILENO, err, strlen(err));
           break;       
    }
            
} 

/*process responses to select number of data bits*/
static void process_data_bits_menu(int fd,char c) {
    switch(c) {
        case '7':
        case '8':
            curr_state.data_bits=c;
            init_comm();
            break;
       default:
           write(STDOUT_FILENO, err, strlen(err));
           break;
    }            
} 
  
/*process responses to select parity settings*/
static void process_parity_menu(int fd,char c) {
    switch(c) {
        case 'n':
        case 'e':
        case 'o':
            curr_state.parity=c;
            init_comm();
            break;
       default:
           write(STDOUT_FILENO, err, strlen(err));
           break;
    }
            
} 

/*process responses to select number of stop bits*/
static void process_stop_bits_menu(int fd,char c) {
    switch(c) {
        case '1':
        case '2':
            curr_state.stop_bits=c;
            init_comm();
            break;
       default:
           write(STDOUT_FILENO, err, strlen(err));
           break;
    }            
} 
  
/*process responses to select flow control type*/
static void process_flow_control_menu(int fd,char c) {
    switch(c) {
        case 'n':
        case 'h':
        case 's':
            curr_state.flow_control=c;
            init_comm();
            break;
       default:
           write(STDOUT_FILENO, err, strlen(err));
           break;
    }            
} 
  
/*process responses to select echo settings*/
static void process_echo_settings_menu(int fd,char c) {
    switch(c) {
        case 'n':
        case 'l':
        case 'r':
            curr_state.echo_type=c;
            init_comm();
            break;
       default:
           write(STDOUT_FILENO, err, strlen(err));
           break;
    }            
} 
  

/*resets the menu state so we aren't left in a deeper level of the menu*/
static void reset_state(void) {
    in_escape=0;
    help_state=0;
    write(STDOUT_FILENO,leaving,strlen(leaving));
}
   

/*launch the right help processor based on the current help state
*/
static void parse_help_state(int fd,char buf)
{
        switch (help_state) {
            case 0:
                process_main_menu(fd, buf);
                break;
            /*there is no need for case 1 as it just displays the settings, no input required*/                
            case 2:
                process_baud_menu(fd,buf);
                reset_state();  
                break;
            case 3:
                process_data_bits_menu(fd,buf);
                reset_state();
                break;
            case 4:
                process_parity_menu(fd,buf);
                reset_state();
                break;
            case 5:
                process_stop_bits_menu(fd,buf);
                reset_state();
                break;
            case 6:
                process_flow_control_menu(fd,buf);
                reset_state();
                break;
           case 7:
                process_echo_settings_menu(fd,buf);
                reset_state();
                break;
            /*there is no need for case 8 as we'll have exited before getting to this menu*/
            /*same goes for case 9*/
           default: 
               reset_state();
               break;
        } 
}


/*void cook_buf(int fd, char *buf, int num) -
    redirect escaped characters to the help handling functions;
    the function is called from mux.c in the main character
    processing ruoutine;
    
    - fd - file handle for the comm port
    - buf - pointer to the character buffer
    - num - number of valid characters in the buffer*/

void cook_buf(int fd, char *buf, int num) { 
  int current = 0;
  
  if (in_escape) {
    /* cook_buf last called with an incomplete escape sequence */
    parse_help_state(fd,buf[current]);
    num--; /* advance to the next character in the buffer */
    buf++; 
  }

  while (current < num) { /* big while loop, to process all the charactes in buffer */

    /* look for the next escape character 'ctrl+s' */
    while ((current < num) && (buf[current] != 0x14)) {
        current++;
    }
    /* and write the sequence befor esc char to the comm port */
    if (current) {
        write (fd, buf, current);
    }

    if (current < num) { /* process an escape sequence */
        /* found an escape character */
        if (help_state == 0) {
            /*not already in help, display the menu*/
            main_menu();
        }
        current++;
        if (current >= num) {
            /* interpret first character of next sequence */
            in_escape = 1;
            return;
        }
        parse_help_state(fd,buf[current]);

        /* end switch */
        current++;
        if (current >= num) {      
            return;      
        }
    } /* if - end of processing escape sequence */
    num -= current;
    buf += current;
    current = 0;
  } /* while - end of processing all the charactes in the buffer */
  return;
}
