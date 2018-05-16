/*
 * File:   main.c
 * Author: David
 *
 * Created on May 15, 2018, 3:16 AM
 */

#include <xc.h>

#include "ir_decoder.h"
#include "leds.h"


void interrupt isr(void) {
    if (PIR1bits.TMR1IF && PIE1bits.TMR1IE) {
        timer1_interrupt_decoder();
    }
    if (INTCONbits.T0IF) {
        timer0_interrupt();
    }
}

void main(void) {
    setup_ir_decoder();
    
    for(;;){}
}
