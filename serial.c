//
//  serial.c
//  
//
//  Created by Sam Picus on 11/30/14.
//
//

#include "serial.h"
#include <avr/io.h>


#define FOSC 16000000 // Clock frequency
#define BAUD 9600  // Baud rate used
#define UBRR FOSC/16/BAUD-1 // Value for UBRR0

void send_bit(char);
char recive_bit();

void init_serial() {
    UCSR0C = (3 << UCSZ00);     // Async., no parity, 1 stop bit, 8 data bits
    UCSR0B |= (1 << TXEN0 | 1 << RXEN0);  // Enable RX and TX
    UBRR0 = UBRR;     // Set baud rate
   
    
}

void send(int temp) {
    char t[4]; //Array for Temp ASCII to go in
    if (temp>=0 ) {
        t[0] = '+';
    }
    else {
        t[0] = '-';
    }
    t[1] = temp/100 + '0';
    t[2] = (temp%100)/10 +'0';
    t[3] = temp%10 + '0';
    //Sends Array
    send_bit(t[0]);
    send_bit(t[1]);
    send_bit(t[2]);
    send_bit(t[3]);
}

int recive(void) {
    char t[4]; //Array for recived temp to go in
    char first = UDR0;
    //first character is + or -
    if (first != '-' && first != '+') {
        return -1; //Return -1 if bad transmission
    }
    //Fill Array
    t[0] = recive_bit();
    t[1] = recive_bit();
    t[2] = recive_bit();
    t[3] = '\0';
    int temp_other = atoi(t);
    if (first == '-') {
        temp_other = temp_other*-1;
    }
    return temp_other;
    
}

void send_bit(char t) {
    while ((UCSR0A & (1<<UDRE0)) == 0) {}
    UDR0 = t;
    
}

char recive_bit() {
    while ( !(UCSR0A & (1 << RXC0)) ) {}
    return UDR0;
}