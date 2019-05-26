#include<lpc214x.h>
#include "gpio.h"
// function prototype declarations 



// Define LCD PinIO Mask 
//jagadeesh
#define  LCD_RS     P1_16   						// P1.24
#define  LCD_EN     P1_17   						// P1.26

#define  LCD_D4     P1_18   						// P1.20
#define  LCD_D5     P1_19 						    // P1.21
#define  LCD_D6     P1_20  						    // P1.22
#define  LCD_D7     P1_21   						// P1.23

#define  LCD_RS_on	(IOSET1 |= LCD_RS)
#define  LCD_RS_off	(IOCLR1 |= LCD_RS)

#define  LCD_EN_on  (IOSET1 |= LCD_EN)
#define  LCD_EN_off	(IOCLR1 |= LCD_EN)

void delay( unsigned int value )
{
 unsigned int ui_temp1,ui_temp2;
 for(ui_temp1=0;ui_temp1<value;ui_temp1++)
 for(ui_temp2=0;ui_temp2<5000;ui_temp2++);
}

void clcd(unsigned char val)
{
 unsigned int lcd_ch;			   // LCD Initial Data 
 unsigned int lcd_i;			   // LCD Initial Delay Count 

 IOCLR1 = LCD_RS ;					// RS = 0 
 
 lcd_ch=((val>>4)&0x0F);			// Strobe 4-Bit High-Nibble to LCD

 IOCLR1 = (LCD_D7|LCD_D6|LCD_D5|LCD_D4);	 // Reset 4-Bit Pin Data
 IOSET1 = (lcd_ch<<18); 			    	// Data Send to Respective Pins    
 IOSET1 = LCD_EN ;					// EN = 1 (Enable)
 for (lcd_i=0;lcd_i<10000;lcd_i++);	//delay
 IOCLR1 = LCD_EN ;					// EN = 0 (Disable)	
  
 lcd_ch=(val&0x0F);				  		// Strobe 4-Bit Low-Nibble to LCD

 IOCLR1 = (LCD_D7|LCD_D6|LCD_D5|LCD_D4); // Reset 4-Bit Pin Data
 IOSET1 = (lcd_ch<<18); 				// EN,0,RW,RS:DDDD:0000:0000:0000:0000:0000:0000   
 IOSET1 = LCD_EN ;					// EN = 1 (Enable)
 for (lcd_i=0;lcd_i<10000;lcd_i++);	//delay
 IOCLR1 = LCD_EN ;					// EN = 0 (Disable)	
  
 for (lcd_i=0;lcd_i<10000;lcd_i++);	 //delay
}

void dlcd(unsigned char val)
{
 unsigned int lcd_ch;		   // LCD Initial Data 
 unsigned int lcd_i;		   // LCD Initial Delay Count 

 IOSET1 = LCD_RS ;	 		// RS = 1 
 
 lcd_ch=((val>>4)&0x0F);	// Strobe 4-Bit High-Nibble to LCD

 IOCLR1 = (LCD_D7|LCD_D6|LCD_D5|LCD_D4); // Reset 4-Bit Pin Data
 IOSET1 = (lcd_ch<<18); 				 // Data Send to Respective Pins  
 IOSET1 = LCD_EN ;					// EN = 1 (Enable)
 for (lcd_i=0;lcd_i<10000;lcd_i++);	// delay
 IOCLR1 = LCD_EN ;					// EN = 0 (Disable)	
  
 lcd_ch=(val&0x0F);				  	// Strobe 4-Bit Low-Nibble to LCD

 IOCLR1 = (LCD_D7|LCD_D6|LCD_D5|LCD_D4); // Reset 4-Bit Pin Data
 IOSET1 = (lcd_ch<<18); 				 // Data Send to Respective Pins   
 
 IOSET1 = LCD_EN ;					// EN = 1 (Enable)
 for (lcd_i=0;lcd_i<10000;lcd_i++);	//delay
 IOCLR1 = LCD_EN ;					// EN = 0 (Disable)	
  
 for (lcd_i=0;lcd_i<10000;lcd_i++);	 //delay
}

void initlcd()
{
 unsigned int i;										// LCD Initial Delay Count 
 
 PINSEL2 |= 0x00000000;				// GPIO1[31..20] = I/O Function
 IODIR1  |= 0x003F0000 ;

 for (i=0;i<10000;i++);								// Power-On Delay (15 mS)

 IOCLR1 = ((LCD_D7|LCD_D6|LCD_D5|LCD_D4|LCD_RS|LCD_EN));									// Reset (RS,RW,EN,4-Bit Data) Pin
 IOSET1 = (LCD_D5|LCD_D4);								// DDDD:EN,RW,RS,0:0000:0000:0000:0000:0000:0000 
 LCD_EN_on;					// EN = 1 (Enable)
 for (i=0;i<10000;i++);
 LCD_EN_off;					// EN = 0 (Disable)	
 for (i=0;i<10000;i++);								// Delay 4.1mS

 IOCLR1 = ((LCD_D7|LCD_D6|LCD_D5|LCD_D4|LCD_RS|LCD_EN));	  								// Reset (RS,RW,EN,4-Bit Data) Pin
 IOSET1 = (LCD_D5|LCD_D4);								// DDDD:EN,RW,RS,0:0000:0000:0000:0000:0000:0000 
 LCD_EN_on;					// EN = 1 (Enable)
 for (i=0;i<10000;i++);
  LCD_EN_off;					// EN = 0 (Disable)	
 for (i=0;i<10000;i++);									// delay 100uS
  
 IOCLR1 = ((LCD_D7|LCD_D6|LCD_D5|LCD_D4|LCD_RS|LCD_EN));	  								// Reset (RS,RW,EN,4-Bit Data) Pin
 IOSET1 = (LCD_D5|LCD_D4);								// DDDD:EN,RW,RS,0:0000:0000:0000:0000:0000:0000 
 LCD_EN_on;						// EN = 1 (Enable)
 for (i=0;i<10000;i++);
 LCD_EN_off;					// EN = 0 (Disable)	
 for (i=0;i<10000;i++);					  
 
 IOCLR1 = ((LCD_D7|LCD_D6|LCD_D5|LCD_D4|LCD_RS|LCD_EN));	  								// Reset (RS,RW,EN,4-Bit Data) Pin
 IOSET1 = (LCD_D5);									// DDDD:EN,RW,RS,0:0000:0000:0000:0000:0000:0000 
 LCD_EN_on;					// EN = 1 (Enable)
 for (i=0;i<10000;i++);
 LCD_EN_off;					// EN = 0 (Disable)	
 for (i=0;i<10000;i++);	  
 			
 clcd(0x02);	
 clcd(0x02);
 clcd(0x28);  //intialise the lcd in 4 bit mode*/
 clcd(0x28);  //intialise the lcd in 4 bit mode*/
 
 clcd(0x0e);	//cursor blinking
 clcd(0x06);	//move the cursor to right side
 clcd(0x01);	//clear the lcd
	
}
  

void stringlcd(unsigned char ch,unsigned char str[])
{
 unsigned char i,Count=0;
 if(ch==0x80){clcd(0x01);}
 clcd(ch);
 

  for(i=0;str[i]!='\0';i++)
  {
  	Count=Count+1;
	
	dlcd(str[i]);
  
	if(Count == 20)
	 clcd(0xc0);
	if(Count >= 32)
	{
	 Count = 0;
	 clcd(0x01);
	}
  }

}

// CONVERT HEX DECIMAL TO ASCII VALUE
 
 void conv(unsigned int temp1_value)  
		  {
            unsigned char value,d1,d2,d3,d4,val1;
              value=temp1_value/10;
              d4=temp1_value%10;
              val1=value/10;
              d3=value%10;
			  d2=val1%10;
			  d1=val1/10;
			   
             dlcd(d1+48);
             dlcd(d2+48);
             dlcd(d3+48);
	         dlcd(d4+48);


          }

 
