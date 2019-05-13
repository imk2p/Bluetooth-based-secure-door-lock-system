#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#include"sbit.h"
#include<string.h>
#include<stdlib.h>
#include"lcd.h"

#define F_CPU 8000000UL
#define BAUD_PRESCALE_BT (((F_CPU / (9600UL * 16UL))) - 1)
#define BAUD_PRESCALE_GSM (((F_CPU / (9600UL * 16UL))) - 1)

#define RELAY 			SBIT(PORTD,5)
#define RELAY_DIR		SBIT(DDRD,5)

#define IR 				SBIT(PINA,2)
#define IR_DIR			SBIT(DDRA,2)

#define RED_LED			SBIT(PORTC,1)
#define GREEN_LED		SBIT(PORTC,0)
#define RL_DIR			SBIT(DDRC,1)
#define GL_DIR			SBIT(DDRC,0)

#define BUZ				SBIT(PORTA,0)

int remaining_coin = 10;
int on_time = 0;
int timeout = 200;

enum{IR_DET,START_ELEC};

char pass[]="1234";

volatile unsigned char received_pass[8];
volatile int indx = 0;

volatile int show = 0;

int wrong_attempts = 0;

int msg_done = 1;

void USART_Init();

void USART_Init()
{
	UCSRB |= (1<<TXEN)|(1 << RXEN)|(1 << RXCIE);
	UCSRC |= (1 << URSEL)| (1 << UCSZ0) | (1 << UCSZ1);
	UBRRL = BAUD_PRESCALE_BT;
	UBRRH = (BAUD_PRESCALE_BT >> 8);
	//UCSRB |= (1 << RXCIE);
}
void send_string(char * str)
{
	unsigned char shifts, length;
	length = strlen(str);
     for(shifts = 0; shifts < length; shifts++)
	{
    	 while(!( UCSRA & (1<<UDRE)));
			UDR = str[shifts];
	}
}

void send_MSG(char *msg_string)
{
	//	cli();
		char j;
		for(j=0;j<2;j++)
	    {
				_delay_ms(1000);
				send_string("AT\r");
	    }
		_delay_ms(1000);
	    send_string("AT+CMGF=1\r");
		_delay_ms(1000);
	    send_string("AT+CMGS=\"+918302189498\"\r");
	    _delay_ms(1000);
	    send_string(msg_string);
		while(!(UCSRA & (1<<UDRE)));
		UDR=26;
		_delay_ms(1500);
}
int main()
{
	RELAY_DIR = 1;
	IR_DIR = 0;
	RL_DIR = 1;
	GL_DIR = 1;

	RELAY = 0;
	RED_LED = 1;
	GREEN_LED = 1;
	USART_Init();
	sei();//Interrupt master enable
	lcd_init();
	lcd_clear();
	DDRA |= (1<<PA0);
	DDRD |= (1<<PD5);	/* Make OC1A pin as output */
	BUZ = 1;
	TCNT1 = 0;		/* Set timer1 count zero */
	ICR1 = 2499;		/* Set TOP count for timer1 in ICR1 register */

	/* Set Fast PWM, TOP in ICR1, Clear OC1A on compare match, clk/64 */
	TCCR1A = (1<<WGM11)|(1<<COM1A1);
	TCCR1B = (1<<WGM12)|(1<<WGM13)|(1<<CS10)|(1<<CS11);
	OCR1A = 60;
	_delay_ms(1000);
	lcd_printf("Bluetooth Based");
	lcd_gotoxy(2,0);
	lcd_printf("Security System");
	_delay_ms(2000);
	lcd_clear();
	lcd_printf("SEND YOUR KEY");

//	lcd_printf("Password");
	while(1)
	{
		if(show)
		{
			show = 0;
			lcd_clear();
			lcd_gotoxy(1,0);
			lcd_printf("****");
			if(!(strcmp(pass,received_pass)))
			{
				BUZ = 1;
				msg_done = 1;
				wrong_attempts = 0;;
				GREEN_LED = 0;
				RED_LED = 1;
				lcd_gotoxy(2,0);
				lcd_printf("PASSWORD CORRECT");
				OCR1A = 175;	/* Set servo shaft at -90° position */
				_delay_ms(5000);
				OCR1A = 60;	// Set servo shaft at 0° position
				_delay_ms(1500);
			}
			else
			{
				wrong_attempts += 1;
				GREEN_LED = 1;
				RED_LED = 0;
				lcd_gotoxy(2,0);
				lcd_printf("PASSWORD WRONG");

			}
			indx = 0;
			if(wrong_attempts >= 3)
			{
				BUZ = 0;
				if(msg_done)
				{
					msg_done = 0;
					lcd_clear();
					lcd_printf("SENDING MSG");
					send_MSG("Unauthorised Access!");
				}
			}
			_delay_ms(2000);
			lcd_clear();
			lcd_printf("SEND YOUR KEY");
		}
	}
}

ISR(USART_RXC_vect)	//Inteerupt service routine
{
	cli();
	received_pass[indx] = UDR;
	//lcd_putc(received_pass[indx]);
	indx+=1;
	if(received_pass[indx-1] =='#' )
	{
		received_pass[indx-1] = '\0';
		//indx = 0;
		show = 1;
	}
	sei();
}
