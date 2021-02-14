#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
unsigned int O1=1*64;
unsigned int O2=2*64;
unsigned int O3=3*64;
unsigned int O4=4*64;
int main (void) {
	DDRB=(1<<PB3);
	TCCR0=(1 << WGM01)|(1 << WGM00)|(1 << COM01)|(1 << CS02)|(1 << CS00);
	while(1) {
		OCR0=O1; // 25% przez 1s
		_delay_ms(1000);
		OCR0=O2; // 50% przez 1s
		_delay_ms(1000);
		OCR0=O3; // 75% przez 1s
		_delay_ms(1000);
		OCR0=O4; // 100% przez 1s
		_delay_ms(1000);
	}
}

