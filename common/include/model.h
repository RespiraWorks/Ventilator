#ifndef MODEL_H
#define MODEL_H
// Ventilator medical mode types
enum class ventilatorMode {
    PRVC = 0x00,
    ACV  = 0x01,

    count                       /* Sentinel */
};
// The different engineering operating mode types
enum class operatingMode {
  medical = 0x00,
  engineering = 0xF0,

  count /* Sentinel */
};

// Solenoid state definitions
enum class solenoidNormaleState {
    normally_open    = 0x00,
    normally_closed  = 0x01,

    count                       /* Sentinel */
};

// The different periodic data transmission modes
enum class periodicMode {
  off = 0x00,
  on = 0x01,

  count /* Sentinel */
};

#endif // MODEL_H
