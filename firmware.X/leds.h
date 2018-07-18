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

#ifndef LEDS_H
#define	LEDS_H

#define LMODE_OFF 0
#define LMODE_CHASE_1 1
#define LMODE_CHASE_2 2
#define LMODE_CHASE_FAST 3
#define LMODE_BLUE_TEAM 4
#define LMODE_RED_TEAM 5
#define LMODE_YELLOW_TEAM 6
#define LMODE_GREEN_TEAM 7
#define LMODE_PARTICIPANT_CHASE 8

#define IR_BITS_ON 0b1110000000011111
#define IR_BITS_OFF 0b0110000010011111
#define IR_BITS_RED 0b1001000001101111
#define IR_BITS_GREEN 0b0001000011101111
#define IR_BITS_BLUE 0b0101000010101111
#define IR_BITS_WHITE 0b1101000000101111
#define IR_BITS_BRIGHTNESS_UP 0b1010000001011111
#define IR_BITS_BRIGHTNESS_DOWN 0b0010000011011111
#define IR_BITS_FLASH 0b1111000000001111
#define IR_BITS_LIGHT_YELLOW 0b1000100001110111

void exactly_on(uint8_t which);
void timer0_interrupt(void);
void timer0_setup(void);
void service_leds(void);
void set_led_mode(uint8_t mode);

#endif	/* LEDS_H */

