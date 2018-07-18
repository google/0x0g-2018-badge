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
#include "ir_transmitter.h"
#include "leds.h"

volatile uint8_t move_leds = 1;
// Transmitting or receiving?
volatile uint8_t ir_transmitting = 0;

static void interrupt ISR(void) {
    if (INTCONbits.T0IF) {
        if (move_leds)
            timer0_interrupt();
        else
            INTCONbits.T0IF = 0;
    }
    if (PIR1bits.TMR1IF) {
        if (!ir_transmitting) {
            timer1_interrupt_decoder();
        } else {
            PIR1bits.TMR1IF = 0;
        }
    }
}

void main(void) {
    timer0_setup();
    setup_ir_decoder();
    setup_ir_transmitter();
    
    while (1) {
        service_leds();
        if (ir_data_valid > 0) {
            uint16_t ird = ir_data & 0xFFFF;
            if (ird == IR_BITS_LIGHTER_BLUE) {
                seen_blue_team();
            }
            else if (ird == IR_BITS_LIGHTER_RED) {
                seen_red_team();
            }
            else if (ird == IR_BITS_LIGHTER_YELLOW) {
                seen_yellow_team();
            }
            else if (ird == IR_BITS_LIGHTER_GREEN) {
                seen_green_team();
            }
            else if (ird == IR_BITS_BLUE) {
                disable_ir_decoder();
                set_led_mode(LMODE_BLUE_TEAM);
            }
            else if (ird == IR_BITS_RED) {
                disable_ir_decoder();
                set_led_mode(LMODE_RED_TEAM);
            }
            else if (ird == IR_BITS_GREEN) {
                disable_ir_decoder();
                set_led_mode(LMODE_GREEN_TEAM);
            }
            else if (ird == IR_BITS_WHITE) {
                disable_ir_decoder();
                set_led_mode(LMODE_YELLOW_TEAM);
            }
            else if (ird == IR_BITS_BRIGHTNESS_UP) {
                set_led_mode(LMODE_CHASE_FAST);
            }
            else if (ird == IR_BITS_BRIGHTNESS_DOWN) {
                set_led_mode(LMODE_CHASE_1);
            }
            else if (ird == IR_BITS_FLASH) {
                set_led_mode(LMODE_CHASE_2);
            }
            else if (ird == IR_BITS_OFF) {
                set_led_mode(LMODE_OFF);
            }
            else if (ird == IR_BITS_ON) {
                set_led_mode(LMODE_PARTICIPANT_CHASE);
            }
            ir_data_valid = 0;
        }
    }

//    while (1){
//        transmit_word(0x55,0x55);
//        //service_leds();
//    }
}
