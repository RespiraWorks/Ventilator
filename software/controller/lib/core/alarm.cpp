#include <stdint.h>
#include "vars.h"
#include "debug.h"
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
static constexpr Duration COMMUNICATION_TIMEOUT = milliseconds(10000);
static DebugUInt32 dbg_alarm_check("alarm_check",
                                      "alarm_check",
                                      0);
uint32_t alarm_check;

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
    alarm_check = dbg_alarm_check.Get();
    debug.Print("alarm_init");
}

void alarm_handler(void)
{
    static uint32_t count;

    alarm_check = dbg_alarm_check.Get();
    if (alarm_check_counter < MAX_CONTROLLER_ALARMS) {
        switch (alarm_check_counter) {
        case COMM_CHECK_ALARM:
            count++;
            // does the communication timed out
            if (is_time_to_raise_alarm() || (alarm_check == 1)) {
                if (alarms[COMM_CHECK_ALARM].trigger_count < 100) {
                    alarms[COMM_CHECK_ALARM].trigger_count++;
                }
                alarms[COMM_CHECK_ALARM].callthis();
                if (count > 100000) {
                    //debug.Print("yes_alarm ");
                    debug.Print("yes_alarm time: %u last_rx: %u", Hal.now(), comms_get_lastrx_time());
                    count = 0;
                }
            }
            else {
                // communication resumed so clear alarm
                if (alarms[COMM_CHECK_ALARM].trigger_count > 0) {
                    alarms[COMM_CHECK_ALARM].trigger_count--;
                    if (alarms[COMM_CHECK_ALARM].trigger_count == 0) {
                        alarms[COMM_CHECK_ALARM].timestamp = Hal.now();
                        alarms[COMM_CHECK_ALARM].triggered = 0;
                        Hal.BuzzerOff();
                    }
                }
                if (count > 100000) {
                    debug.Print("no_alarm ");
                    count = 0;
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

