#ifndef IR_DECODER_H
#define	IR_DECODER_H

#include <stdint.h>
#include <xc.h>

void setup_ir_decoder();
/** Call from ISR when (TMR1IF && TMR1IE) */
void timer1_interrupt_decoder();

extern uint32_t ir_data;  // Data when valid
extern uint8_t ir_data_valid;  // Reader should reset this


#endif	/* IR_DECODER_H */

