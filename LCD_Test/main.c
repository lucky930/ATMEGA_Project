/*
 * LCD Project.c
 *
 * Created: 7/3/2023 12:29:19 AM
 * Author : Raman
 */ 
#define F_CPU 1600000UL
//#include <reg51.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#define BUTTON_PIN PORTD3
#define RS PORTC6
#define RW PORTC5
#define EN PORTC4
#define LED_GREEN PORTA4
#define LED_RED PORTA5

 void lcd_data(unsigned char data)  //for writing data to LCD
 {
	 PORTC = data>>4 ;
	 PORTC |=(1<<RS);  //RS=1
	 PORTC &=(~(1<<RW));  //RW=0
	 PORTC |=(1<<EN);  //EN=1
	 _delay_ms(10);  //10ms
	 PORTC &=(~(1<<EN)); //EN=0
	 
	 	 PORTC = (data & 0x0F);
	 	 PORTC |=(1<<RS);  //RS=1
	 	 PORTC &=(~(1<<RW));  //RW=0
	 	 PORTC |=(1<<EN);  //EN=1
	 	 _delay_ms(10);  //10ms
	 	 PORTC &=(~(1<<EN)); //EN=0
 }
 
 void lcd_cmd(unsigned char command)  //used to write instruction to LCD
 {
	 PORTC = command>>4;
	 PORTC &= (~(1<<RS)); //RS=0
	 PORTC &= (~(1<<RW));  //RW=0
	 PORTC |= (1<<EN);	//EN=1
	 _delay_ms(10);		//10ms
	 PORTC &= (~(1<<EN));	//EN=0
	 
	 	 PORTC = (command & 0x0F);
	 	 PORTC &= (~(1<<RS)); //RS=0
	 	 PORTC &= (~(1<<RW));  //RW=0
	 	 PORTC |= (1<<EN);	//EN=1
	 	 _delay_ms(10);		//10ms
	 	 PORTC &= (~(1<<EN));	//EN=0
 }

 void lcd_string(const unsigned char *str,unsigned char length)
 {
	 char i=0;
	 for(i=0;i<length;i++)
	 {
		 lcd_data(str[i]);
	 }
 }
 void lcd_initialise()
 {
	 //lcd_cmd(0x38);
	 lcd_cmd(0x02);   //
	 //lcd_cmd(0x20);
	 lcd_cmd(0x28);  //4bit mode
	 lcd_cmd(0x06); //Entry Mode
	 lcd_cmd(0x0c); //display ON curson OFF
	 lcd_cmd(0x01);   //clear the display
 }


 int main(void)
 {
	sei();    //enabling status register
	GICR |= (1<<INT0) | (1<<INT1); //Enable General Intruppt control register
	MCUCR |= (0<<ISC10) | (1<<ISC11); // |(1<<ISC00)| (1<<ISC01); 	 //MCUCR register, raising interrupt on pullup or rising edge. ISC11, ISC10 for INT1 and ISC01, ISC00 for INT0
	 DDRC = 0xFF;
	 DDRA=0x30;
	 DDRD &= ~(1 << BUTTON_PIN);   //setting button register port as Input i.e 0
 	 PORTD |= (1 << BUTTON_PIN);   //setting port register as output
	 //DDRB |= (1<<DDB0) | (1<<DDB1) | (1<<DDB2);
	 lcd_initialise();
	 
	 menu_display();   //calling menu_display function
	 
	 while(1)
	 {
		 //if (PIND & (1 << BUTTON_PIN))
		 
	 }
 }


ISR(INT1_vect)  //int1 interrupt function
{
	lcd_cmd(0x01);
	lcd_cmd(0x80);
	lcd_string("You are in Therapy!",19);
	
}	
/*
ISR(INT0_vect)	//int2 interrupt function
{
	lcd_cmd(0x01);
	lcd_cmd(0x80);
	lcd_string("You are in Therapy!",19);
	
	
}*/
void menu_display()
{
	//screen1:
	_delay_ms(100);
	lcd_cmd(0x01);  //clear display
	lcd_cmd(0x80);   //line1
	lcd_string("SARASO PVT LTD",14);
	lcd_cmd(0xC0);    //line2
	lcd_string("SELECT OPTIONS:",15);
	lcd_cmd(0x94);    // line3
	lcd_string(">THERAPY",8);
	lcd_cmd(0xD4);   //line 4
	lcd_string("SETTINGS",8);
	
	//PORTA=0x30;     // Put value for PortA i.e. LED
	PORTA |= (1<<LED_GREEN) | (1<<LED_RED);
	_delay_ms(5000);
	PORTA &= ~(1<<LED_GREEN) & ~(1<<LED_RED);
	_delay_ms(5000);
}