#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

unsigned char digit[] =  {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C, 0x07, 0x7F, 0x67};   //FND 0~9값 설정
unsigned char FND_NUM[] = {0x01, 0x02, 0x04, 0x08};                                      //FND 자리값 설정 

void display_7segled(unsigned char led[], unsigned int number)                           //FND 출력 함수
{
	PORTC = led[number];
}

int main(void)
{
	int sec, i;
	sec = 0;
	i = 0;
	
	DDRC = 0xFF;
	DDRG = 0x0F;
	
	while (1)
	{
		for(i=0; i<500; i++)                    // 1초마다 값이 변하도록 설정하는 FOR문
		{
			PORTG=FND_NUM[2];                   // 1의자리 출력(COM3)
			display_7segled(digit, sec%10);
			_delay_ms(1);
					
			PORTG=FND_NUM[3];                   // 10의자리 출력(COM4)
			display_7segled(digit, sec/10);
			_delay_ms(1);
		}

		sec++;
		
		if(sec==100)                             // SEC값이 100가되면 0으로 초기화
			sec=0;
	}
	return 0;
}