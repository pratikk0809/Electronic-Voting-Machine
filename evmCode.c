#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define F_CPU 1000000UL
#define E 5 
#define RS 6 

void send_a_command(unsigned char command);
void send_a_character(unsigned char character);
void send_a_string(const char *str);
void display_count(int count, char *buffer, char  lcd_position);

int main(void)
{
	DDRA = 0xFF; 
	DDRD = 0xFF; 
	DDRB = 0x00; 
	PORTB = 0xFF; 

	_delay_ms(50); 

	unsigned char COUNTA = 0;  
	unsigned char COUNTB = 0;  
	unsigned char COUNTC = 0;  
	unsigned char COUNTD = 0;  

	char SHOWA [3];       
	char SHOWB [3];       
	char SHOWC [3];       
	char SHOWD [3];       
	
	send_a_command(0x01);  
	_delay_ms(50);
	send_a_command(0x38);  
	_delay_ms(50);
	send_a_command(0x0F);  

	while (1)
	{
		lcd_set_cursor(0, 0);
		send_a_string("A=");
		display_count(COUNTA, SHOWA, 0x80);    

		lcd_set_cursor(0, 8);
		send_a_string("B=");
		display_count(COUNTB, SHOWB, 0x80 + 8); 

		lcd_set_cursor(1, 0);
		send_a_string("C=");
		display_count(COUNTC, SHOWC, 0xC0);     

		lcd_set_cursor(1, 8);
		send_a_string("D=");
		display_count(COUNTD, SHOWD, 0xC0 + 8); 

		if (bit_is_clear(PINB, 0))  
		{
			COUNTA++;
			_delay_ms(300);         
			while (bit_is_clear(PINB, 0)); 
		}
		if (bit_is_clear(PINB, 1))  
		{
			COUNTB++;
			_delay_ms(300);         
			while (bit_is_clear(PINB, 1)); 
		}
		if (bit_is_clear(PINB, 2))  
		{
			COUNTC++;
			_delay_ms(300);         
			while (bit_is_clear(PINB, 2)); 
		}
		if (bit_is_clear(PINB, 3))  
		{
			COUNTD++;
			_delay_ms(300);         
			while (bit_is_clear(PINB, 3)); 
		}
		if (bit_is_clear(PINB, 4))  
		{
			COUNTA = COUNTB = COUNTC = COUNTD = 0;  
			_delay_ms(300);         
			while (bit_is_clear(PINB, 4)); 
		}
	}
}
void send_a_command(unsigned char command)
{
	PORTA = command;               
	PORTD &= ~(1 << RS);           
	PORTD |= (1 << E);             
	_delay_ms(1);                  
	PORTD &= ~(1 << E);            
	PORTA = 0;                     
}
void send_a_character(unsigned char character)
{
	PORTA = character;             
	PORTD |= (1 << RS);            
	PORTD |= (1 << E);             
	_delay_ms(1);                  
	PORTD &= ~(1 << E);            
	PORTA = 0;                     
}
void send_a_string(const char *str)
{
	while (*str != '\0')   
{
		send_a_character(*str);    
		str++;
	}
}
void lcd_set_cursor(char row, char col)
{
	char address;
	if (row == 0)
	address = 0x80 + col; 
	else
	address = 0xC0 + col; 
	send_a_command(address);
}
void display_count(int count, char *buffer, char  lcd_position)
{
	if (count > 99) count = 99; 
	itoa(count, buffer, 10);    
	if (count < 10) {
		send_a_string("0");      
	}
	send_a_string(buffer);       
}

