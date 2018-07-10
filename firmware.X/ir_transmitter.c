/*
 * Copyright 2018 Google LLC
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

#include <stdint.h>
#include <xc.h>

#include "ir_transmitter.h"

// Determined by 1MHz/38kHz/2
#define TIMER2_PERIOD 13

#define TRANSMIT_OFF()      TRISB |= (1 << 2);
#define TRANSMIT_ON()       TRISB &= ~(1 << 2);

void setup_ir_transmitter() {
    // Setup timer 2 for base carrier
    T2CONbits.TMR2ON = 1;
    PIE1bits.TMR2IE = 0;
    PIR1bits.TMR2IF = 0;
    PR2 = TIMER2_PERIOD;
    // TODO: controlled by timer
    TRANSMIT_ON();
}

void transmit_mark() {
    // Transmit a 38kHz timer for 562.5us, 21.375 cycles = 42 inversions
    INTCONbits.GIE = 0;
    // Called frequently.  Don't do much.
    // Toggle output (RB2))
    register uint8_t i;
    for(i=42;i;--i) {
      PORTB ^= 4; //rb2
      asm("NOP");
    }
    INTCONbits.GIE = 1;
}

void timer1_interrupt_transmitter() {
    
}