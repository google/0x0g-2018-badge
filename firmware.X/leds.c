#include <stdint.h>

#include <xc.h>

#include "leds.h"

/*
 * Turn all LEDs off
 */
static inline void all_off() {
    TRISA |= TRISA0 | TRISA2 | TRISA3;
    TRISB |= TRISB3;
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
    static uint8_t pos = 0;
    pos++;
    pos &= 7;
    all_off();
    switch (pos) {
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
