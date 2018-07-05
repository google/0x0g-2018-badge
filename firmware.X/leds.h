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

#ifndef LEDS_H
#define	LEDS_H

#define LMODE_OFF 0
#define LMODE_CHASE_1 1
#define LMODE_CHASE_2 2
#define LMODE_CHASE_FAST 3
#define LMODE_BLUE_TEAM 4
#define LMODE_RED_TEAM 5

void exactly_on(uint8_t which);
void timer0_interrupt(void);
void timer0_setup(void);
void service_leds(void);

#endif	/* LEDS_H */

