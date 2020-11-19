#include <avr/io.h>

#define S1 PE4
#define S2 PE5
#define D0 PA0
#define D1 PA1
#define D2 PA2
#define D3 PA3

int main(void)
{
    DDRA = 1<<D0 | 1<<D1 | 1<<D2 | 1<<D3;
	
    while (1) 
    {
		if(!(PINE & (1<<S1)))
			PORTA |= 1<<D0 | 1<<D1;
		else
			PORTA &= ~(1<<D0 | 1<<D1);
		
		if(!(PINE & (1<<S2)))
			PORTA |= 1<<D2;
		else
			PORTA &= ~1<<D2;
			
		if((PINE & (1<<S1)) && (PINE & (1<<S2)))
			PORTA |= 1<<D3;
		else
			PORTA &= ~1<<D3;
    }
}

