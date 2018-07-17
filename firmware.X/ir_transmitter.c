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

#define _XTAL_FREQ 4000000

#include <stdint.h>
#include <xc.h>

#include "ir_transmitter.h"

#define TRANSMIT_OFF()      TRISB |= (1 << 2);
#define TRANSMIT_ON()       TRISB &= ~(1 << 2);

void transmit_byte(uint8_t data);
void transmit_mark();
void transmit_space_short();
void transmit_space_long();

void setup_ir_transmitter() {
    TRANSMIT_ON();
}

void transmit_word(uint8_t addr, uint8_t command) {
    uint8_t inv_addr = addr ^ 0xFF;
    uint8_t inv_command = command ^ 0xFF;
    INTCONbits.GIE = 0;
    // Start of frame
    register uint8_t i;
    for(i=16;i;--i)
        transmit_mark();
    __delay_us(4500);
    transmit_byte(addr);
    transmit_byte(inv_addr);
    transmit_byte(command);
    transmit_byte(inv_command);
    transmit_mark(); // EOM
    INTCONbits.GIE = 1;
    __delay_us(4500);
}

void transmit_byte(uint8_t data) {
    register uint8_t i;
    for(i=8;i;--i) {
        transmit_mark();
        if(data & 1) {
            transmit_space_long();
        } else {
            transmit_space_short();
        }
        data >>= 1;
    }
}

void transmit_mark() {
    // Transmit a 38kHz timer for 562.5us, 21.375 cycles = 42 inversions
    // Called frequently.  Don't do much.
    // Toggle output (RB2))
    register uint8_t i;
    for(i=42;i;--i) {
      PORTB ^= 4; //rb2
      asm("NOP");
    }
    PORTB &= ~4;
}

void transmit_space_short() {
    __delay_us(550);
}

void transmit_space_long() {
    __delay_us(1675);
}