//
//  main.c
//  
//
//  Created by Sam Picus on 11/30/14.
//
//

#include <stdio.h>
#include "lcd.h"
#include "encoder.h"
#include "serial.h"
#include "ds1631.h"
#include <avr/io.h>
#include <avr/interrupt.h>

int low;
int high;
int temp;
unsigned char t[2];
unsigned char previous_t[2];
unsigned char button; // 0 for low 1 for high

void convert(unsigned char*);
void init_button();
void init_LED();

/* Blue is out and Green is In*/

int main() {
    init_lcd();
    init_serial();
    init_encoder();
    init_button();
    ds1631_init();
    ds1631_conv();
    init_LED();
    sei();
    DDRC |= (1<<PC1); //Turns On Tri-State Gate
    
    //Prints Format to Display
    stringout("Temp:");
    moveto(9);
    stringout("R2:");
    moveto(17);
    stringout("Low:");
    moveto(25);
    stringout("Hi:");
    button = 4;
    
 


    while (1) {
        
        ds1631_temp(t);
        //See if it has changes
        if (previous_t[0] != t[0] || previous_t[1] != t[1]) {
            //Updates Previous
            previous_t[0] = t[0];
            previous_t[1] = t[1];
            convert(t);
            send(temp);
            writenumbers(temp, 5);
        }
        
       if (UCSR0A & (1 << RXC0)) {
           int temp_other= recive();
           //If temp_other == 1 then Bad Transmision
           if (temp_other != -1) {
               writenumbers(temp_other, 12);
           }
       }
        
       
        //Checks if Heat/AC needs to be on or off
        if (temp > high) {
            PORTB |= (1<< PB3);
        }
        else {
            PORTB &= ~(1<<PB3);
        }
        if (temp < low) {
            PORTB |= (1<<PB4);
        }
        else {
            PORTB &= ~(1<<PB4);
        }
     
    }
}

void convert(unsigned char* te) {
    temp = te[0] *2 ;
    if (te[1] == 0x80) {
        temp = temp + 1; //Add 1 to point five because it is multiplied
    }
    temp = ((temp * 9)/10) + 32;
}



ISR(PCINT2_vect) {
    //Checks which button is pushed before then calls Encoder
    if (button) {
        low = change(low);
        if ( high < low) {
            low = high;
        }
        writenumbers(low,21);
    }
    else {
        high = change(high);
        if ( high < low) {
            high = low;
        }
        writenumbers(high,28);
    }


}

ISR(PCINT1_vect){
    //Changes the button and places curson next to Number on screen
    if (!(PINC & (1<<PC2))) {
        button = 1;
        writecommand(0xc0 + 5 );
    }
    if (!(PINC & (1<<PC3))) {
        button = 0;
        writecommand(0xc0 + 12);
    }
}

void init_button() {
    PORTC |= (1<<PC2)|(1<<PC3);//Turns on Pull up Resistors
    PCICR |= (1<< PCIE1); //Turns on Pin Change Interupts
    PCMSK1 |= (1<<PC2) | (1<<PC3); //Turns on Pin change interrupts of PC2 and PC3
}

void init_LED() {
    DDRB |= ((0b1<< PB3) | (0b1<<PB4));
    PORTB |= ((1 <<PB3)|(1 <<PB4));
}
