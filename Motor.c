#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

#define PRESSED 1
#define RELEASED 0
#define DEBOUNCE_MS 100

#define MOTOR_FORE 0x80
#define MOTOR_BACK 0x40
#define MOTOR_STOP (1<<PB6 | 1<<PB7)
#define CLEAR (PORTB & 0x3F)

unsigned char digit[] =  {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C, 0x07, 0x7F, 0x67};  //FND 0~9값 설정

void display_7segled(unsigned char led[], unsigned int number)                          //FND 출력 함수
{
	PORTC = led[number];
}

int main(void)
{
	int number, before;
	
	DDRC=0xFF;
	DDRG=0x0F;
	PORTG = 1<<PG0;           //FND 출력핀 설정
	
	DDRE = DDRE & ~(1<<PE4);  //Switch 1의 입력 핀으로 설정
	PORTE = PORTE | 1<<PE4;   //Switch 1에 내부 풀업저항 연결
	DDRE = DDRE & ~(1<<PE5);  //Switch 2의 입력 핀으로 설정
	PORTE = PORTE | 1<<PE5;   //Switch 2의 내부 풀업저항 연결
	
	DDRB |= 1<<PB6 | 1<<PB7;  //Motor드라이버 출력 핀으로 설정
	PORTB=(PORTB & CLEAR) | MOTOR_STOP;
	
	number=0;
	before = RELEASED;
	
	while(1)
	{
		display_7segled(digit, number % 10);
		
		if(before==RELEASED && ! (PINE & 1<<PE4))      //전에 눌리지 않은 상태에서 처음 눌림
		{
			number++;
			_delay_ms(DEBOUNCE_MS);
			before=PRESSED;
		}
		else if(before == PRESSED && (PINE & 1<<PE4))  //전에 눌린 상태에서 처음으로 떨어짐
		{
			_delay_ms(DEBOUNCE_MS);
			before=RELEASED;
		}
		
		if(before==RELEASED && ! (PINE & 1<<PE5))      //전에 눌리지 않은 상태에서 처음 눌림
		{
			number--;
			if(number==-1)                            //number 값이 -가 되는 경우 Error 방지
				number=9;
			_delay_ms(DEBOUNCE_MS);
			before=PRESSED;
		}
		else if(before == PRESSED && (PINE & 1<<PE5))  //전에 눌린 상태에서 처음으로 떨어짐
		{
			_delay_ms(DEBOUNCE_MS);
			before=RELEASED;
		}
		
		if(number % 5 == 2)     //5로 나눈 나머지가 2인 경우 정방향 회전
			PORTB=MOTOR_FORE;
		else if(number % 5==4)  //5로 나눈 나머지가 4인 경우 역방향 회전
			PORTB=MOTOR_BACK;
		else                    //모터정지
			PORTB=(PORTB & CLEAR) | MOTOR_STOP;
	}
	return 0;
}