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

#ifndef IR_DECODER_H
#define	IR_DECODER_H

#include <stdint.h>
#include <xc.h>

void setup_ir_decoder();
/** Call from ISR when (TMR1IF && TMR1IE) */
void timer1_interrupt_decoder();

extern volatile uint32_t ir_data;  // Data when valid
extern volatile uint8_t ir_data_valid;  // Reader should reset this


#endif	/* IR_DECODER_H */

