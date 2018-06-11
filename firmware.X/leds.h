#ifndef LEDS_H
#define	LEDS_H

#define LMODE_OFF 0
#define LMODE_CHASE_1 1
#define LMODE_CHASE_2 2

void timer0_interrupt(void);
void timer0_setup(void);
void service_leds(void);

#endif	/* LEDS_H */

