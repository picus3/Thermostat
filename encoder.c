//
//  encoder.c
//  
//
//  Created by Sam Picus on 11/30/14.
//
//

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile int count;
volatile char A;
volatile char B;

int change (int num){
    //Takes Low or High Temp and changes it accordingly
    count = num;
    if (A != (PIND & (1<<PD2))) {
        A = (PIND & (1<<PD2));
        if (B == 0) {
            if (A == 0) {
                count -= 1;
                
            }
            else  {
                count += 1;
                
            }
        }
        else  {
            if (A == 0) {
                count += 1;
            }
            else {
                count -= 1;
                
            }
        }
        
    }
    
    if (B != (PIND & (1<<PD3))) {
        B = (PIND & (1<<PD3));
        if (A == 0) {
            if (B == 0) {
                count += 1;
            }
            else {
                count -= 1;
                
            }
        }
        else {
            if (B == 0) {
                count -= 1;
            }
            else  {
                count += 1;
                
            }
            
        }
        
    }
    return count;

    
}

void init_encoder(void) {
    //Turns on Interuptts
    PORTD |= (1<<PD2) | (1<<PD3);
    PCICR |= (1<< PCIE2);
    PCMSK2 |= (1<<PD2) | (1<<PD3);
    //Gets inital redaing of Pins
    A = (PIND & (1<<PD2));
    B = (PIND & (1<<PD3));
}