#include <stdint.h>

#include <xc.h>

#include "leds.h"

volatile uint16_t led_pos = 0;
volatile uint8_t led_mode = LMODE_BLUE_TEAM; // default mode

/*
 * Turn all LEDs off
 */
static inline void all_off() {
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA2 = 1;
    TRISAbits.TRISA3 = 1;
    TRISBbits.TRISB3 = 1;
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

#define LED_RED_LEFT()          leda_on(0, 0)
#define LED_RED_RIGHT()         leda_on(0, 1)
#define LED_YELLOW_LEFT()       leda_on(2, 0)
#define LED_YELLOW_RIGHT()      leda_on(2, 1)
#define LED_GREEN_LEFT()        ledb_on(3, 0)
#define LED_GREEN_RIGHT()       leda_on(3, 0)
#define LED_BLUE_LEFT()         ledb_on(3, 1)
#define LED_BLUE_RIGHT()        leda_on(3, 1)

void timer0_interrupt(void) {
    led_pos++;
    led_pos &= 0b1111111111;
    INTCONbits.T0IF = 0;  // Clear flag
}

void bits3_chase(uint8_t pos) {
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

void bits2_double_chase(uint8_t pos, uint8_t side) {
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

void inline blue_team_led(uint16_t pos) {
    if (pos < 512) {
        if ((pos%2) == 0)
            LED_BLUE_RIGHT();
        else
            LED_BLUE_LEFT();
    } else {
        bits2_double_chase((pos - 512) / 128, pos%2);
    }
}

void inline red_team_led(uint16_t pos) {
    if (pos < 512) {
        if ((pos%2) == 0)
            LED_RED_RIGHT();
        else
            LED_RED_LEFT();
    } else {
        bits2_double_chase((pos - 512) / 128, pos%2);
    }
}


void service_leds(void) {
    uint16_t temp_pos = led_pos;
    
    switch (led_mode) {
        case LMODE_OFF:
            all_off();
            break;
        case LMODE_CHASE_1:
            all_off();
            bits3_chase((temp_pos & 0b11111111) / 32);
            break;
        case LMODE_CHASE_FAST:
            all_off();
            bits3_chase(((temp_pos & 0b11111111) / 16) % 8);
            break;
        case LMODE_BLUE_TEAM:
            all_off();
            blue_team_led(temp_pos);
            break;
        case LMODE_RED_TEAM:
            all_off();
            red_team_led(temp_pos);
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
