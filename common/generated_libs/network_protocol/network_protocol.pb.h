/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.1 */

#ifndef PB_NETWORK_PROTOCOL_PB_H_INCLUDED
#define PB_NETWORK_PROTOCOL_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Enum definitions */
typedef enum _VentMode {
    VentMode_OFF = 0,
    VentMode_PRESSURE_CONTROL = 1,
    VentMode_PRESSURE_ASSIST = 2
} VentMode;

/* Struct definitions */
typedef struct _SensorsProto {
    float patient_pressure_cm_h2o;
    float volume_ml;
    float flow_ml_per_min;
    float inflow_pressure_diff_cm_h2o;
    float outflow_pressure_diff_cm_h2o;
} SensorsProto;

typedef struct _VentParams {
    VentMode mode;
    uint32_t peep_cm_h2o;
    uint32_t breaths_per_min;
    uint32_t pip_cm_h2o;
    float inspiratory_expiratory_ratio;
    uint32_t inspiratory_trigger_cm_h2o;
    uint32_t expiratory_trigger_ml_per_min;
} VentParams;

typedef struct _ControllerStatus {
    uint64_t uptime_ms;
    VentParams active_params;
    SensorsProto sensor_readings;
    float pressure_setpoint_cm_h2o;
    float fan_power;
} ControllerStatus;

typedef struct _GuiStatus {
    uint64_t uptime_ms;
    VentParams desired_params;
} GuiStatus;


/* Helper constants for enums */
#define _VentMode_MIN VentMode_OFF
#define _VentMode_MAX VentMode_PRESSURE_ASSIST
#define _VentMode_ARRAYSIZE ((VentMode)(VentMode_PRESSURE_ASSIST+1))


/* Initializer values for message structs */
#define GuiStatus_init_default                   {0, VentParams_init_default}
#define ControllerStatus_init_default            {0, VentParams_init_default, SensorsProto_init_default, 0, 0}
#define VentParams_init_default                  {_VentMode_MIN, 0, 0, 0, 0, 0, 0}
#define SensorsProto_init_default                {0, 0, 0, 0, 0}
#define GuiStatus_init_zero                      {0, VentParams_init_zero}
#define ControllerStatus_init_zero               {0, VentParams_init_zero, SensorsProto_init_zero, 0, 0}
#define VentParams_init_zero                     {_VentMode_MIN, 0, 0, 0, 0, 0, 0}
#define SensorsProto_init_zero                   {0, 0, 0, 0, 0}

/* Field tags (for use in manual encoding/decoding) */
#define SensorsProto_patient_pressure_cm_h2o_tag 1
#define SensorsProto_volume_ml_tag               2
#define SensorsProto_flow_ml_per_min_tag         3
#define SensorsProto_inflow_pressure_diff_cm_h2o_tag 4
#define SensorsProto_outflow_pressure_diff_cm_h2o_tag 5
#define VentParams_mode_tag                      1
#define VentParams_peep_cm_h2o_tag               3
#define VentParams_breaths_per_min_tag           4
#define VentParams_pip_cm_h2o_tag                5
#define VentParams_inspiratory_expiratory_ratio_tag 6
#define VentParams_inspiratory_trigger_cm_h2o_tag 8
#define VentParams_expiratory_trigger_ml_per_min_tag 9
#define ControllerStatus_uptime_ms_tag           1
#define ControllerStatus_active_params_tag       2
#define ControllerStatus_sensor_readings_tag     3
#define ControllerStatus_pressure_setpoint_cm_h2o_tag 5
#define ControllerStatus_fan_power_tag           6
#define GuiStatus_uptime_ms_tag                  1
#define GuiStatus_desired_params_tag             2

/* Struct field encoding specification for nanopb */
#define GuiStatus_FIELDLIST(X, a) \
X(a, STATIC,   REQUIRED, UINT64,   uptime_ms,         1) \
X(a, STATIC,   REQUIRED, MESSAGE,  desired_params,    2)
#define GuiStatus_CALLBACK NULL
#define GuiStatus_DEFAULT NULL
#define GuiStatus_desired_params_MSGTYPE VentParams

#define ControllerStatus_FIELDLIST(X, a) \
X(a, STATIC,   REQUIRED, UINT64,   uptime_ms,         1) \
X(a, STATIC,   REQUIRED, MESSAGE,  active_params,     2) \
X(a, STATIC,   REQUIRED, MESSAGE,  sensor_readings,   3) \
X(a, STATIC,   REQUIRED, FLOAT,    pressure_setpoint_cm_h2o,   5) \
X(a, STATIC,   REQUIRED, FLOAT,    fan_power,         6)
#define ControllerStatus_CALLBACK NULL
#define ControllerStatus_DEFAULT NULL
#define ControllerStatus_active_params_MSGTYPE VentParams
#define ControllerStatus_sensor_readings_MSGTYPE SensorsProto

#define VentParams_FIELDLIST(X, a) \
X(a, STATIC,   REQUIRED, UENUM,    mode,              1) \
X(a, STATIC,   REQUIRED, UINT32,   peep_cm_h2o,       3) \
X(a, STATIC,   REQUIRED, UINT32,   breaths_per_min,   4) \
X(a, STATIC,   REQUIRED, UINT32,   pip_cm_h2o,        5) \
X(a, STATIC,   REQUIRED, FLOAT,    inspiratory_expiratory_ratio,   6) \
X(a, STATIC,   REQUIRED, UINT32,   inspiratory_trigger_cm_h2o,   8) \
X(a, STATIC,   REQUIRED, UINT32,   expiratory_trigger_ml_per_min,   9)
#define VentParams_CALLBACK NULL
#define VentParams_DEFAULT NULL

#define SensorsProto_FIELDLIST(X, a) \
X(a, STATIC,   REQUIRED, FLOAT,    patient_pressure_cm_h2o,   1) \
X(a, STATIC,   REQUIRED, FLOAT,    volume_ml,         2) \
X(a, STATIC,   REQUIRED, FLOAT,    flow_ml_per_min,   3) \
X(a, STATIC,   REQUIRED, FLOAT,    inflow_pressure_diff_cm_h2o,   4) \
X(a, STATIC,   REQUIRED, FLOAT,    outflow_pressure_diff_cm_h2o,   5)
#define SensorsProto_CALLBACK NULL
#define SensorsProto_DEFAULT NULL

extern const pb_msgdesc_t GuiStatus_msg;
extern const pb_msgdesc_t ControllerStatus_msg;
extern const pb_msgdesc_t VentParams_msg;
extern const pb_msgdesc_t SensorsProto_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define GuiStatus_fields &GuiStatus_msg
#define ControllerStatus_fields &ControllerStatus_msg
#define VentParams_fields &VentParams_msg
#define SensorsProto_fields &SensorsProto_msg

/* Maximum encoded size of messages (where known) */
#define GuiStatus_size                           50
#define ControllerStatus_size                    87
#define VentParams_size                          37
#define SensorsProto_size                        25

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
