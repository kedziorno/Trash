#define F_CPU 16000000UL // czestotliwosc atmega32 na plytce - od niej zalezy komunikacja rs232 wiec musi byc prawidlowa
#define BAUDRATE 2400 // szybkosc portu szeregowego - 9600bit,bez bitu parzystosci,1 stop bit
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
	char a;
	DDRC=0xff; // diody na wyjscie
	while(1) {
		while(!(UCSRA & _BV(RXC))); // czekamy na odbior znaku z realterm w petli
		a = UDR-48; // odebrany znak w rejestrze UDR, odejmujemy 48 aby dostac integery od 0-7
		PORTC=0x00; // gasimy diody
		switch (a) { // zapalamy diody od 0-7
			case 0 :
				PORTC=1<<0;
				break;
			case 1 :
				PORTC=1<<1;
				break;
			case 2 :
				PORTC=1<<2;
				break;
			case 3 :
				PORTC=1<<3;
				break;
			case 4 :
				PORTC=1<<4;
				break;
			case 5 :
				PORTC=1<<5;
				break;
			case 6 :
				PORTC=1<<6;
				break;
			case 7 :
				PORTC=1<<7;
				break;
			default:
				PORTC=0xff; // gdy rozne od 0-7 to palimy wszystkie ledy
				_delay_ms(1000); // na 1 sekunde
				PORTC=0x00; // a potem gasimy
		}
	}
}

