/*
BLIND STICK WITH GPS AND IOT

LPC2148

https://thingspeak.com/channels/636266

GPS -- UART 0
IOT -- UART 1 
MEMS -- AD0.1

MANHOLE SENSOR(IR) -- P0.2
OBSTACLE SENSOR(IR) -- P0.3
WATER SENSOR -- P0.4
BUZZER - P0.5
LCD
APR VOICE MODULE
	1.WELCOME MSG -- P.10
	2.MEMS ALERT -- P.11
	3.OBSTACLE ALERT -- P.12
	4.MANHOLE ALERT -- P.13
	5.WATER ALERT -- P.14
	



***********************************************************************************************/
#include <lpc214x.h>		 //21007BB4B15F         18008D991A16
#include <stdio.h>
#include <string.h>
#include "lcd.h"
//#include "serial.h"
#include "gpio.h"
#include "adc0.h"
#include "uart1.h"
#include "uart0.h"

#define ON  '1'//0x31
#define OFF '0' //0x30

#define manhole P0_2

#define obstacle P0_3

#define water P0_4

#define buzzer P0_5

#define v0 P0_10
#define v1 P0_11
#define v2 P0_12
#define v3 P0_13
#define v4 P0_14

//void serial0_RxISR(void) __irq; 



void wifiinit(void) ;
void thingspeak (void) ;
void getgps (void);

unsigned int x;
unsigned int f = 4;

unsigned int obs;
unsigned int man;
unsigned int wat;
unsigned int fal;

unsigned char rcv,rcv1,rcv2,rcv3,rcv4,rcv5,msg1[15],msg2[15];
unsigned int r;

////////////ADC FUNCTIONS STOP/////////////////////
void pin()
{
 PINSEL0=0X00050005;//SELECTING PIN OPERATION
 PINSEL1=0X15000000;
 PINSEL2=0X00000000;
 //IOSET0=sensor1|sensor2;
	
	IODIR0 &= ~(1<<2); // input -- manhole
	IODIR0 &= ~(1<<3); // input -- obstacle
	IODIR0 &= ~(1<<4); // input -- water
	
	IODIR0 |= (1<<5);		// OUTPUT -- buzzer
	
	IODIR0 |= (1<<10);		//OUTPUT -- welcome message (apr module)
	IODIR0 |= (1<<11);		// OUTPUT -- (apr module)
	IODIR0 |= (1<<12);		//OUTPUT -- (apr module)
	IODIR0 |= (1<<13);		//OUTPUT -- (apr module)
	IODIR0 |= (1<<14);		//OUTPUT -- (apr module)
	
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

int main ()
{
	pin();
	
	initlcd();
	ADC0_Init();
	uart0_init();
uart1_init ();
	
	
	//// VOICE MODULE 
	IOSET0=v0;
	IOSET0=v1;
	IOSET0=v2;
	IOSET0=v3;
	IOSET0=v4;
	
	IOSET0=buzzer;
	delay(400);
	
	IOCLR0=buzzer;
	delay(400);
	
	IOSET0=buzzer;
	delay(400);
	
	IOCLR0=buzzer;
	delay(400);
	
	//SERIAL AND LCD INITIALIZING 
	//ser_init_0(0x61,0x00);
	//ser_init_1(0x61,0x00); //serial initilization
	
//WELCOME MESSAGE...	 
	stringlcd(0x80," SMART  WALKING "); //initial message
	stringlcd(0xc0,"STICK FOR BLIND ");
	
	IOCLR0=v0;
	delay(2000);
	IOSET0=v0;
				 
				// wifiinit ();
	while (1)
	{
		obs = 0;
		man = 0;
		wat = 0;
		fal = 0;
		
		x = adc0_channel_1();
	 /*
		if ((x < 250) || (x > 400))
		{
			f = 1;
			fal = 100;
			clcd(1);
  stringlcd(0x80," FALLING ALERT  "); 
	stringlcd(0xC0,"PLEASE CHECK AT "); 
sound ();		 
			IOCLR0 = v1;
		 delay(1500);
			IOSET0 = v1;
			getgps ();
		}
		*/
		
		if (inp0(manhole)==0)
		{
			f = 1;
			man = 100;
			clcd(1);
  stringlcd(0x80,"MANHOLE DETECTED"); 
	stringlcd(0xC0,"PLEASE CHECK AT ");  
sound ();		
		 IOCLR0 = v2;
		 delay(1500);
			IOSET0 = v2;
				getgps ();
		}
		
		if (inp0(obstacle)== 1)
		{
			f = 1;
			obs = 100;
			clcd(1);
  stringlcd(0x80,"OBSTACLE DETECTED"); 
	stringlcd(0xC0,"PLEASE CHECK AT "); 
sound ();	
		 IOCLR0 = v3;
		 delay(1500);
			IOSET0 = v3;		
				getgps ();
		}
		
		if (inp0(water)== 0)
		{
			f = 1;
			wat = 100;
			clcd(1);
  stringlcd(0x80," WATER DETECTED "); 
			stringlcd(0xC0,"PLEASE CHECK AT "); 
sound ();		 
		 IOCLR0 = v4;
		 delay(1500);
			IOSET0 = v4;	
				getgps ();
		}
	
	 stringlcd(0x80,"F:");	clcd(0x82);conv(x);
	 stringlcd(0x89,"O:");  clcd(0x8C);conv(inp0(obstacle)); //stringlcd(0x8C,"0");
   stringlcd(0xC0,"M:");  clcd(0xC3);conv(inp0(manhole)); //stringlcd(0xc3,"0");
	 stringlcd(0xC9,"W:");  clcd(0xCC);conv(inp0(water)); 
	 delay(2000);		
	
		if (f == 1)
		{
			f = 0;
			stringlcd(0x80,"WI-FI updating"); //initial message
	stringlcd(0xc0,"**** PLZ WAIT ****");
				 delay(2000);
		//thingspeak () ;
		}
		
	}//while
	
}// main


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
	
	uart1_puts("AT+CIPMUX=0\r\n"); delay(2000);
	uart1_puts("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n\r\n"); delay(2000);	
	
	uart1_puts("AT+CIPSEND=90\r\n"); delay(1000);

	uart1_puts("GET /update?api_key=9WZ2EYXEVXCNTL5V&field1="); //delay(2000);
 uart1_convert(fal);
 uart1_puts("&field2="); //delay(2000);
 uart1_convert(man);
 uart1_puts("&field3="); //delay(2000);
 uart1_convert(obs);
	uart1_puts("&field4="); //delay(2000);
 uart1_convert(wat);
 uart1_puts("\r\n"); delay(2000);
 uart1_puts("AT\r\n"); delay(100); 
 uart1_puts("AT\r\n"); delay(100); 
 uart1_puts("AT\r\n"); delay(100); 
 uart1_puts("AT\r\n"); delay(2000); 
 
 uart1_puts("AT+CIPCLOSE\r\n"); delay(2000); 
	
	stringlcd(0x80,"UPDATING TO IOT"); //initial message
	stringlcd(0xc0,"   COMPLETED   ");
				 delay(800);
	
}

void getgps ()
{
  uart0_puts("Receiving..");
do{
    	 rcv=uart0_getch();
      }while(rcv != '$'); 


	     rcv1=uart0_getch();
		 rcv2=uart0_getch();
		 rcv3=uart0_getch();
		 rcv4=uart0_getch();
		 rcv5=uart0_getch(); 

		 if((rcv1 == 'G') && (rcv2 == 'P') && (rcv3 == 'R') && (rcv4 == 'M') && (rcv5 == 'C')) 
            {
			  for(r=0;r<13;r++)
			    {
				  rcv=uart0_getch();
				  uart0_putch(rcv);
					//	uart0_putch("\r\n");
				 // delay(50);
				}
              for(r=0;r<14;r++)
			    {
				  msg1[r]=uart0_getch();
				   uart0_putch(msg1[r]);
					//	uart0_putch("\r\n");
				  // delay(50);
				}
				 
			  for(r=0;r<10;r++)
			    {
				  msg2[r]=uart0_getch();
				   uart0_putch(msg2[r]);
						//uart0_putch("\r\n");
				  // delay(50);
				}
			
  /*
  lcdcmd(0x85);
   msgdisplay(msg1);
   delay(50);
   lcdcmd(0xc5);
   msgdisplay(msg2);
				*/
				
				stringlcd(0x80,"LAT:");
				stringlcd(0x85,msg1);
				stringlcd(0xC0,"LON:");
				stringlcd(0xC5,msg2);
				delay(4000);
	}
}