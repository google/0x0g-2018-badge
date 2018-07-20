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

#include "leds.h"
#include "ir_transmitter.h"

volatile uint16_t led_pos = 0;
volatile uint8_t seen_teams = 0;
volatile uint8_t led_mode = LMODE_CHASE_2; // default mode

#define LED_RED_LEFT()          leda_on(0, 0)
#define LED_RED_RIGHT()         leda_on(0, 1)
#define LED_YELLOW_LEFT()       leda_on(2, 0)
#define LED_YELLOW_RIGHT()      leda_on(2, 1)
#define LED_GREEN_LEFT()        ledb_on(3, 0)
#define LED_GREEN_RIGHT()       leda_on(3, 0)
#define LED_BLUE_LEFT()         ledb_on(3, 1)
#define LED_BLUE_RIGHT()        leda_on(3, 1)

static inline void all_off();
static inline void leda_on(uint8_t pos, uint8_t dir);
static inline void ledb_on(uint8_t pos, uint8_t dir);

void set_led_mode(uint8_t mode){
    led_mode = mode;
}


/*
 * Turn on exactly one LED
 */
void exactly_on(uint8_t which) {
    all_off();
    switch (which) {
        case 0:
            LED_RED_LEFT();
            break;
        case 1:
            LED_YELLOW_LEFT();
            break;
        case 2:
            LED_GREEN_LEFT();
            break;
        case 3:
            LED_BLUE_LEFT();
            break;
        case 4:
            LED_RED_RIGHT();
            break;
        case 5:
            LED_YELLOW_RIGHT();
            break;
        case 6:
            LED_GREEN_RIGHT();
            break;
        case 7:
            LED_BLUE_RIGHT();
            break;
    }
}

/*
 * Turn all LEDs off
 */
static inline void all_off() {
    TRISA |= 1 | (1 << 2) | (1 << 3);
    TRISB |= (1 << 3);
}

/*
 * Turn a LED on on PORTA
 */
static inline void leda_on(uint8_t pos, uint8_t dir) {
    if (dir)
        PORTA |= (1u<<pos);
    else
        PORTA &= ~(1u<<pos);
    TRISA &= ~(1u<<pos);
}

/*
 * Turn a LED on on PORTB
 */
static inline void ledb_on(uint8_t pos, uint8_t dir) {
    if (dir)
        PORTB |= (1u<<pos);
    else
        PORTB &= ~(1u<<pos);
    TRISB &= ~(1u<<pos);
}

void timer0_interrupt(void) {
    led_pos++;
    led_pos &= 0b11111111111;
    INTCONbits.T0IF = 0;  // Clear flag
}

void static inline bits3_chase(uint8_t pos) {
    switch (pos) {
     default:
     case 0:
         LED_RED_LEFT();
         break;
     case 1:
         LED_YELLOW_LEFT();
         break;
     case 2:
         LED_GREEN_LEFT();
         break;
     case 3:
         LED_BLUE_LEFT();
         break;
     case 4:
         LED_RED_RIGHT();
         break;
     case 5:
         LED_YELLOW_RIGHT();
         break;
     case 6:
         LED_GREEN_RIGHT();
         break;
     case 7:
         LED_BLUE_RIGHT();
         break;
    }
}

void static inline bits2_double_chase(uint8_t pos, uint8_t side) {
    switch (pos) {
     default:
     case 0:
         if (side)
            LED_RED_LEFT();
         else
            LED_RED_RIGHT();
         break;
     case 1:
         if (side)
            LED_YELLOW_LEFT();
         else
            LED_YELLOW_RIGHT();
         break;
     case 2:
         if (side)
            LED_GREEN_LEFT();
         else
            LED_GREEN_RIGHT();
         break;
     case 3:
         if (side)
            LED_BLUE_LEFT();
         else
            LED_BLUE_RIGHT();
         break;
    }
}

void static inline blue_team_led(uint16_t pos) {
    if (pos < 1024) {
        all_off();
        if (pos == 0) {
            transmit_word((uint8_t)((IR_BITS_LIGHTER_BLUE_R >> 8) & 0xFF), (uint8_t)(IR_BITS_LIGHTER_BLUE_R & 0xFF));
        } else {
            if ((pos%2) == 0)
                LED_BLUE_RIGHT();
            else
                LED_BLUE_LEFT();
        }
    } else {
        uint8_t v = (((pos - 1024) / 256) + 3) % 4;
        all_off();
        bits2_double_chase(v, pos%2);
    }
}

void static inline red_team_led(uint16_t pos) {
    if (pos < 1024) {
        all_off();
        if (pos == 0) {
            transmit_word((uint8_t)((IR_BITS_LIGHTER_RED_R >> 8) & 0xFF), (uint8_t)(IR_BITS_LIGHTER_RED_R & 0xFF));
        } else {
            if ((pos%2) == 0)
                LED_RED_RIGHT();
            else
                LED_RED_LEFT();
        }
    } else {
        uint8_t v = (((pos - 1024) / 256) + 0) % 4;
        all_off();
        bits2_double_chase(v, pos%2);
    }
}

void static inline yellow_team_led(uint16_t pos) {
    if (pos < 1024) {
        all_off();
        if (pos == 0) {
            transmit_word((uint8_t)((IR_BITS_LIGHTER_YELLOW_R >> 8) & 0xFF), (uint8_t)(IR_BITS_LIGHTER_YELLOW_R & 0xFF));
        } else {
            if ((pos%2) == 0)
                LED_YELLOW_RIGHT();
            else
                LED_YELLOW_LEFT();
        }
    } else {
        uint8_t v = (((pos - 1024) / 256) + 1) % 4;
        all_off();
        bits2_double_chase(v, pos%2);
    }
}

void static inline green_team_led(uint16_t pos) {
    if (pos < 1024) {
        all_off();
        if (pos == 0) {
            transmit_word((uint8_t)((IR_BITS_LIGHTER_GREEN_R >> 8) & 0xFF), (uint8_t)(IR_BITS_LIGHTER_GREEN_R & 0xFF));
        } else {
            if ((pos%2) == 0)
                LED_GREEN_RIGHT();
            else
                LED_GREEN_LEFT();
        }
    } else {
        uint8_t v = (((pos - 1024) / 256) + 2) % 4;
        all_off();
        bits2_double_chase(v, pos%2);
    }
}

void service_participant_chase(uint16_t pos) {
    if (pos < 1024) {
        if (pos < 512) {
            uint8_t v = pos / 64;
            all_off();
            bits2_double_chase(v, pos%2);
        } else {
            uint8_t v = (1024-pos) / 64;
            all_off();
            bits2_double_chase(v, pos%2);
        }
    } else {
        uint8_t v = (pos % 32) / 16;
        all_off();
        if (pos < 1536) {
            if (pos < 1280 && ((seen_teams & RED_TEAM_BIT) != 0))
                bits2_double_chase(0, v);
            else if ((seen_teams & GREEN_TEAM_BIT) != 0)
                bits2_double_chase(2, v);
        } else {
            if (pos < 1792 && ((seen_teams & YELLOW_TEAM_BIT) != 0))
                bits2_double_chase(1, v);
            else if ((seen_teams & BLUE_TEAM_BIT) != 0)
                bits2_double_chase(3, v);
        }
    }
}

void bluered_flash(uint16_t pos) {
    uint8_t v = (pos % 32) / 16;
    uint8_t side = (pos / 256) % 2;
    all_off();
    if (side == 0)
        bits2_double_chase(0, v);
    else
        bits2_double_chase(3, v);
}


void service_leds(void) {
    uint16_t temp_pos = led_pos;
    uint8_t v;
    
    switch (led_mode) {
        case LMODE_OFF:
            all_off();
            break;
        case LMODE_PARTICIPANT_CHASE:
            service_participant_chase(temp_pos);
            break;
        case LMODE_BLUE_TEAM:
            blue_team_led(temp_pos);
            break;
        case LMODE_RED_TEAM:
            red_team_led(temp_pos);
            break;
        case LMODE_YELLOW_TEAM:
            yellow_team_led(temp_pos);
            break;
        case LMODE_GREEN_TEAM:
            green_team_led(temp_pos);
            break;
        case LMODE_CHASE_1:
            v = ((temp_pos & 0b11111111) / 32);
            all_off();
            bits3_chase(v);
            break;
        case LMODE_CHASE_2:
            v = ((temp_pos & 0b111111111) / 64);
            all_off();
            bits3_chase(v);
            break;
        case LMODE_CHASE_FAST:
            v = ((temp_pos & 0b11111111) / 16) % 8;
            all_off();
            bits3_chase(v);
            break;
        case LMODE_BLUE_RED:
            bluered_flash(temp_pos);
            break;
    }
}

void timer0_setup() {
    OPTION_REGbits.PSA   = 0;      // Prescaler
    OPTION_REGbits.PS    = 0b010;  // 1:???8? Prescaler
    OPTION_REGbits.T0CS  = 0;      // Source = oscillator
    INTCONbits.T0IF      = 0;      // Clear flag.
    INTCONbits.T0IE      = 1;      // Enable timer0 interrupts
    INTCONbits.GIE       = 1;      // Enable global interrupts
}

void seen_blue_team(void) {
    seen_teams |= BLUE_TEAM_BIT;
}
void seen_red_team(void) {
    seen_teams |= RED_TEAM_BIT;
}
void seen_green_team(void) {
    seen_teams |= GREEN_TEAM_BIT;
}
void seen_yellow_team(void) {
    seen_teams |= YELLOW_TEAM_BIT;
}
void clear_seen_teams(void) {
    seen_teams = 0;
}