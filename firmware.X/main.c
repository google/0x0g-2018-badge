/*
 * File:   main.c
 * Author: David
 *
 * Created on May 15, 2018, 3:16 AM
 */

// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config BODENV = 40      // Brown-out Reset Voltage bit (VBOR set to 4.0V)
#pragma config CP = OFF         // Code Protect (Program memory code protection is disabled)

#include <xc.h>

#include "ir_decoder.h"
#include "leds.h"

volatile uint8_t move_leds = 1;

static void interrupt ISR(void) {
    if (INTCONbits.T0IF) {
        timer0_interrupt();
    }
    if (ir_data_valid) {
        move_leds ^= 1;
        ir_data_valid = 0;
    }
    if (PIR1bits.TMR1IF && PIE1bits.TMR1IE) {
        timer1_interrupt_decoder();
    }
}

void main(void) {
    timer0_setup();
    //setup_ir_decoder();
    
    while (1){
        service_leds();
    }
}
