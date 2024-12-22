

#include <avr/io.h>
#include "std_macro.h"

void DIO_vDisable_internal_Pull_up()
{
	SET_BIT(SFIOR,2);
}
void DIO_vsetPINDir(char portname,char pin,char direction)
{ 
	switch (portname)
	{
		case 'A':
		case 'a':
			if(direction==1)
			{
				SET_BIT(DDRA,pin);
			}
			else
			{
				CLR_BIT(DDRA,pin);
			}
		break;
		case 'B':
		case 'b':
		if(direction==1)
		{
			SET_BIT(DDRB,pin);
		}
		else
		{
			CLR_BIT(DDRB,pin);
		}
		break;
		case 'C':
		case 'c':
		if(direction==1)
		{
			SET_BIT(DDRC,pin);
		}
		else
		{
			CLR_BIT(DDRC,pin);
		}
		break;
		case 'D':
		case 'd':
		if(direction==1)
		{
			SET_BIT(DDRD,pin);
		}
		else
		{
			CLR_BIT(DDRD,pin);
		}
		break;
	}
}

void DIO_write_pin(char portname,char pin,char value)
{
	
	switch (portname)
	{
		
		case 'A':
		case 'a':
		if(value==1)
		{
			SET_BIT(PORTA,pin);
		}
		else
		{
			CLR_BIT(PORTA,pin);
		}
		break;
		case 'B':
		case 'b':
		if(value==1)
		{
			SET_BIT(PORTB,pin);
		}
		else
		{
			CLR_BIT(PORTB,pin);
		}
		break;
		case 'C':
		case 'c':
		if(value==1)
		{
			SET_BIT(PORTC,pin);
		}
		else
		{
			CLR_BIT(PORTC,pin);
		}
		break;
		case 'D':
		case 'd':
		if(value==1)
		{
			SET_BIT(PORTD,pin);
		}
		else
		{
			CLR_BIT(PORTD,pin);
		}
		break;
	}
}
unsigned char DIO_u8read(char portname,char pinnumber)
{
	unsigned char value;
	switch(portname)
	{
		case 'A':
		case 'a':
		value=READ_BIT(PINA,pinnumber);
		break;
		
		case 'B':
		case 'b':
		value=READ_BIT(PINB,pinnumber);
		break;
		
		case 'C':
		case 'c':
		value=READ_BIT(PINC,pinnumber);
		break;
		
		case 'D':
		case 'd':
		value=READ_BIT(PIND,pinnumber);
		break;
	}
	return value;
}
void DIO_toggle_pin(unsigned char portname,unsigned char pinnumber)
{
	switch (portname)
	{
		case 'A':
		case 'a':
		TOG_BIT(PORTA,pinnumber);
		break;
		
		case 'B':
		case 'b':
		TOG_BIT(PORTB,pinnumber);
		break;
		
		case 'C':
		case 'c':
		TOG_BIT(PORTC,pinnumber);
		break;
		
		case 'D':
		case 'd':
		TOG_BIT(PORTD,pinnumber);
		break;
	}
}
void DIO_set_port_direction(unsigned char portname,unsigned char direction)
{
	switch (portname)
	{
		case 'A':
		case 'a':
		DDRA=direction;
		break;
		case 'B':
		case 'b':
		DDRB=direction;
		break;
		case 'C':
		case 'c':
		DDRC=direction;
		break;
		case 'D':
		case 'd':
		DDRD=direction;
		break;
		default:
		break;
	}		
}
void DIO_write_port(unsigned char portname,unsigned char portvalue)
{
	switch(portname)
	{
		case 'A' :
		case 'a':
		PORTA=portvalue; //Write the given value to the port A
		break ;
		case 'B':
		case 'b':
		PORTB=portvalue; //Write the given value to the port B
		break ;
		case 'C' :
		case 'c':
		PORTC=portvalue; //Write the given value to the port C
		break ;
		case 'D':
		case 'd':
		PORTD=portvalue; //Write the given value to the port D
		break ;
		default:
		break ;
	}
}
unsigned char DIO_read_port(unsigned char portname)
{
	unsigned char return_val=0;
	switch(portname)
	{
		case 'A' :
		case 'a' :
		return_val=PINA; // read the value of port A
		break ;
		case 'B':
		case 'b' :
		return_val=PINB; // read the value of port B
		break ;
		case 'C' :
		case 'c' :
		return_val=PINC; // read the value of port C
		break ;
		case 'D':
		case 'd' :
		return_val=PIND; // read the value of port D
		break ;
		default:
		break ;
	}
	return return_val;
}
void DIO_toggle_port(unsigned char portname)
{
		switch(portname)
		{
			case 'A' :
			case 'a' :
			PORTA=~PORTA;
			break;
			case 'B' :
			case 'b' :
			PORTB=~PORTB;
			break;
			case 'C' :
			case 'c' :
			PORTC=~PORTC;
			break;
			case 'D' :
			case 'd' :
			PORTD=~PORTD;
			break;
		}			
}
void DIO_write_low_nibble(unsigned char portname,unsigned char value)
{
	DIO_write_pin(portname,0,READ_BIT(value,0));
	DIO_write_pin(portname,1,READ_BIT(value,1));
	DIO_write_pin(portname,2,READ_BIT(value,2));
	DIO_write_pin(portname,3,READ_BIT(value,3));
}
void DIO_write_high_nibble(unsigned char portname,unsigned char value)
{
	DIO_write_pin(portname,4,READ_BIT(value,0));
	DIO_write_pin(portname,5,READ_BIT(value,1));
	DIO_write_pin(portname,6,READ_BIT(value,2));
	DIO_write_pin(portname,7,READ_BIT(value,3));
}
void DIO_vconnectpullup(unsigned char portname,unsigned char pinnumber)
{
	DIO_vsetPINDir(portname,pinnumber,0);
	DIO_write_pin(portname,pinnumber,1);
}