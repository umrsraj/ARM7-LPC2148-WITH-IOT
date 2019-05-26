#include <lpc214x.h>

void uart0_init(void);
void uart0_putch(unsigned char ch);
void uart0_puts(unsigned char str[]);
unsigned char uart0_getch(void);
void uart0_enter(void);
void uart0_convert(unsigned int value);

void uart0_init(void)
{
   PINSEL0 |= UART0;		  //SELECT THE PINS FOR UART0;

   /****** FOR 9600 Baud Rate ************/
   U0LCR  = 0x80;                 //SET THE DLAB=1;
   U0DLM  = 0x00;				   //SET THE DLM;
   U0DLL  = 0x61;				   //SET THE DLL;

   U0LCR  = 0x03;                 //CLEAR THE DLAB=1;
}

void uart0_putch(unsigned char ch)
{

  while(!(U1LSR&0x20));
  U0THR = ch;
}

void uart0_puts(unsigned char str[])
{
  unsigned char i;

  for(i=0;str[i]!='\0';i++)
  {
   while(!(U0LSR&0x20));
  U0THR = str[i];
  }
}

unsigned char uart0_getch(void)
{
 
  while(!(U0LSR&0x01));
  return U0RBR;
}

void uart0_enter(void)
{

  uart0_putch(13);
  uart0_putch(10); 
}

void uart0_convert(unsigned int value)
   { 
    unsigned int adcdata;
	unsigned char x,str[3];
	
     adcdata=value;

  	 for(x=1;x<5;x++)					   
     {
      str[x]=adcdata%10+ 48;
      adcdata= adcdata/10;
     }
	  str[x]='\0';
	  
	  for(x=4; x!=0 ; x--)					/* sending data to PC*/
      		uart0_putch(str[x]);
	}
