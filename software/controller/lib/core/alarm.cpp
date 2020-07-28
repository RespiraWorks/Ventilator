#include <stdint.h>
#include "hal.h"
#include "alarm.h"
#include "comms.h"
#include "commfail_alarm.h"

// MAX_CONTROLLER_ALARMS 
#define MAX_CONTROLLER_ALARMS 2

// structure definition for MAX_CONTROLLER_ALARMS 
alarm_cb_t alarms[MAX_CONTROLLER_ALARMS];

// this value is reset to 0 after MAX_CONTROLLER_ALARMS 
static uint16_t alarm_check_counter;

// There should be a packet from GUI for every 30ms. Assuming something went
// wrong, we wait for 50ms before raising a communication failure alarm
static constexpr Duration COMMUNICATION_TIMEOUT = milliseconds(50);

extern Time last_rx;
extern Time comms_get_lastrx_time(void);

static bool is_time_to_raise_alarm() {
    return Hal.now() - comms_get_lastrx_time() > COMMUNICATION_TIMEOUT;
}

void alarm_init(void)
{
    // for COMMUNICATION_CHECK_ALARM 1
    alarms[COMM_CHECK_ALARM].timestamp = Hal.now();
    alarms[COMM_CHECK_ALARM].callthis = commfail_alarm_cb;
    alarms[COMM_CHECK_ALARM].priority = ALARM_HIGH_PRIORITY;
    alarms[COMM_CHECK_ALARM].triggered = 0;
    alarms[COMM_CHECK_ALARM].alarm_pause = 0;// 0 not pausable, 1 pausable
    alarms[COMM_CHECK_ALARM].audio_pause = 0;// 0 not pausable, 1 pausable
    alarms[COMM_CHECK_ALARM].trigger_count = 0;
    // for UNDEFINED_ALARM 2
    alarms[UNDEFINED_ALARM].timestamp = Hal.now();
    alarms[UNDEFINED_ALARM].callthis = NULL;
    alarms[UNDEFINED_ALARM].priority = 0;
    alarms[UNDEFINED_ALARM].triggered = 0;
    alarms[UNDEFINED_ALARM].alarm_pause = 0;
    alarms[UNDEFINED_ALARM].audio_pause = 0;
    alarms[UNDEFINED_ALARM].trigger_count = 0;
    alarm_check_counter = 0;
}

void alarm_handler(void)
{
    if (alarm_check_counter < MAX_CONTROLLER_ALARMS) {
        switch (alarm_check_counter) {
        case COMM_CHECK_ALARM:
            // does the communication timed out
            if (is_time_to_raise_alarm()) {
                if (alarms[COMM_CHECK_ALARM].trigger_count < 100) {
                    alarms[COMM_CHECK_ALARM].trigger_count++;
                }
                alarms[COMM_CHECK_ALARM].callthis();
            }
            else {
                if (alarms[COMM_CHECK_ALARM].trigger_count > 0) {
                    alarms[COMM_CHECK_ALARM].trigger_count--;
                    if (alarms[COMM_CHECK_ALARM].trigger_count == 0) {
                        alarms[COMM_CHECK_ALARM].timestamp = Hal.now();
                        alarms[COMM_CHECK_ALARM].triggered = 0;
                    }
                }
            }
            break;
        default:
            break;
        }
        alarm_check_counter++;
    }
    else {
        alarm_check_counter = 0;
    }
}

