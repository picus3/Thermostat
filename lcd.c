 //
//  lcd.c
//  
//
//  Created by Sam Picus on 10/22/14.
//
//

#include "lcd.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


void writenibble(unsigned char x);

/*
 init_lcd - Do various things to initialize the LCD display
 */
void init_lcd()
{
    DDRD |= (0b1111<<PD4);
    DDRB |= (0b11<<PB0);
    
    _delay_ms(15); // Delay at least 15ms
    writenibble(0b00110000);
    // Use writenibble to send 0011
    _delay_ms(5);               // Delay at least 4msec
    writenibble(0b00110000);
    // Use writenibble to send 0011
    _delay_us(120);             // Delay at least 100usec
    writenibble(0b00110000);
    // Use writenibble to send 0011
    writenibble(0b00100000);
    // Use writenibble to send 0010    // Function Set: 4-bit interface
    _delay_ms(2);
    writecommand(0b00101000);         // Function Set: 4-bit interface, 2 lines
    _delay_ms(2);
    writecommand(0b00001111); // Display and cursor on
    writecommand(0x01); //Clears Display
    
    
}

/*
 stringout - Print the contents of the character string "str"
 at the current cursor position.
 */
void stringout(char *str)
{
    unsigned char i;
    for (i = 0; str[i] != '\0'; i++) {
        writedata(str[i]); //Prints each letter in the string till reaching Null Character
    }
}

/*
 moveto - Move the cursor to the postion "pos"
 */
void moveto(unsigned char pos)
{
    if (pos < 17){
        pos += 0x80;
    }
    else {
        pos += 0xC0-16;
    }
    
    writecommand(pos);
    
}

//Writes Numbers, Location is where number goes

void writenumbers(char num, char location) {
    cli(); //Prevetns Writing Numbers in Wrong Location 
    char stri[5];
    sprintf(stri,"%3d",num);
    moveto(location);
    stringout(stri);
    sei();
}

/*
 writecommand - Output a byte to the LCD display instruction register.
 */
void writecommand(unsigned char x)
{
    PORTB &= ~(0b1<<PB0);
    writenibble(x); //uppper 4
    writenibble(x<<4); // lowwer 4
    _delay_ms(2);
    
    
}

/*
 writedata - Output a byte to the LCD display data register
 */
void writedata(unsigned char x)
{
    
    PORTB |= (0b1<<PB0);
    writenibble(x); // upper 4
    writenibble(x<<4); //lowwer 4
    _delay_ms(2);
    
}



/*
 writenibble - Output four bits from "x" to the display
 */
void writenibble(unsigned char x)
{
    x &= 0b11110000; //clears out bottom four bits
    PORTD |= x;
    x |= 0b00001111; //Prepares for AND
    PORTD &= x;
    PORTB |= (0b1<<PB1);
    PORTB |= (0b1<<PB1);
    PORTB &= ~(0b1<<PB1);
}