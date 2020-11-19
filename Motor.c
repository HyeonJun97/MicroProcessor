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

unsigned char digit[] =  {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C, 0x07, 0x7F, 0x67};  //FND 0~9�� ����

void display_7segled(unsigned char led[], unsigned int number)                          //FND ��� �Լ�
{
	PORTC = led[number];
}

int main(void)
{
	int number, before;
	
	DDRC=0xFF;
	DDRG=0x0F;
	PORTG = 1<<PG0;           //FND ����� ����
	
	DDRE = DDRE & ~(1<<PE4);  //Switch 1�� �Է� ������ ����
	PORTE = PORTE | 1<<PE4;   //Switch 1�� ���� Ǯ������ ����
	DDRE = DDRE & ~(1<<PE5);  //Switch 2�� �Է� ������ ����
	PORTE = PORTE | 1<<PE5;   //Switch 2�� ���� Ǯ������ ����
	
	DDRB |= 1<<PB6 | 1<<PB7;  //Motor����̹� ��� ������ ����
	PORTB=(PORTB & CLEAR) | MOTOR_STOP;
	
	number=0;
	before = RELEASED;
	
	while(1)
	{
		display_7segled(digit, number % 10);
		
		if(before==RELEASED && ! (PINE & 1<<PE4))      //���� ������ ���� ���¿��� ó�� ����
		{
			number++;
			_delay_ms(DEBOUNCE_MS);
			before=PRESSED;
		}
		else if(before == PRESSED && (PINE & 1<<PE4))  //���� ���� ���¿��� ó������ ������
		{
			_delay_ms(DEBOUNCE_MS);
			before=RELEASED;
		}
		
		if(before==RELEASED && ! (PINE & 1<<PE5))      //���� ������ ���� ���¿��� ó�� ����
		{
			number--;
			if(number==-1)                            //number ���� -�� �Ǵ� ��� Error ����
				number=9;
			_delay_ms(DEBOUNCE_MS);
			before=PRESSED;
		}
		else if(before == PRESSED && (PINE & 1<<PE5))  //���� ���� ���¿��� ó������ ������
		{
			_delay_ms(DEBOUNCE_MS);
			before=RELEASED;
		}
		
		if(number % 5 == 2)     //5�� ���� �������� 2�� ��� ������ ȸ��
			PORTB=MOTOR_FORE;
		else if(number % 5==4)  //5�� ���� �������� 4�� ��� ������ ȸ��
			PORTB=MOTOR_BACK;
		else                    //��������
			PORTB=(PORTB & CLEAR) | MOTOR_STOP;
	}
	return 0;
}