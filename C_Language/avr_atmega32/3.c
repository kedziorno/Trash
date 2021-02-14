#define F_CPU 16000000UL // czestotliwosc atmega32 na plytce - od niej zalezy komunikacja rs232 wiec musi byc prawidlowa
#define BAUDRATE 1200 // szybkosc portu szeregowego - 9600bit,bez bitu parzystosci,1 stop bit
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
	UCSRB=_BV(TXEN); // wlaczamy odbior i wysylanie UART
}

void adc_init() {
	ADMUX = _BV(REFS1)|_BV(REFS0);
	ADCSRA = _BV(ADEN)|_BV(ADPS2)|_BV(ADPS1)|_BV(ADPS0)|_BV(ADSC);
}

uint16_t ReadADC(uint8_t ADCchannel) {
 ADMUX = (ADMUX&0xF0)|(ADCchannel&0x0F);
 ADCSRA |= (1<<ADSC);
 while(ADCSRA&(1<<ADSC));
 return ADC;
}
void tx_char(char c) {
	while(!(UCSRA&_BV(UDRE)));
	UDR = c;
}

int main() {
	DDRA=0x00;
	uart_init();
	adc_init();
	while(1) {
		#define NR_CHARS 5
		char a[NR_CHARS];
		memset(a,'\x00',NR_CHARS);
		int v=ReadADC(4);
		int f;
		sprintf(a,"%d\r\n",v);
		for(int i=0;i<NR_CHARS;i++) {
			tx_char(a[i]);
		}
		_delay_ms(3000);
	}
}

