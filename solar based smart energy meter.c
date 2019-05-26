/*
SOLAR BASED SMART ENERGY METER

LPC2148

https://thingspeak.com/channels/636266

P0.15 - EXTERNAL INTERRUPT FOR ENERGYMETER (PULL-UP)
VOLTAGE MONITORING - AD0.1
TAMPERING SWITCH - P0.3
LED INDICATIONS - P0.5,P0.6
BUZZER - P0.4
RELAY - LOAD - P0.7
IOT - UART1
LCD



***********************************************************************************************/

#include <lpc214x.h>		 //21007BB4B15F         18008D991A16
#include <stdio.h>
#include <string.h>
#include "lcd.h"
#include "serial.h"
#include "gpio.h"
#include "adc0.h"
#include "uart1.h"

#define ON  '1'//0x31
#define OFF '0' //0x30


//#define ALC P0_3

#define buzzer P0_4

#define SW P0_3

#define rled P0_5

#define gled P0_6

#define relay P0_7

///////////////////
char *i_to_a(int num);

//////////////////

char postStr[100] = "";
char A[45] = "GET /update?api_key=9WZ2EYXEVXCNTL5V&field1=";
char B[20] = "&field2=";
char C[20] = "&field3=";
char u[50] = "";
char c[50] = "";
char t[50] = "";

unsigned int f = 3;
unsigned int pastunits = 0;

unsigned int n = 0;

float v = 0.0;

float vol = 0.0;

volatile unsigned int units = 0;
volatile unsigned int bal = 0;



void init_ext_interrupt(void);
__irq void Ext_ISR(void);

void serial0_RxISR(void) __irq; 

void wifiinit(void) ;
void thingspeak (void) ;

unsigned int tamp,temp;

////////////ADC FUNCTIONS STOP/////////////////////
void pin()
{
 PINSEL0=0X00050005;//SELECTING PIN OPERATION
 PINSEL1=0X15000000;
 PINSEL2=0X00000000;
 //IOSET0=sensor1|sensor2;
	
	IODIR0 &= ~(1<<3); // input
	IODIR0 |= (1<<4);		// OUTPUT
	IODIR0 |= (1<<5);		//OUTPUT
	IODIR0 |= (1<<6);		// OUTPUT
	IODIR0 |= (1<<7);		//OUTPUT
	
}
/*


int no_of_digits(int num)
{
    int digit_count = 0;

    while(num > 0)
    {
        digit_count++;
        num /= 10;
    }

    return digit_count;
}


char *i_to_a(int num)
{
    char *str;
    int digit_count = 0;

    if(num < 0)
    {
        num = -1*num;
        digit_count++;
    }

    digit_count += no_of_digits(num);   
    str = malloc(sizeof(char)*(digit_count+1));

    str[digit_count] = '\0';

    while(num > 0)
    {
        str[digit_count-1] = num%10 + '0';
        num = num/10;
        digit_count--;
    }

    if(digit_count == 1)
        str[0] = '-';

    return str;
}
*///////////////////////////////
void blink ()
{
	
	IOSET0=buzzer;
	IOSET0=rled;
	IOSET0=gled;
	delay(400);
	
 IOCLR0=buzzer;
	IOCLR0=rled;
	IOCLR0=gled;
	delay(400);
	
	IOSET0=buzzer;
	IOSET0=rled;
	IOSET0=gled;
	delay(400);
	
 IOCLR0=buzzer;
 	IOCLR0=rled;
	IOCLR0=gled;
	
}

int inp0(int pinNumber)
{
  int pinBlockState = IOPIN0;
  int pinState = (pinBlockState & (/*1 <<*/ pinNumber)) ? 1 : 0;
  return pinState;
}

void sound()
{
 IOSET0=buzzer;
 delay(800);
 IOCLR0=buzzer;
}

int main (void) 
{
	pin();

  init_ext_interrupt();   // initialize the external interrupt
 
	initlcd();
	ADC0_Init();
uart1_init ();	
	//wifiinit() ;
	
	IOSET0=buzzer;
	IOSET0=rled;
	IOSET0=gled;
	delay(400);
	
 IOCLR0=buzzer;
	IOCLR0=rled;
	IOCLR0=gled;
	delay(400);
	
	IOSET0=buzzer;
	IOSET0=rled;
	IOSET0=gled;
	delay(400);
	
 IOCLR0=buzzer;
 	IOCLR0=rled;
	IOCLR0=gled;
	delay(400);
	
	
	//IODIR0 =0xfffffff0;    // lase 1= for input of 	  (f0  --- means p0.4=1,p0.5=1,p0.6=1,p0.7=1)  1 nibble 
	//SERIAL AND LCD INITIALIZING 
	ser_init_0(0x61,0x00);
	ser_init_1(0x61,0x00); //serial initilization 
	
	IOCLR0=buzzer;
	
	//WELCOME MESSAGE...	 
	stringlcd(0x80,"  SOLAR  BASED  "); //initial message
	stringlcd(0xc0,"  SMART  METER  ");
				 delay(2000);
				 
 units = 0;
 wifiinit ();
	units = 0;
 
 //WHILE................................................................
	
	
  while (1)  
  { 
		
		IOSET0=gled;
			IOCLR0=rled;
		
		tamp = 0;
		
		//bal = (units*5) ;
		
		  v = adc0_channel_1();
	 
	 vol = (int)(v * (5.00/1023.00)*4);	
		
		if (vol <5)
		{
			IOSET0=rled;
			IOCLR0=gled;
			
			clcd(1);
  stringlcd(0x80,"LOW VOLTAGE ALERT"); 
	stringlcd(0xC0,"PLEASE CHECK******"); 
sound ();		 
		 delay(1500);
		}
		IOSET0=relay;
		 if(inp0(SW)==1)
	 {		  
		 tamp = 100;
		 IOSET0=rled;
			IOCLR0=gled;
		 IOCLR0=relay;
		 f = 0;
		  clcd(1);
  stringlcd(0x80,"TAMPERING  ALERT"); 
	stringlcd(0xC0,"PLEASE CHECK******"); 
sound ();		 
		 delay(2000);
	 }
	 stringlcd(0x80,"U:");clcd(0x82);conv(units);
	 stringlcd(0x89,"C:");  clcd(0x8C);conv(bal); //stringlcd(0x8C,"0");
   stringlcd(0xC0,"V:");  clcd(0xC3);conv(vol); //stringlcd(0xc3,"0");
	 stringlcd(0xC9,"T:");  clcd(0xCC);conv(tamp); 
	 delay(2000);
	 
	 if ((pastunits < units)||(f < 2))
	 {
		 f ++ ;
		 pastunits = units ;
	 thingspeak () ;
	 }
	 
 }//WHILE
}// MAIN

void init_ext_interrupt()  //Initialize Interrupt
{
  EXTMODE = 0x4;           //Edge sensitive mode on EINT2
  
  EXTPOLAR &= ~(0x4);    //Falling Edge Sensitive
 
  PINSEL0 = 0x80000000;    //Select Pin function P0.15 as EINT2
  
  /* initialize the interrupt vector */
  VICIntSelect &= ~ (1<<16);        	// EINT2 selected as IRQ 16
  VICVectAddr5 = (unsigned int)Ext_ISR; // address of the ISR
  VICVectCntl5 = (1<<5) | 16; // 
  VICIntEnable = (1<<16);           // EINT2 interrupt enabled
 
  EXTINT &= ~(0x4); 
}
 
 
__irq void Ext_ISR(void) // Interrupt Service Routine-ISR 
{
	/*
 IO1DIR |= (1<<25);
 IO1CLR |= (1<<25); // Turn ON Buzzer
 delay(100);
 IO1SET |= (1<<25); // Turn OFF Buzzer
 
 EXTINT |= 0x4; //clear interrupt
 VICVectAddr = 0; // End of interrupt execution
	
	
	if (flag == 0)
	{
		flag = 1;
		IO1DIR |= (1<<25);
		 IO1SET |= (1<<25); // Turn OFF Buzzer
	}
	else
	{
		flag = 0;
		IO1DIR |= (1<<25);
		 IO1CLR |= (1<<25); // Turn ON Buzzer
	}
	*/
	
	++units;
	bal = (units*5) ;
	
	EXTINT |= 0x4; //clear interrupt
 VICVectAddr = 0; // End of interrupt execution
}
 
void wifiinit() 
{
	stringlcd(0x80,"WI-FI INITIALIZING"); //initial message
	stringlcd(0xc0,"**** PLZ WAIT ****");
				 delay(500);
	
	uart1_puts("AT\r\n"); delay(1000);
	uart1_puts("AT+RST\r\n"); delay(2000);
	uart1_puts("AT\r\n"); delay(1000);
	uart1_puts("AT+CWJAP=\"project12\",\"project12345\"\r\n"); delay(3000);	
}
void thingspeak ()
{	
		stringlcd(0x80,"UPDATING TO IOT"); //initial message
	stringlcd(0xc0,"**** PLZ WAIT ****");
				 delay(500);
	/*
	*c = i_to_a (bal);
	*t = i_to_a(tamp);
	*u = i_to_a(units);
	
	strcpy (postStr,A);
  strcat (postStr,u);
	strcat (postStr,B);
	strcat (postStr,c);
	strcat (postStr,C);
	strcat (postStr,t);
	
	n = (strlen(postStr))+5;
	*/
	
	uart1_puts("AT+CIPMUX=0\r\n"); delay(2000);
	uart1_puts("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n\r\n"); delay(2000);	
	/*
	uart1_puts("AT+CIPSEND="); delay(5);
	 uart1_convert(n);delay(5);
	 */
	uart1_puts("AT+CIPSEND=80\r\n"); delay(1000);
	/*
	uart1_puts(postStr); delay(5);
	uart1_puts("\r\n"); delay(2000);
	uart1_puts("AT\r\n"); delay(2000); 
	*/
	uart1_puts("GET /update?api_key=9WZ2EYXEVXCNTL5V&field1="); //delay(2000);
 uart1_convert(units);
 uart1_puts("&field2="); //delay(2000);
 uart1_convert(bal);
 uart1_puts("&field3="); //delay(2000);
 uart1_convert(tamp);
 uart1_puts("\r\n"); delay(2000);
 uart1_puts("AT\r\n"); delay(100); 
 uart1_puts("AT\r\n"); delay(100); 
 uart1_puts("AT\r\n"); delay(100); 
 uart1_puts("AT\r\n"); delay(2000); 
 
 uart1_puts("AT+CIPCLOSE\r\n"); delay(2000); 
	
	stringlcd(0x80,"UPDATING TO IOT"); //initial message
	stringlcd(0xc0,"   COMPLETED   ");
				 blink ();
				 delay(800);
	
}
