#ifndef APP_MAIN_H
#define APP_MAIN_H

#include <stdint.h>

#define VERSION "1.0.0"
#define BLINK_GPIO 48
typedef enum
{
    WAIT_FOR_WAKEUP,
    WAIT_FOR_CONNECT,
    START_DETECT,
    START_RECOGNITION,
    START_ENROLL,
    START_DELETE,

} en_fsm_state;

extern en_fsm_state g_state;

extern int g_is_enrolling;
extern int g_is_deleting;

#endif