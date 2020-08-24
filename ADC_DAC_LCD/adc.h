#ifndef _ADC_H
#define _ADC_H


#define END_0F_CONVERSION_BIT		(1<<31)
#define END_OF_CONVERSION(i)		(i & END_0F_CONVERSION_BIT)
#define ADC_VALUE_MASK                  0x03FF
#define ADC_CHANNEL_NUMBER_MASK		0x07


#define CHANNEL_0   0
#define CHANNEL_1   1
#define CHANNEL_2   2
#define CHANNEL_3   3
#define CHANNEL_4   4
#define CHANNEL_5   5
#define CHANNEL_6   6
#define CHANNEL_7   7


/* A/D Control Register */

#define AD0_0		0x00000001
#define AD0_1		0x00000002
#define AD0_2		0x00000004
#define AD0_3		0x00000008
#define AD0_4		0x00000010
#define AD0_5		0x00000020
#define AD0_6		0x00000040
#define AD0_7		0x00000080

#define AD1_0		0x00000001
#define AD1_1		0x00000002
#define AD1_2		0x00000004
#define AD1_3		0x00000008
#define AD1_4		0x00000010
#define AD1_5		0x00000020
#define AD1_6		0x00000040
#define AD1_7		0x00000080


#define CLKDIV_BIT0	(1<<8)
#define CLKDIV_BIT1	(1<<9)
#define CLKDIV_BIT2	(1<<10)
#define CLKDIV_BIT3	(1<<11)
#define CLKDIV_BIT4	(1<<12)
#define CLKDIV_BIT5	(1<<13)
#define CLKDIV_BIT6	(1<<14)
#define CLKDIV_BIT7	(1<<15)
#define BURST		(1<<16)		//to eneble burst mode
#define CLKS_BIT0	(1<<17)
#define CLKS_BIT1	(1<<18)
#define CLKS_BIT2	(1<<19)
#define PDN 		(1<<21)
#define START_BIT0 	(1<<24)
#define START_BIT1 	(1<<25)
#define START_BIT2 	(1<<26)
#define EDGE 		(1<<27)

/* A/D Global Data Register */

#define OVERRUN		(1L<<30)
#define DONE		(1L<<31)



/* A/D Interrupt Enable Register  */

#define ADINTEN0 	(1<<0)
#define ADINTEN1 	(1<<1)
#define ADINTEN2 	(1<<2)
#define ADINTEN3 	(1<<3)
#define ADINTEN4 	(1<<4)
#define ADINTEN5 	(1<<5)
#define ADINTEN6 	(1<<6)
#define ADINTEN7 	(1<<7)

#define ADGINTEN	(1<<8)

#define ADC0 0
#define ADC1 1
#define ADC0CHANNELS  8
#define ADC1CHANNELS  8

#define POWER_DOWN_ADC0()    AD0CR &= ~(PDN)
#define POWER_UP_ADC0()      AD0CR |= (PDN)

#define POWER_DOWN_ADC1()    AD1CR &= ~(PDN)
#define POWER_UP_ADC1()      AD1CR |= (PDN)

//TN_EVENT EVT_ADC0;
//TN_EVENT EVT_ADC1;

void adc0_isr(void);
void adc1_isr(void);
void init_adc0( void );
void init_adc1( void );
unsigned short adc_read(unsigned char adc_num, unsigned char ch);

#endif
