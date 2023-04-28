
#include <std_types.h>
#include <Macros.h>
#include <cpu_freq.h>
#include <Keypad.h>
#include <Lcd.h>
#include <Dio.h>
#include <Ssd.h>
#include <Registers.h>
#include <Gpt.h>
#include <Interrupts.h>
#include <ExtInterrupt.h>
#include <util/delay.h>
#include <stdio.h>

volatile u16 counter=0;


void Timer0Handler (void) {
	counter++;
}
void Go_cars (void) {
	        /*TRAFFIC_CARS*/
			Dio_SetPinLevel(DIO_PORTB,DIO_PIN0,STD_LOW);
	        Dio_SetPinLevel(DIO_PORTB,DIO_PIN1,STD_LOW);
	        Dio_SetPinLevel(DIO_PORTB,DIO_PIN2,STD_HIGH);
			/*TRAFFIC_PEOPLE*/
			Dio_SetPinLevel(DIO_PORTB,DIO_PIN3,STD_HIGH);
			Dio_SetPinLevel(DIO_PORTB,DIO_PIN4,STD_LOW);
			Dio_SetPinLevel(DIO_PORTB,DIO_PIN5,STD_LOW);
}
void Ready (void) { 
	       /*TRAFFIC_CARS*/
			Dio_SetPinLevel(DIO_PORTB,DIO_PIN0,STD_LOW);
			Dio_SetPinLevel(DIO_PORTB,DIO_PIN1,STD_HIGH);
			Dio_SetPinLevel(DIO_PORTB,DIO_PIN2,STD_LOW);
			/*TRAFFIC_PEOPLE*/
			Dio_SetPinLevel(DIO_PORTB,DIO_PIN3,STD_LOW);
			Dio_SetPinLevel(DIO_PORTB,DIO_PIN4,STD_HIGH);
			Dio_SetPinLevel(DIO_PORTB,DIO_PIN5,STD_LOW);
}
void StopandINT_cars (void) {
	       /*TRAFFIC_CARS*/
			Dio_SetPinLevel(DIO_PORTB,DIO_PIN0,STD_HIGH);//INT_RED
	        Dio_SetPinLevel(DIO_PORTB,DIO_PIN1,STD_LOW);
	        Dio_SetPinLevel(DIO_PORTB,DIO_PIN2,STD_LOW);
			/*TRAFFIC_PEOPLE*/
			Dio_SetPinLevel(DIO_PORTB,DIO_PIN3,STD_LOW);//INT_GREEN
	        Dio_SetPinLevel(DIO_PORTB,DIO_PIN4,STD_LOW);
	        Dio_SetPinLevel(DIO_PORTB,DIO_PIN5,STD_HIGH);

	        for(u8 i=1;i<9;i++){
	        	Ssd_DisplayNumber(i);
	        	_delay_ms(950);
	        }
	        Ssd_Clear();
}

void Toggle_Traffic (void) {
	               StopandINT_cars();
					for(u8 i=1;i<9;i++){
						Ssd_DisplayNumber(i);
						_delay_ms(950);
					}
					Ssd_Clear();
		            counter = 800;
}

int main(void)
{
	Dio_SetPinMode(DIO_PORTD, DIO_PIN2, DIO_PIN_INPUT_PULLUP);	/* External Interrupt Pin Input Pullup */
	Gie_Enable();
	Gpt_Init();
	Gpt_Start(GPT_CHANNEL_TIM0,  78);
	Gpt_EnableNotification(GPT_INT_SOURCE_TIM0_COMP, Timer0Handler);

	Ssd_Init();
	ExtInterrupt_Init();
	ExtInterrupt_EnableNotification(EXT_INTERRUPT_INT0, Toggle_Traffic);

    /*TRAFFIC_CARS*/
	Dio_SetPinMode(DIO_PORTB, DIO_PIN0, DIO_PIN_OUTPUT);    /*RED_CAR*/
	Dio_SetPinMode(DIO_PORTB, DIO_PIN1, DIO_PIN_OUTPUT);    /*YELLOW_CAR*/
	Dio_SetPinMode(DIO_PORTB, DIO_PIN2, DIO_PIN_OUTPUT);    /*GREEN_CAR*/
	/*TRAFFIC_PEOPLE*/
	Dio_SetPinMode(DIO_PORTB, DIO_PIN3, DIO_PIN_OUTPUT);    /*RED_PEOPLE*/
	Dio_SetPinMode(DIO_PORTB, DIO_PIN4, DIO_PIN_OUTPUT);    /*YELLOW_PEOPLE*/
	Dio_SetPinMode(DIO_PORTB, DIO_PIN5, DIO_PIN_OUTPUT);    /*GREEN_PEOPLE*/
	/*MODE*/
	StopandINT_cars ();

	while (1)
	{

		   if (counter == 800) {
					Ready ();
					for(u8 i=1;i<4;i++){
						Ssd_DisplayNumber(i);
						_delay_ms(950);
					}
					Ssd_Clear();
				}

				else if (counter == 1100){

					Go_cars ();
					for(u8 i=1;i<9;i++){
						Ssd_DisplayNumber(i);
						_delay_ms(950);
					}
					Ssd_Clear();
				}
				else if (counter >= 1900){
					counter = 0;
					StopandINT_cars ();

				}

	}
}
