params = {
    0x01: "ABS_POS",
    0x02: "ELE_POS",
    0x03: "MARK_POS",
    0x04: "SPEED",
    0x05: "ACCEL",
    0x06: "DECEL",
    0x07: "MAX_SPEED",
    0x08: "MIN_SPEED",
    0x12: "ADC_OUT",
    0x13: "OVER_CRNT_THRESH",
    0x15: "FULL_STEP_SPEED",
    0x16: "STEP_MODE",
    0x17: "ALARM_ENA",
    0x18: "GATE_CFG1",
    0x19: "GATE_CFG2",
    0x1B: "STATUS",
    0x1A: "CONFIG",
    0x09: "KVAL_HOLD",
    0x0A: "KVAL_RUN",
    0x0B: "KVAL_ACCEL",
    0x0C: "KVAL_DECEL",
    0x0D: "INT_SPEED",
    0x0E: "START_SLOPE",
    0x0F: "FINAL_SLOPE_ACCEL",
    0x10: "FINAL_SLOPE_DECEL",
    0x11: "THERMAL_COMP",
    0x14: "STALL_THRESH",
}


def main():
    for p in params:
        SetVar("get_param", p)
        print("%20s: %s" % (params[p], GetVar("mtr_value")))


main()
