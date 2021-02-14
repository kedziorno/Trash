#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// 6000000us/255=23530us

int main (void) {
	DDRB=(1<<PB3);
	TCCR0=(1 << WGM01)|(1 << WGM00)|(1 << COM01)|(1 << CS02)|(1 << CS00);
	while(1) {
		while (OCR0<255) {
			OCR0++;
			_delay_us(23530);
		}
		while (OCR0>0) {
			OCR0--;
			_delay_us(23530);
    }
	}
}
