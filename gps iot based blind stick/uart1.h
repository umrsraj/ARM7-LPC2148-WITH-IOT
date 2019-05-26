#include <lpc214x.h>

void uart1_init(void);
void uart1_putch(unsigned char ch);
void uart1_puts(unsigned char str[]);
unsigned char uart1_getch(void);
void uart1_enter(void);
void uart1_convert(unsigned int value);

void uart1_init(void)
{
   PINSEL0 |= UART1;		  //SELECT THE PINS FOR UART0;

   /****** FOR 9600 Baud Rate ************/
   U1LCR  = 0x80;                 //SET THE DLAB=1;
   U1DLM  = 0x00;				   //SET THE DLM;
   U1DLL  = 0x61;				   //SET THE DLL;

   U1LCR  = 0x03;                 //CLEAR THE DLAB=1;
}

void uart1_putch(unsigned char ch)
{

  while(!(U1LSR&0x20));
  U1THR = ch;
}

void uart1_puts(unsigned char str[])
{
  unsigned char i;

  for(i=0;str[i]!='\0';i++)
  {
   while(!(U1LSR&0x20));
  U1THR = str[i];
  }
}

unsigned char uart1_getch(void)
{
 
  while(!(U1LSR&0x01));
  return U1RBR;
}

void uart1_enter(void)
{

  uart1_putch(13);
  uart1_putch(10); 
}

void uart1_convert(unsigned int value)
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
      		uart1_putch(str[x]);
	}
