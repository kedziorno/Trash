#include <stdio.h>
#include <memory.h>

typedef struct {
	unsigned char		Min,Sec,Hour,Day,DOW,Month;
	unsigned short	Year;
} TimeDate;

//TimeDate referenceTimeDate_start = { Min:0, Sec:0, Hour:0, Day:1, Month:1, Year:1970, DOW:1 }; // UNIX EPOCH DATETIME
TimeDate referenceTimeDate_start = { Min:0, Sec:0, Hour:0, Day:1, Month:1, Year:2000, DOW:6 }; // ASSUMING DATETIME

#define MORE 1

typedef struct __attribute__((packed, aligned(1))) {
	unsigned int	__fhalf_packed_datetime;
	unsigned char	__shalf_packed_datetime;
} Packed_TimeDate;

// s:utils
void print_TimeDate(char *msg, TimeDate *referenceTD);
void print_bits(void *pmem, size_t s, int brief);
void print_packetTimeDate(char *msg, Packed_TimeDate *referenceTD);
unsigned long calculateSecondsPackedTimeDate(Packed_TimeDate *currentTD);
int leap_year(unsigned int year);
// e:utils

void setMinutes(Packed_TimeDate *packedTD, unsigned char minutes) {
	packedTD->__fhalf_packed_datetime |= (minutes & 0b00111111) << 25;
}

unsigned char getMinutes(Packed_TimeDate *packedTD) {
	return (packedTD->__fhalf_packed_datetime >> 25) & 0b00111111 ;
}

void setSeconds(Packed_TimeDate *packedTD, unsigned char seconds) {
	packedTD->__fhalf_packed_datetime |= (seconds & 0b00111111) << 19;
}

unsigned char getSeconds(Packed_TimeDate *packedTD) {
	return (packedTD->__fhalf_packed_datetime >> 19) & 0b00111111;
}

void setHour(Packed_TimeDate *packedTD, unsigned char hour) {
	packedTD->__fhalf_packed_datetime |= (hour & 0b00111111) << 13;
}

unsigned char getHour(Packed_TimeDate *packedTD) {
	return (packedTD->__fhalf_packed_datetime >> 13) & 0b00111111;
}

void setDay(Packed_TimeDate *packedTD, unsigned char day) {
	packedTD->__fhalf_packed_datetime |= (day & 0b00011111) << 8;
}

unsigned char getDay(Packed_TimeDate *packedTD) {
	return (packedTD->__fhalf_packed_datetime >> 8) & 0b00011111;
}

void setDOW(Packed_TimeDate *packedTD, unsigned char DOW) {
	packedTD->__fhalf_packed_datetime |= (DOW & 0b00000111) << 5;
}

unsigned char getDOW(Packed_TimeDate *packedTD) {
	return (packedTD->__fhalf_packed_datetime >> 5 ) & 0b00000111;
}

void setMonth(Packed_TimeDate *packedTD, unsigned char month) {
	packedTD->__fhalf_packed_datetime |= (month & 0b00001111) << 0;
}

unsigned char getMonth(Packed_TimeDate *packedTD) {
	return (packedTD->__fhalf_packed_datetime >> 0) & 0b00001111;
}

void setYear(Packed_TimeDate *packedTD, unsigned short year) {
	if (2000 <= year && year < 2100) {
		packedTD->__shalf_packed_datetime |= 0b10000000;
		unsigned short year_difference = year - 2000;
		packedTD->__shalf_packed_datetime |= (year_difference & 0b01111111);
	} else
	if (1900 <= year && year < 2000)  {
		packedTD->__shalf_packed_datetime |= 0b00000000;
		unsigned short year_difference = year - 1900;
		packedTD->__shalf_packed_datetime |= (year_difference & 0b01111111);
	}
}

unsigned short getYear(Packed_TimeDate *packedTD) {
	unsigned short millenium = (packedTD->__shalf_packed_datetime & 0b10000000) ? 2000 : 1900;
	unsigned char years = packedTD->__shalf_packed_datetime & 0b01111111;

	return millenium + years;
}

Packed_TimeDate TimeDateToPacked(TimeDate Td) {
	Packed_TimeDate packedTD;

	memset(&packedTD, 0, sizeof(packedTD));

	setMinutes(&packedTD, Td.Min);
	setSeconds(&packedTD, Td.Sec);
	setHour(&packedTD, Td.Hour);
	setDay(&packedTD, Td.Day);
	setDOW(&packedTD, Td.DOW);
	setMonth(&packedTD, Td.Month);
	setYear(&packedTD, Td.Year);

	return packedTD;
}

TimeDate useReferenceTimeDate() {
	return referenceTimeDate_start;
}

// -- calculate function
unsigned long RtcConvTimeDateToSec(TimeDate Td) {
	Packed_TimeDate currentTD, referenceTD;

	referenceTD = TimeDateToPacked(useReferenceTimeDate());
	currentTD = TimeDateToPacked(Td);

	if (MORE) {
		printf("address             TimeDate = 0x%x\n", &Td);
		printf("address ref. Packed_TimeDate = 0x%x\n", &referenceTD);
		printf("address cur. Packed_TimeDate = 0x%x\n", &currentTD);

		printf("struct size         TimeDate = %d\n", sizeof(TimeDate)); // 8
		printf("struct size  Packed_TimeDate = %d\n", sizeof(Packed_TimeDate)); // 5

		print_TimeDate("Reference TD", &referenceTimeDate_start);
		printf("Reference bits:\n");
		print_bits(&referenceTD, sizeof(Packed_TimeDate), 0);

		print_TimeDate("Current TD", &Td);
		printf("Current bits:\n");
		print_bits(&currentTD, sizeof(Packed_TimeDate), 0);

		print_packetTimeDate("Returned Reference TD", &referenceTD);
		print_packetTimeDate("Returned Current TD", &currentTD);
	}

	unsigned long diff = calculateSecondsPackedTimeDate(&currentTD) - calculateSecondsPackedTimeDate(&referenceTD);

	return diff;
}

// s:utils
void print_bits(void *pmem, size_t s, int brief) {
	for (size_t i = 0; i < s; i++) {
		unsigned char p = (unsigned char *)(pmem + i);
		if (!brief) {
			printf("0x%x 0x%x : ", (pmem + i), p);
		}
		for (size_t i1 = 0; i1 < sizeof(unsigned char) * 8; i1++) {
			if (p & 0x80) { // 0b10000000
				printf("1");
			} else {
				printf("0");
			}
			p <<= 1;
		}
		printf(" ");
	}
	printf("\n");
}

void print_packetTimeDate(char *msg, Packed_TimeDate *referenceTD) {
	printf("%s:\n", msg);
	printf(".Min  = %d\n", getMinutes(referenceTD));
	printf(".Sec  = %d\n", getSeconds(referenceTD));
	printf(".Hour = %d\n", getHour(referenceTD));
	printf(".DOW  = %d\n", getDOW(referenceTD));
	printf(".Day  = %d\n", getDay(referenceTD));
	printf(".Month= %d\n", getMonth(referenceTD));
	printf(".Year = %d\n", getYear(referenceTD));
}

void print_TimeDate(char *msg, TimeDate *referenceTD) {
	printf("%s:\n", msg);
	printf(".Min  = %d\n", referenceTD->Min);
	printf(".Sec  = %d\n", referenceTD->Sec);
	printf(".Hour = %d\n", referenceTD->Hour);
	printf(".DOW  = %d\n", referenceTD->DOW);
	printf(".Day  = %d\n", referenceTD->Day);
	printf(".Month= %d\n", referenceTD->Month);
	printf(".Year = %d\n", referenceTD->Year);
}

unsigned long calculateSecondsPackedTimeDate(Packed_TimeDate *currentTD) {
	// use the algorithm from https://stackoverflow.com/a/24194955

#define DAYS_PER_YEAR			365U
#define DAYS_PER_LEAP			366U
#define SECONDS_PER_DAY		86400UL
#define SECONDS_PER_HOUR	3600UL
#define SECONDS_PER_MIN		60UL

	unsigned long returnDifferenceSeconds = 0;

	unsigned short epoch = 1970;

	unsigned char year_tab[2][12] = {
		{ 31,28,31,30,31,30,31,31,30,31,30,31 },
		{ 31,29,31,30,31,30,31,31,30,31,30,31 }
	};

	returnDifferenceSeconds += getYear(currentTD) * (SECONDS_PER_DAY * DAYS_PER_YEAR);
	unsigned char leap_year_count = 0;
	for (size_t i = 0; i < getYear(currentTD) - epoch; i++) {
		if (leap_year(epoch+i))
			leap_year_count++;
	}
	returnDifferenceSeconds += leap_year_count * SECONDS_PER_DAY;

	returnDifferenceSeconds += getSeconds(currentTD);
	returnDifferenceSeconds += getHour(currentTD) * SECONDS_PER_HOUR;
	returnDifferenceSeconds += getMinutes(currentTD) * SECONDS_PER_MIN;
	returnDifferenceSeconds += getDay(currentTD) * SECONDS_PER_DAY;

	if (getMonth(currentTD) > 1) {
		char leap = 0;
		if (leap_year(getYear(currentTD))) {
			if (getMonth(currentTD) > 2) {
				leap = 1;
			} else
			if (getMonth(currentTD) == 2 && getDay(currentTD) >= 29) {
				leap = 1;
			}
		}
		for (size_t i = 0; i < getMonth(currentTD) - 1; i++) {
			returnDifferenceSeconds += year_tab[leap][i] * SECONDS_PER_DAY;
		}
	}

	returnDifferenceSeconds += 1 * SECONDS_PER_DAY; // day of 'orphan'

	//returnDifferenceSeconds = getSeconds(currentTD) + getMinutes(currentTD)*60 + getHour(currentTD)*3600 + getDay(currentTD)*86400 + (getYear(currentTD)-70)*31536000 + ((getYear(currentTD)-69)/4)*86400 - ((getYear(currentTD)-1)/100)*86400 + ((getYear(currentTD)+299)/400)*86400;

	return returnDifferenceSeconds;
}

int leap_year(unsigned int year) {
	// calculate leap year from https://stackoverflow.com/a/725111
	if ((!(year % 4) && (year % 100)) || !(year % 400)) {
		return 1;
	} else {
		return 0;
	}
}

// e:utils

int main() {
	printf("Hello World!\n");

	TimeDate td;
	td.Min = 07;
	td.Sec = 14;
	td.Hour = 17;
	td.Day = 16;
	td.DOW = 4;
	td.Month = 4;
	td.Year = 2020;

	printf("%lld\n", RtcConvTimeDateToSec(td));

	return 0;
}
