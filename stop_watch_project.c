#include <avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
unsigned char sec1=0;
unsigned char sec2=0;

unsigned char min1=0;
unsigned char min2=0;

unsigned char hour1=0;
unsigned char hour2=0;



void enable_INT0(void)
{

	DDRD&=~(1<<2);
	PORTD|=(1<<2);
	GICR|=(1<<6);
	MCUCR|=(1<<ISC01);
	MCUCR&=~(1<<ISC00);

}
void enable_INT1(void)
{

	DDRD&=~(1<<3);
	GICR|=(1<<7);
	MCUCR|=(1<<ISC11)|(1<<ISC10);

}
void enable_INT2(void)
{
	DDRB&=~(1<<2);
	PORTB|=(1<<2);
	GICR|=(1<<5);
	MCUCSR&=~(1<<6);
}


ISR(INT0_vect)
{

	sec1=0;
	sec2=0;

	min1=0;
	min2=0;

	hour1=0;
	hour2=0;
	TCNT1=0;

}

ISR(INT1_vect)
{
	TCCR1B&=(1<< CS10)&(1<<CS12);
}
ISR(INT2_vect)
{
	TCCR1B|=(1<<CS12)|(1<<WGM12)|(1<<CS10);

}



void timer1(void)
{
	TCCR1A|=(1<<FOC1A);
	TCCR1B|=(1<<CS12)|(1<<WGM12)|(1<<CS10);
	TIMSK|=(1<<OCIE1A);

	TCNT1=0;
	OCR1A=977;
	sei();


}

ISR(TIMER1_COMPA_vect)
{

	sec1++;

	if(sec1>9)
	{ sec1=0;
	sec2++;
	if(sec2==6)
	{
	sec2=0;
	min1++;
	if(min1>9)
	{

	min1=0;
	min2++;}
	if(min2==6)
	{
	min2=0;
	hour1++;
	}
	if(hour1>9)
	{hour1=0;
		hour2++;}}}
		if(hour2>9)
		{

			hour2=0;
		}

}




int main(void)
{
	DDRA|=0X3F;
	DDRC|=(1<<0)|(1<<1)|(1<<2)|(1<<3);
	PORTA|=0X3F;
	PORTC=0X00;
	SREG|=(1<<7);

	enable_INT0();
	enable_INT1();
	enable_INT2();
	timer1();







	// Insert code


	while(1)
	{
		PORTA&=0XC0;
		PORTA=0X01;
		PORTC=sec1;
		_delay_ms(2);


		PORTA&=0XC0;
		PORTA=0X02;
		PORTC=sec2;
		_delay_ms(2);

		PORTA&=0XC0;
		PORTA=0X04;
		PORTC=min1;
		_delay_ms(2);

		PORTA&=0XC0;
		PORTA=0X08;
		PORTC=min2;
		_delay_ms(2);

		PORTA&=0XC0;
		PORTA=0X10;
		PORTC=hour1;
		_delay_ms(2);

		PORTA&=0XC0;
		PORTA=0X20;
		PORTC=hour2;
		_delay_ms(2);


	}


}
