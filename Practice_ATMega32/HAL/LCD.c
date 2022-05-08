/*
 * LCD.c
 *
 * Created: 07/05/2022 13:08:05
 *  Author: Mohamed
 */ 

#include "LCD.h"
#include "DIO_Interface.h"

#define LCD_PORT PA
#define RS PINB4
#define EN PINB5

void WriteInstruction(u8 ins)
{
	DIO_WritePin(RS,LOW);
	DIO_WritePort(LCD_PORT,ins);
	DIO_WritePin(EN,HIGH);
	_delay_ms(1);
	DIO_WritePin(EN,LOW);
}

void LCD_Init(void)
{
	_delay_ms(50);
	WriteInstruction(0x38);
	_delay_ms(1);
	WriteInstruction(0x0C);
	_delay_ms(1);
	WriteInstruction(0x01);
	_delay_ms(2);
	WriteInstruction(0x06); //0x06 for no shift //0x07 for shift
}
void WriteData(u8 data)
{
	DIO_WritePin(RS,HIGH);
	DIO_WritePort(LCD_PORT,data);
	DIO_WritePin(EN,HIGH);
	_delay_ms(1);
	DIO_WritePin(EN,LOW);
	_delay_ms(1);
}

void LCD_WriteChar(u8 ch)
{
	WriteData(ch);
}

void LCD_WriteString(u8 * str)
{
	u8 i;
	for(i=0;str[i];i++)
	{
		LCD_WriteChar(str[i]);
	}
}

void LCD_Clear(void)
{
	WriteInstruction(0x01);
	_delay_ms(2);
}

void LCD_WriteNumber(s32 num)
{
	u8 i=0, str[20];
	s8 j=0;
	if(num==0)
	{
		LCD_WriteChar('0');
		return ;
	}
	if(num<0)
	{
		LCD_WriteChar('-');
		num=num*-1;
	}
	while (num)
	{
		str[i]=num%10+'0';
		num/=10;
		i++;
	}
	for(j=i-1;j>=0;j--)
	{
		LCD_WriteChar(str[j]);
	}
}
void LCD_WriteBinary(u8 num)
{
	s8 i;
	for(i=7;i<=0;i--)
	{
		LCD_WriteChar(GET_BIT(num,i)+'0');
	}
}
void LCD_WriteHex(u8 num)
{
	u8 Lnibble=num & 0x0f;
	u8 Hnibble=num >> 4;
	if(Hnibble<=9)
	{
		LCD_WriteChar(Hnibble+'0');
	}else
	{
		LCD_WriteChar(Hnibble-10+'A');
	}
	if(Lnibble<=9)
	{
		LCD_WriteChar(Lnibble+'0');
	}else
	{
		LCD_WriteChar(Lnibble-10+'A');
	}
}
void LCD_SetCursor(u8 line, u8 cell)
{
	if (line ==0)
	{
		WriteInstruction(0x80|cell);
	}else if (line ==1)
	{
		WriteInstruction(0x80|0x40|cell);
	}
}
