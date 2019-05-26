#include <lpc214x.h>
#include "gpio.h"


void ADC0_Init(void)
{
 
 // Enable ADC0
 AD0CR &= 0x00;		   // Clear All Bit Control
 AD0CR |= CLK_5;		// ADC Clock = VPB(PCLK) / 4
 AD0CR &= BURST_OFF;		// Busrt = 0		- Conversions are software controlled and reguire 11 clocks
 AD0CR &= BURST_ON ;		// CLKS  = 000		- 10Bit : 11 Cycle Clock Conversion	   19:17    0001 = 1
 AD0CR |= PDN_ON;		// PDN   = 1			- Active ADC Module

}

void ADC1_Init(void)
{
 
 // Enable ADC1
 AD1CR &= 0x00;		   // Clear All Bit Control
 AD1CR |= CLK_5;		// ADC Clock = VPB(PCLK) / 4
 AD1CR &= BURST_OFF;		// Busrt = 0		- Conversions are software controlled and reguire 11 clocks
 AD1CR &= BURST_ON ;		// CLKS  = 000		- 10Bit : 11 Cycle Clock Conversion	   19:17    0001 = 1
 AD1CR |= PDN_ON;		// PDN   = 1			- Active ADC Module

}

unsigned int adc1_channel_2(void)
{
 unsigned int	val;
 
 PINSEL0 |= AD1_2;	    
 
 AD1CR &= ADC1_CH_DISABLE;
 AD1CR   |= CH_2;		
 AD1CR   |= ADC_start;	
 
 while(ADC1_Wait_CH2 == 0);	
 val = (unsigned int)Result1_CH2; 
 ADC_Stop;   
 return (val);
} 
unsigned int adc1_channel_3(void)
{
 unsigned int	val;
 
 PINSEL0 |= AD1_3;	    
 
 AD1CR &= ADC1_CH_DISABLE;
 AD1CR   |= CH_3;		
 AD1CR   |= ADC_start;	
 
 while(ADC1_Wait_CH3 == 0);	
 val = (unsigned int)Result1_CH3; 
 ADC_Stop;   
 return (val);
} 


unsigned int adc0_channel_1(void)
{

 unsigned int	val;
 

 PINSEL1 |= AD0_1;	                   //  channel 0 in adc0

 AD0CR &= ADC0_CH_DISABLE;
 AD0CR   |= CH_1;		               // Select ADC = AD0.1  
 AD0CR   |= ADC_start;	               // START = 001 = Start Conversion Now
 
 while(ADC0_Wait_CH1 == 0);	          // Wait ADC Conversion to Complete and Read A/D Data Register
 val = (unsigned int)Result0_CH1;     // Shift ADC Result to Integer	  // 15:6
 ADC_Stop;                           //stop =000
 return (val);
} 

unsigned int adc0_channel_2(void)
{
 unsigned int	val;
 
 PINSEL1 |= AD0_2;	    
 
 AD0CR &= ADC0_CH_DISABLE;
 AD0CR   |= CH_2;		
 AD0CR   |= ADC_start;	
 
 while(ADC0_Wait_CH2 == 0);	
 val = (unsigned int)Result0_CH2; 
 ADC_Stop;   
 return (val);
} 

unsigned int adc0_channel_3(void)
{
 unsigned int	val;
 
 PINSEL1 |= AD0_3;	    
 
 AD0CR &= ADC0_CH_DISABLE;
 AD0CR   |= CH_3;		
 AD0CR   |= ADC_start;	
 
 while(ADC0_Wait_CH3 == 0);	
 val = (unsigned int)Result0_CH3; 
 ADC_Stop;   
 return (val);
} 

unsigned int adc0_channel_4(void)
{
 unsigned int	val;
 
 PINSEL1 |= AD0_4;	    
 
 AD0CR &= ADC0_CH_DISABLE;
 AD0CR   |= CH_4;		
 AD0CR   |= ADC_start;	
 
 while(ADC0_Wait_CH4 == 0);	
 val = (unsigned int)Result0_CH4; 
 ADC_Stop;   
 return (val);
} 


