#define F_CPU 16000000UL // czestotliwosc atmega32 na plytce - od niej zalezy komunikacja rs232 wiec musi byc prawidlowa
#define BAUDRATE 9600 // szybkosc portu szeregowego - 9600bit,bez bitu parzystosci,1 stop bit
#define BAUDPRESCALE ((F_CPU/(BAUDRATE*16UL)))-1 // liczymy prescaler z sybkosci cpu i szybkoscu uart/rs232 (tu: kiedy stopbit ma 2, czyli flaga U2X w UCSRB/UCSRC ustawiona mnozymy przez 8UL a nie 16UL) 

#include <avr/io.h>
#include <util/delay.h>

static void uart_baud() {
	UBRRL = BAUDPRESCALE; // mlodszy bajt preskalera 16bit - 8bit
	UBRRH = (BAUDPRESCALE >> 8); // starszy bajt preskalera 16bit - 8 bit
}

static void uart_init() {
	uart_baud();
	UCSRC=_BV(UCSZ1)|_BV(UCSZ0)|_BV(URSEL);
	UCSRB=_BV(TXEN)|_BV(RXEN); // wlaczamy odbior i wysylanie UART
}

int main() {
	uart_init();
	while(1) {
		while(!(UCSRA & _BV(RXC)));
		a = UDR;
		UDR = a;
		while(!(UCSRA & _BV(UDRE)));
	}
}

