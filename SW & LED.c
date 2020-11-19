#include <avr/io.h>
#include <avr/interrupt.h>

#define REQUEST 1
#define NOT_REQUEST -1
#define REPLY 0

volatile int req_INT4 = REQUEST;
volatile int req_INT5 = NOT_REQUEST;
unsigned char LED_NUM[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};  //LED 위치 지정 배열
volatile int check = 0;                                                      //LED 위치 지정 변수

ISR(INT4_vect)   //SW1 인터럽트
{
	if(req_INT4==REQUEST)           //REQUEST = 1
		req_INT4 = REPLY;          //REPLY = 0
	check++;
}

ISR(INT5_vect)   //SW2 인터럽트
{
	if(req_INT5==REQUEST)
		req_INT5 = REPLY;
	check--;
}

int main(void)
{
	EIMSK |= 1<<INT4 | 1<<INT5;   //외부 인터럽트4, 외부 인터럽트5 활성화
	PORTE |= 1<<INT4 | 1<<INT5;   //INT4, INT5 핀 내부에서 풀업 저항 연결
	EICRB = 3<<ISC40 | 2<<ISC50;  //INT4은 상승에지, INT5은 하강에지에 Interrupt
	DDRA = 0xFF;                  //LED 출력 설정
	sei();

	
    while (1) 
    {
		if(req_INT4==REPLY)
		{
			req_INT4 = NOT_REQUEST;    //req_INT4 요청 해지
			req_INT5 = REQUEST;        //req_INT5 요청
		}
		
		else if(req_INT5==REPLY)
		{
			req_INT4 = REQUEST;        //req_INT4 요청
			req_INT5 = NOT_REQUEST;    //req_INT5 요청 해지
		}
		
		PORTA = LED_NUM[check];
		
		if(check==-1)                 //Check 값 오류 방지
			check=7;
		
		if(check==8)                  //Check 값 오류 방지
			check=0; 
    }
}

