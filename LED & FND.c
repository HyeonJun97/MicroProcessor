#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#define PRESCALE 32L    //32분주 Prescale 설정
#define PULSE_PER_OVERFLOW 256L
#define MS_OVERFLOW_CYCLE ((double)(PULSE_PER_OVERFLOW*PRESCALE)/(double)((double)F_CPU/1000.0))
#define NUM_REQ 3

volatile unsigned long timer0;  //overflow마다 1씩 증가될 변수
volatile unsigned long req_timer0[NUM_REQ] = {0, 0, 0};  //요청 변수
volatile unsigned int number;  //증가되어 7-segment LED에 display될 숫자
unsigned char led[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C, 0x07, 0x7F, 0x67};

unsigned long ms_req_timer0(unsigned long ms_interval) //ms_interval 시간 지연을 위한 Overflow 횟수 계산 함수
{
	return (ms_interval<=0.0) ? 0 : (unsigned long)(ms_interval / MS_OVERFLOW_CYCLE);
}

ISR(TIMER0_OVF_vect)  //Timer/Counter0 인터럽트 서비스 루틴
{
	int i;
	timer0++;  //overflow마다 1씩 증가
	if(timer0 % 2 == 0)  //overflow 횟수가 4의 배수일 때 10의자리 or 2의 배수일 때 1자리 display
	{
		PORTC = (timer0%4 == 0) ? led[(number%100)/10] : led[number%10];
		PORTG = (PORTG | 0x03) & (1<<((timer0%4 == 0) ? PG1 : PG0));
	}
	for(i=0; i<NUM_REQ; i++)  //요청 시간 도달 여부 검사 및 응답
		if(req_timer0[i]>0)
			req_timer0[i]--;
}

int main(void)
{
	DDRC = 0xFF;
	DDRG |= 1<<PG1 | 1<<PG0;   //번갈아가며 7-segment LED를 켜기 위한 출력
	DDRA |= 1<<PA7 | 1<<PA6;   //시간 지연에 따라 LED ON/OFF를 위한 출력
	
	TCCR0 = 1<<CS01 | 1<<CS00;  //32분주 prescale 설정 (0,1,1)
	TIMSK |= 1<<TOIE0;          // Timer/Counter0 인터럽트 활성화
	
	timer0 = 0;    //Timer overflow마다 갱신되는 변수
	sei();
	number = 0;
	
	unsigned long ms_overflow0 = ms_req_timer0(500);     //overflow 주기시간 계산1
	unsigned long ms_overflow1 = ms_req_timer0(1000);    //overflow 주기시간 계산2
	unsigned long ms_overflow2 = ms_req_timer0(1000);    //overflow 주기시간 계산3
	
    while (1) 
    {
		if(req_timer0[0]==0)   //500ms 경과 응답 확인
		{
			PORTA^=1<<PA7;    //PA7 출력 반전
			req_timer0[0] = ms_overflow0;  //500ms 지연 요청
		}
		if(req_timer0[1]==0)  //1000ms 경과 응답 확인
		{
			PORTA^=1<<PA6;    //PA6 출력 반전
			req_timer0[1] = ms_overflow1;  //1000ms 지연 요청
		}
		if(req_timer0[2] == 0)  //1000ms 경과 응답 확인
		{
			number++;    //7-segment LED display용 number 증가
			req_timer0[2] = ms_overflow2;  //1000ms 지연 요청
		}
    }
	return 0;
}

