/* Copyright 2020-2021, RespiraWorks

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#pragma once

#include <cstdint>

/*
The structures below represent the STM32 registers used
to configure various modules (like timers, serial ports, etc).

Detailed information on these modules and the registers
used to configure them can be found in [RM]

Reference abbreviations ([RM], [PCB], etc) are defined in hal/README.md
*/

// The System control block (SCB) provides system implementation information, and system control.
// This includes configuration, control, and reporting of the system exceptions.
// Most of this is defined in [PM] 4.4 (pg 221), except the ones marked otherwise
struct SysControlStruct {
  uint32_t reserved0;
  uint32_t int_type;     // 0xE000E004 What is this ????? And WTF is this from ?????
  uint32_t aux_control;  // 0xE000E008 ACTLR
  uint32_t reserved1;
  uint32_t sys_tick[3];  // 0xE000E010 SysTick timer registers [PM] 4.5 (pg 246)
  uint32_t reserved2[57];
  uint32_t nvic[768];               // 0xE000E100 NVIC [RM] 4.3 (pg 208)
  uint32_t cpu_id;                  // 0xE000ED00 CPUID
  uint32_t interrupt_control;       // 0xE000ED04 ICSR
  uint32_t vector_table;            // 0xE000ED08 VTOR
  uint32_t app_interrupt;           // 0xE000ED0C AIRCR
  uint32_t system_control;          // 0xE000ED10 SCR
  uint32_t config_control;          // 0xE000ED14 CCR
  uint32_t system_priority[3];      // 0xE000ED18 SHPR1-3
  uint32_t system_handler_control;  // 0xE000ED24 SHCSR
  uint32_t fault_status;            // 0xE000ED28 CFSR/MMSR/BFSR/UFSR
  uint32_t hard_fault_status;       // 0xE000ED2C HFSR
  uint32_t reserved3;
  uint32_t mm_fault_address;  // 0xE000ED34 MMAR
  uint32_t bus_fault_addr;    // 0xE000ED38 BFAR
  uint32_t reserved4[19];
  uint32_t coproc_access_control;  // 0xE000ED88 CPACR [PM] 4.6 (pg 252)
};
typedef volatile SysControlStruct SysControlReg;
inline SysControlReg *const SysControlBase = reinterpret_cast<SysControlReg *>(0xE000E000);

// Nested vectored interrupt controller (NVIC) [PM] 4.3 (pg 208)
struct InterruptControlStruct {
  uint32_t set_enable[32];
  uint32_t clear_enable[32];
  uint32_t set_pending[32];
  uint32_t clear_pending[32];
  uint32_t active[64];
  uint8_t priority[1024];
};
typedef volatile InterruptControlStruct InterruptControlReg;
inline InterruptControlReg *const NvicBase = reinterpret_cast<InterruptControlReg *>(0xE000E100);

// [RM] 37.7 I2C Registers
struct I2CStruct {
  union {
    struct {
      uint32_t enable : 1;
      uint32_t tx_interrupts : 1;
      uint32_t rx_interrupts : 1;
      uint32_t addr_interrupts : 1;
      uint32_t nack_interrupts : 1;
      uint32_t stop_interrupts : 1;
      uint32_t tx_complete_interrupts : 1;
      uint32_t error_interrupts : 1;
      uint32_t digital_noise_filter_conntrol : 4;
      uint32_t analog_noise_filternig_off : 1;
      uint32_t reserved1 : 1;
      uint32_t dma_tx : 1;
      uint32_t dma_rx : 1;
      uint32_t slave_byte : 1;
      uint32_t no_clock_stretch : 1;
      uint32_t wake_from_stop : 1;
      uint32_t general_call : 1;
      uint32_t smbus_default_host : 1;
      uint32_t smbus_default_device : 1;
      uint32_t smbus_alert : 1;
      uint32_t smbus_packet_error_checking : 1;
      uint32_t reserved2 : 8;
    };
    uint32_t full_reg;
  } control_reg1;  // Control register 1 [RM] 37.7.1
  union {
    struct {
      uint32_t slave_addr_lsb : 1;           // lsb of 10 bits slave address
                                             // (master)
      uint32_t slave_addr_7b : 7;            // middle 7b bits of slave address
                                             // (master)
      uint32_t slave_addr_msb : 2;           // msb of 10 bits slave address
                                             // (master)
      uint32_t transfer_direction : 1;       // 0 = write, 1 = read (master)
      uint32_t address_10b : 1;              // Set to enable 10 bits address header
                                             // (master)
      uint32_t read_10b_header : 1;          // Clear to send complete read sequence
                                             // (master)
      uint32_t start : 1;                    // Set to generate START condition
      uint32_t stop : 1;                     // Set to generate STOP after byte transfer
                                             // (master)
      uint32_t nack : 1;                     // Generate NACK after byte reception (slave)
      uint32_t n_bytes : 8;                  // Set to the number of bytes to send
      uint32_t reload : 1;                   // Set to allow several consecutive transfers
      uint32_t autoend : 1;                  // Set to automatically send stop condition
      uint32_t packet_error_check_byte : 1;  // Set to send SMBus packet error
                                             // checking byte
      uint32_t reserved : 5;
    };
    uint32_t full_reg;
  } control2;        // Control register 2 [RM] 37.7.2
  uint32_t addr[2];  // Own address (1-2) register [RM] 37.7.{3,4} (slave)
  union {
    struct {
      uint32_t scl_low : 8;    // Duration of SCL Low state (cycles)
      uint32_t scl_high : 8;   // Duration of SCL High state (cycles)
      uint32_t sda_hold : 4;   // Delay between SCL falling edge and SDA
                               // edge (cycles)
      uint32_t scl_delay : 4;  // Delay between SDA edge and SCL rising
                               // edge (cycles)
      uint32_t reserved : 4;
      uint32_t prescaler : 4;  // Prescaler
    };
    uint32_t full_reg;
  } timing;          // Timing register [RM] 37.7.5
  uint32_t timeout;  // Timout register [RM] 37.7.6
  union {
    struct {
      uint32_t tx_empty : 1;
      uint32_t tx_interrupt : 1;
      uint32_t rx_not_empty : 1;
      uint32_t address_match : 1;
      uint32_t nack : 1;
      uint32_t stop : 1;
      uint32_t transfer_complete : 1;
      uint32_t transfer_reload : 1;
      uint32_t bus_error : 1;
      uint32_t arbitration_loss : 1;
      uint32_t overrun : 1;
      uint32_t packet_check_error : 1;
      uint32_t timeout : 1;
      uint32_t alert : 1;
      uint32_t reserved1 : 1;
      uint32_t busy : 1;
      uint32_t transfer_direction : 1;
      uint32_t address_code : 7;
      uint32_t reserved : 8;
    };
    uint32_t full_reg;
  } status;                     // Interrupt & status register [RM] 37.7.7
  uint32_t interrupt_clear;     // Interrupt clear register [RM] 37.7.8
  uint32_t packet_error_check;  // PEC register [RM] 37.7.9
  uint32_t rx_data;             // Receive data register [RM] 37.7.10
  uint32_t tx_data;             // Transmit data register [RM] 37.7.11
};
typedef volatile I2CStruct I2CReg;
inline I2CReg *const I2C1Base = reinterpret_cast<I2CReg *>(0x40005400);
inline I2CReg *const I2C2Base = reinterpret_cast<I2CReg *>(0x40005800);
inline I2CReg *const I2C3Base = reinterpret_cast<I2CReg *>(0x40005c00);
inline I2CReg *const I2C4Base = reinterpret_cast<I2CReg *>(0x40008400);

// Watchdog timer
// [RM] 32.4 Watchdog Registers (pg 1016)
struct WatchdogStruct {
  uint32_t key;        // Key register [RM] 32.4.1
  uint32_t prescaler;  // Prescale register [RM] 32.4.2
  uint32_t reload;     // Reload register [RM] 32.4.3
  uint32_t status;     // Status register [RM] 32.4.4
  uint32_t window;     // Window register [RM] 32.4.5
};
typedef volatile WatchdogStruct WatchdogReg;
inline WatchdogReg *const WatchdogBase = reinterpret_cast<WatchdogReg *>(0x40003000);

// CRC calculation unit
// [RM] 14.4 CRC Registers (pg 341)
struct CrcStruct {
  uint32_t data;     // Data register [RM] 14.4.1
  uint32_t scratch;  // Independent data register [RM] 14.4.2
  uint32_t control;  // Control register [RM] 14.4.3
  uint32_t reserved;
  uint32_t init;        // Initial CRC value [RM] 14.4.4
  uint32_t polynomial;  // CRC polynomial [RM] 14.4.5
};
typedef volatile CrcStruct CrcReg;
inline CrcReg *const CrcBase = reinterpret_cast<CrcReg *>(0x40023000);
