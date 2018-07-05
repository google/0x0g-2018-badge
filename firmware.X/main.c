/*
 * Copyright 2018 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
        if (move_leds)
            timer0_interrupt();
        else
            INTCONbits.T0IF = 0;
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
    setup_ir_decoder();
    
    while (1){
        service_leds();
    }
}
