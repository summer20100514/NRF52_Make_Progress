#ifndef __ROTARY_ENCODER_H_
#define __ROTARY_ENCODER_H_

#include <stdint.h>
 
enum {
    ROTARY_STATE_INVALID,
    ROTARY_STATE_CLOCKWISE,
    ROTARY_STATE_COUNTERCLOCKWISE,
};

enum {
    KEY_UNPRESSED,
    KEY_PRESSED_SHORT,
    KEY_PRESSED_LONG
};

#define ROTARY_ENCODER_PIN_A      2
#define ROTARY_ENCODER_PIN_B      3
#define ROTARY_ENCODER_PIN_KEY    4

#define PULSE_CNT_THRESHOLD       3

#define KEY_PRESS_SHORT_TIMEOUT   10
#define KEY_PRESS_LONG_TIMEOUT    600

void rotary_encoder_init(void);
int get_rotary_encoder_state(void);
int get_key_value(void);

#endif


