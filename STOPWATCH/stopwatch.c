/*
 * stopwatch.c
 *
 *  Created on: Aug 17, 2022
 *      Author: Omar
 */
#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#define SET_BIT(REG,BIT_NUM) (REG |=(1<<BIT_NUM))
#define TOGGLE_BIT(REG,BIT_NUM) (REG ^=(1<<BIT_NUM))

unsigned int minutes=0,seconds=0,COUNT=0;
ISR(TIMER0_OVF_vect){
	COUNT++;
		if(COUNT==4){
			seconds++;
		    COUNT=0;}
		if(seconds==60){
				seconds=0;
				minutes++;
		}
		if(minutes==99){
					minutes=0;
			}
}
ISR(INT0_vect){

	 char hold = 0;
		TOGGLE_BIT(TIMSK,TOIE0);
		while(!(PIND & (1 << PD2))) {
			PORTA = 0;
			_delay_ms(20);
			if(++hold == 30){
				seconds = minutes = COUNT =0;
				break;
			}
		}
}

int main(void){
	DDRC = 0x0F;
	DDRA = 0X0F;
	SET_BIT(TCCR0,FOC0);SET_BIT(TCCR0,CS02);SET_BIT(TCCR0,CS00);
	TCNT0=6;
    SET_BIT(SREG,7);
    SET_BIT(GICR,INT0); SET_BIT(MCUCR,ISC01);
	while(1) {
		            PORTA = 0x01;
					PORTC = seconds % 10;
					_delay_ms(4);
					PORTA = 0x02;
					PORTC = seconds / 10;
					_delay_ms(4);
					PORTA = 0x04;
					PORTC = minutes % 10;
					_delay_ms(4);
					PORTA = 0x08;
					PORTC = minutes / 10;
					_delay_ms(4);
		}


}

