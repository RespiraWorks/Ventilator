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

// [RM] 38.8 USART Registers (pg 1238)
struct UartStruct {
  union {
    struct {
      uint32_t enable : 1;
      uint32_t stop_mode : 1;
      uint32_t rx_enable : 1;
      uint32_t tx_enable : 1;
      uint32_t idle_interrupt : 1;
      uint32_t rx_interrupt : 1;
      uint32_t tx_complete_interrupt : 1;
      uint32_t tx_interrupt : 1;
      uint32_t parity_error_interrupt : 1;
      uint32_t parity : 1;
      uint32_t parity_control_enable : 1;
      uint32_t rx_wakeup_method : 1;
      uint32_t word_length0 : 1;
      uint32_t mute_mode_enable : 1;
      uint32_t char_match_interrupt : 1;
      uint32_t oversampling_mode : 1;
      uint32_t driver_deassertion_time : 5;  // Driver Enable de-assertion time
      uint32_t driver_assertion_time : 5;    // Driver Enable assertion time
      uint32_t rx_timeout_interrupt : 1;
      uint32_t eob_interrupt : 1;  // End of Block interrupt enable
      uint32_t word_length1 : 1;
      uint32_t reserved : 3;
    } bitfield;
    uint32_t full_reg;
  } control_reg1;  // Control Register 1 (USART_CR1) [RM] 38.8.1 (pg 1238)
  union {
    struct {
      uint32_t reserved2 : 4;
      uint32_t address_detection_mode : 1;  // 7-bit | 4bit Address Detection
      uint32_t break_detection_length : 1;
      uint32_t break_detection_interrput : 1;
      uint32_t reserved1 : 1;
      uint32_t last_bit_clock_pulse : 1;
      uint32_t clock_phase : 1;
      uint32_t clock_polarity : 1;
      uint32_t clock_enable : 1;
      uint32_t stop_bits : 2;
      uint32_t lin_mode_enable : 1;
      uint32_t swap_rx_tx_pins : 1;
      uint32_t rx_inversion : 1;
      uint32_t tx_inversion : 1;
      uint32_t data_inversion : 1;
      uint32_t msb_first : 1;
      uint32_t auto_baudrate_enable : 1;
      uint32_t auto_baudrate_mode : 2;
      uint32_t rx_timeout_enable : 1;
      uint32_t addr : 8;  // used for character detection during normal
                          // reception.  This bit field can only be written
                          // when reception is disabled (RE = 0) or the
                          // USART is disabled (UE=0)
    } bitfield;
    uint32_t full_reg;
  } control2;  // Control Register 2 (USART_CR2) [RM] 38.8.2 (pg 1241)
  union {
    struct {
      uint32_t error_interrupt : 1;
      uint32_t irda_mode : 1;
      uint32_t irda_low_power : 1;
      uint32_t half_duplex : 1;
      uint32_t smartcard_nack_enable : 1;
      uint32_t smartcard_mode : 1;
      uint32_t rx_dma : 1;
      uint32_t tx_dma : 1;
      uint32_t rts_enable : 1;
      uint32_t cts_enable : 1;
      uint32_t cts_interrupt : 1;
      uint32_t onebit : 1;  // One sample bit method enable
      uint32_t overrun_disable : 1;
      uint32_t dma_disable_on_rx_error : 1;
      uint32_t driver_enable_mode : 1;
      uint32_t driver_enable_polarity : 1;
      uint32_t reserved2 : 1;
      uint32_t smartcard_auto_retry_count : 3;
      uint32_t wakeup_stop_interrupt_select : 2;
      uint32_t wakeup_stop_interrupt_enable : 1;
      uint32_t uart_clock_stop_mode : 1;
      uint32_t tx_complete_before_guard_interrupt : 1;
      uint32_t reserved : 7;
    } bitfield;
    uint32_t full_reg;
  } control3;  // Control Register 3 [RM] 38.8.3  (pg 1245)
  uint32_t baudrate;
  uint32_t guard_time;
  union {
    struct {
      uint32_t rx_timeout : 24;
      uint32_t block_length : 8;
    } bitfield;
    uint32_t full_reg;
  } timeout;  // Receiver Timeout Register (USART_RTOR) [RM] 38.8.6 (pg 1251)
  union {
    struct {
      uint32_t auto_baudrate : 1;
      uint32_t send_break : 1;
      uint32_t mute_mode : 1;
      uint32_t flush_rx : 1;
      uint32_t flush_tx : 1;
      uint32_t reserved : 27;
    } bitfield;
    uint32_t full_reg;
  } request;  // Request Register (USART_QRQ) [RM] 38.8.7 (pg 1252)
  union {
    struct {
      const uint32_t parity_error : 1;
      const uint32_t framing_error : 1;
      const uint32_t noise_detection : 1;
      const uint32_t overrun_error : 1;
      const uint32_t idle : 1;
      const uint32_t rx_not_empty : 1;
      const uint32_t tx_complete : 1;
      const uint32_t tx_empty : 1;
      const uint32_t lin_break_detected : 1;
      const uint32_t cts_interrupt : 1;
      const uint32_t cts : 1;
      const uint32_t rx_timeout : 1;
      const uint32_t end_of_block : 1;
      const uint32_t reserved : 1;
      const uint32_t auto_baudrate_error : 1;
      const uint32_t auto_baudrate_flag : 1;
      const uint32_t busy : 1;
      const uint32_t char_match : 1;
      const uint32_t send_break : 1;
      const uint32_t rx_wakeup : 1;
      const uint32_t wakeup_from_stop : 1;
      const uint32_t tx_enable_ack : 1;
      const uint32_t rx_enable_ack : 1;
      const uint32_t reserved2 : 2;
      const uint32_t tx_complete_before_guard : 1;
      const uint32_t reserved3 : 6;
    } bitfield;
    uint32_t full_reg;
  } status;  // [RM] 38.8.8 Interrupt And Status Register (USART_ISR) (pg 1253)
  union {
    struct {
      uint32_t parity_error_clear : 1;
      uint32_t framing_error_clear : 1;
      uint32_t noise_detected_clear : 1;
      uint32_t overrun_clear : 1;
      uint32_t clear_idle : 1;
      uint32_t reserved : 1;
      uint32_t tx_complete_clear : 1;
      uint32_t tx_complete_before_guard_clear : 1;
      uint32_t lin_break_detected_clear : 1;
      uint32_t cts_clear : 1;
      uint32_t reserved2 : 1;
      uint32_t rx_timeout_clear : 1;
      uint32_t end_of_block_clear : 1;
      uint32_t reserved3 : 4;
      uint32_t char_match_clear : 1;
      uint32_t reserved4 : 2;
      uint32_t wakeup_from_stop_clear : 1;
      uint32_t reserved5 : 11;
    } bitfield;
    uint32_t full_reg;
  } interrupt_clear;  // [RM] 38.8.9 Interrupt Flag Clear Register (USART_ICR)
                      // (pg 1257)
  uint32_t rx_data;
  uint32_t tx_data;
};
typedef volatile UartStruct UartReg;
inline UartReg *const Uart1Base = reinterpret_cast<UartReg *>(0x40013800);
inline UartReg *const Uart2Base = reinterpret_cast<UartReg *>(0x40004400);
inline UartReg *const Uart3Base = reinterpret_cast<UartReg *>(0x40004800);
inline UartReg *const Uart4Base = reinterpret_cast<UartReg *>(0x40004C00);

// [RM] 16.6 ADC Registers (for each ADC) (pg 450)
struct AdcStruct {
  // A/D specific registers (0x100 total length)
  struct {
    uint32_t status;
    uint32_t interrupts_enable;
    uint32_t control;
    struct {
      uint32_t dma_enable : 1;
      uint32_t dma_config : 1;
      uint32_t df_sdm_config : 1;
      uint32_t resolution : 2;
      uint32_t alignment : 1;
      uint32_t external_trigger_selection : 4;
      uint32_t external_trigger_enable : 2;
      uint32_t overrun_mode : 1;
      uint32_t continuous_conversion : 1;
      uint32_t delayed_conversion : 1;
      uint32_t reserved1 : 1;
      uint32_t discontinuous_mode : 1;
      uint32_t discontinuous_channel_count : 3;
      uint32_t discontinuous_injected_channels : 1;
      uint32_t jsqr_mode : 1;
      uint32_t single_channel_analog_watchdog1 : 1;
      uint32_t analog_watchdog1_enable : 1;
      uint32_t analog_watchdog1_on_injected : 1;
      uint32_t automatic_injected_conversion : 1;
      uint32_t analog_watchdog1_channel : 5;
      uint32_t injected_queue_disable : 1;
    } configuration1;  // 0x0C - [RM] 16.6.4 ADC Configuration Register
                       // (ADC_CFGR) (pg 458)

    struct {
      uint32_t regular_oversampling : 1;
      uint32_t injected_oversampling : 1;
      uint32_t oversampling_ratio : 3;
      uint32_t oversampling_shift : 4;
      uint32_t triggered_oversampling : 1;
      uint32_t regular_oversampling_mode : 1;
      uint32_t reserved : 21;
    } configuration2;  // [RM] 16.6.5 ADC configuration register 2 (ADC_CFGR2)
                       // (pg 462)

    struct {
      uint32_t ch0 : 3;
      uint32_t ch1 : 3;
      uint32_t ch2 : 3;
      uint32_t ch3 : 3;
      uint32_t ch4 : 3;
      uint32_t ch5 : 3;
      uint32_t ch6 : 3;
      uint32_t ch7 : 3;
      uint32_t ch8 : 3;
      uint32_t ch9 : 3;
      uint32_t reserved1 : 2;
      uint32_t ch10 : 3;
      uint32_t ch11 : 3;
      uint32_t ch12 : 3;
      uint32_t ch13 : 3;
      uint32_t ch14 : 3;
      uint32_t ch15 : 3;
      uint32_t ch16 : 3;
      uint32_t ch17 : 3;
      uint32_t ch18 : 3;
      uint32_t reserved2 : 5;
    } sample_times;  // [RM] 16.6.6 ADC Sample Time Register 1 (ADC_SMPR1) (pg
                     // 464)

    uint32_t reserved1;
    uint32_t watchdogs[3];  // 0x20 - [RM] 16.6.8 ADC Watchdog
    uint32_t reserved2;

    // 4x sequence registers.  These registers are used
    // to define the number of A/D readings and the
    // channel numbers being read.
    struct {
      uint32_t length : 6;
      uint32_t sequence1 : 6;
      uint32_t sequence2 : 6;
      uint32_t sequence3 : 6;
      uint32_t sequence4 : 6;
      uint32_t reserved1 : 2;

      uint32_t sequence5 : 6;
      uint32_t sequence6 : 6;
      uint32_t sequence7 : 6;
      uint32_t sequence8 : 6;
      uint32_t sequence9 : 6;
      uint32_t reserved2 : 2;

      uint32_t sequence10 : 6;
      uint32_t sequence11 : 6;
      uint32_t sequence12 : 6;
      uint32_t sequence13 : 6;
      uint32_t sequence14 : 6;
      uint32_t reserved3 : 2;

      uint32_t sequence15 : 6;
      uint32_t sequence16 : 6;
      uint32_t reserved4 : 20;
    } sequence;  // ADC Regular Sequence Register (pg 468)

    uint32_t data;
    uint32_t reserved3[2];
    uint32_t injected_sequence;
    uint32_t reserved4[4];
    uint32_t offset[4];
    uint32_t reserved5[4];
    uint32_t injected_data[4];
    uint32_t reserved6[4];
    uint32_t watchdoc_config[2];
    uint32_t reserved7[2];
    uint32_t differential_mode;
    uint32_t calibration;  // 0xB4 - Calibration Factors [RM] 16.6.22 (pg 476)
    uint32_t reserved8[18];
  } adc[2];  // Master ADC1, Slave ADC2

  uint32_t common_status;
  uint32_t reserved9;
  uint32_t common_control;
  uint32_t common_data;
};
typedef volatile AdcStruct AdcReg;
inline AdcReg *const AdcBase = reinterpret_cast<AdcReg *>(0X50040000);

// Timer Register
// NOTE: Offset values and applicable registers depend on timer used
struct TimerStruct {
  union {
    struct {
      uint32_t counter_enable : 1;
      uint32_t update_disable : 1;
      uint32_t update_request_source : 1;
      uint32_t one_pulse_mode : 1;
      uint32_t direction : 1;
      uint32_t center_aligned_mode : 2;
      uint32_t auto_reload_preload : 1;
      uint32_t clock_division : 2;
      uint32_t reserved : 1;
      uint32_t uif_remapping : 1;
      uint32_t reserved2 : 20;
    } bitfield;
    uint32_t full_reg;
  } control_reg1;
  uint32_t control2;
  uint32_t slave_control;
  uint32_t interrupts_enable;
  uint32_t status;
  uint32_t event;
  uint32_t capture_compare_mode[2];
  uint32_t capture_compare_enable;
  // The topmost bit of counter will contain contain UIFCOPY if UIFREMAP is
  // enabled, but *this register should not be decomposed into a bitfield
  // struct*.  The idea behind UIFREMAP is to read the counter plus the UIFCOPY
  // value atomically, in one go.
  uint32_t counter;
  uint32_t prescaler;
  uint32_t auto_reload;
  uint32_t repeat;
  uint32_t capture_compare[4];
  uint32_t dead_time;
  uint32_t dma_control;
  uint32_t dma_address;
  uint32_t option1;
  uint32_t capture_compare_mode3;
  uint32_t capture_compare_mode5;
  uint32_t capture_compare_mode6;
  uint32_t option2;
  uint32_t option3;
};
typedef volatile TimerStruct TimerReg;

// [RM] 26 Advanced-control Timers (TIM1) (pg 718)
inline TimerReg *const Timer1Base = reinterpret_cast<TimerReg *>(0x40012C00);
// [RM] 27 General-purpose Timers (TIM2/TIM3) (pg 817)
inline TimerReg *const Timer2Base = reinterpret_cast<TimerReg *>(0x40000000);
inline TimerReg *const Timer3Base = reinterpret_cast<TimerReg *>(0x40000400);
// [RM] 29 Basic Timers (TIM6/TIM7) (pg 968)
inline TimerReg *const Timer6Base = reinterpret_cast<TimerReg *>(0x40001000);
inline TimerReg *const Timer7Base = reinterpret_cast<TimerReg *>(0x40001400);
// [RM] 28 General-purpose Timers (TIM15/TIM16) (pg 887)
inline TimerReg *const Timer15Base = reinterpret_cast<TimerReg *>(0x40014000);
inline TimerReg *const Timer16Base = reinterpret_cast<TimerReg *>(0x40014400);

// [RM] 3.7 Flash Registers (pg 100)
struct FlashStruct {
  // Access control register
  struct {
    uint32_t latency : 3;  // Number of wait states
    uint32_t reserved1 : 5;
    uint32_t prefecth_enable : 1;
    uint32_t instruction_cache_enable : 1;
    uint32_t data_cache_enable : 1;
    uint32_t instruction_cache_reset : 1;  // Instruction cache reset
    uint32_t data_cache_reset : 1;         // Data cache reset
    uint32_t run_powerdown : 1;            // Power-down during run or low power mode
    uint32_t sleep_powerdown : 1;          // Power-down during sleep mode
    uint32_t reserved2 : 17;
  } access;

  uint32_t powerdown_key;
  uint32_t key;
  uint32_t option_key;
  uint32_t status;

  // 0x14 - Control Register (FLASH_CR)
  struct {
    uint32_t program : 1;
    uint32_t page_erase : 1;
    uint32_t mass_erase : 1;
    uint32_t page : 8;
    uint32_t reserved1 : 5;
    uint32_t start_operation : 1;
    uint32_t opt_start : 1;
    uint32_t fast_programming : 1;
    uint32_t reserved2 : 5;
    uint32_t end_op_interrupt : 1;
    uint32_t error_interrupt : 1;
    uint32_t read_error_interrupt : 1;
    uint32_t force_byte_reloading : 1;
    uint32_t reserved3 : 2;
    uint32_t options_lock : 1;
    uint32_t flash_lock : 1;
  } control;

  uint32_t ecc;
  uint32_t reserved1;
  uint32_t option;
  uint32_t pcrop_start_address;
  uint32_t pcrop_end_address;
  uint32_t wrp_area_a;
  uint32_t wrp_area_b;
};
typedef volatile FlashStruct FlashReg;
inline FlashReg *const FlashBase = reinterpret_cast<FlashReg *>(0x40022000);

// [RM] 11.4.4 DMA channels (pg 302)
enum class DmaChannel : uint8_t {
  Chan1 = 0,
  Chan2 = 1,
  Chan3 = 2,
  Chan4 = 3,
  Chan5 = 4,
  Chan6 = 5,
  Chan7 = 6,
};

enum class DmaChannelDir { PeripheralToMemory = 0, MemoryToPeripheral = 1 };
enum class DmaTransferSize { Byte = 0, HalfWord = 1, Word = 2 };

struct DmaStruct {
  union {
    struct {
      uint32_t gif1 : 1;   // global interrupt flag
      uint32_t tcif1 : 1;  // transfer complete (TC) flag
      uint32_t htif1 : 1;  // half transfer (HT) flag
      uint32_t teif1 : 1;  // transfer error (TE) flag
      uint32_t gif2 : 1;   // global interrupt flag
      uint32_t tcif2 : 1;  // transfer complete (TC) flag
      uint32_t htif2 : 1;  // half transfer (HT) flag
      uint32_t teif2 : 1;  // transfer error (TE) flag
      uint32_t gif3 : 1;   // global interrupt flag
      uint32_t tcif3 : 1;  // transfer complete (TC) flag
      uint32_t htif3 : 1;  // half transfer (HT) flag
      uint32_t teif3 : 1;  // transfer error (TE) flag
      uint32_t gif4 : 1;   // global interrupt flag
      uint32_t tcif4 : 1;  // transfer complete (TC) flag
      uint32_t htif4 : 1;  // half transfer (HT) flag
      uint32_t teif4 : 1;  // transfer error (TE) flag
      uint32_t gif5 : 1;   // global interrupt flag
      uint32_t tcif5 : 1;  // transfer complete (TC) flag
      uint32_t htif5 : 1;  // half transfer (HT) flag
      uint32_t teif5 : 1;  // transfer error (TE) flag
      uint32_t gif6 : 1;   // global interrupt flag
      uint32_t tcif6 : 1;  // transfer complete (TC) flag
      uint32_t htif6 : 1;  // half transfer (HT) flag
      uint32_t teif6 : 1;  // transfer error (TE) flag
      uint32_t gif7 : 1;   // global interrupt flag
      uint32_t tcif7 : 1;  // transfer complete (TC) flag
      uint32_t htif7 : 1;  // half transfer (HT) flag
      uint32_t teif7 : 1;  // transfer error (TE) flag
      uint32_t reserved : 4;
    };
    uint32_t full_reg;
  } interrupt_status;  // Interrupt Status Register (DMA_ISR) [RM] 11.6.1 (pg
                       // 308)

  union {
    struct {
      uint32_t gif1 : 1;   // global interrupt flag
      uint32_t tcif1 : 1;  // transfer complete (TC) flag
      uint32_t htif1 : 1;  // half transfer (HT) flag
      uint32_t teif1 : 1;  // transfer error (TE) flag
      uint32_t gif2 : 1;   // global interrupt flag
      uint32_t tcif2 : 1;  // transfer complete (TC) flag
      uint32_t htif2 : 1;  // half transfer (HT) flag
      uint32_t teif2 : 1;  // transfer error (TE) flag
      uint32_t gif3 : 1;   // global interrupt flag
      uint32_t tcif3 : 1;  // transfer complete (TC) flag
      uint32_t htif3 : 1;  // half transfer (HT) flag
      uint32_t teif3 : 1;  // transfer error (TE) flag
      uint32_t gif4 : 1;   // global interrupt flag
      uint32_t tcif4 : 1;  // transfer complete (TC) flag
      uint32_t htif4 : 1;  // half transfer (HT) flag
      uint32_t teif4 : 1;  // transfer error (TE) flag
      uint32_t gif5 : 1;   // global interrupt flag
      uint32_t tcif5 : 1;  // transfer complete (TC) flag
      uint32_t htif5 : 1;  // half transfer (HT) flag
      uint32_t teif5 : 1;  // transfer error (TE) flag
      uint32_t gif6 : 1;   // global interrupt flag
      uint32_t tcif6 : 1;  // transfer complete (TC) flag
      uint32_t htif6 : 1;  // half transfer (HT) flag
      uint32_t teif6 : 1;  // transfer error (TE) flag
      uint32_t gif7 : 1;   // global interrupt flag
      uint32_t tcif7 : 1;  // transfer complete (TC) flag
      uint32_t htif7 : 1;  // half transfer (HT) flag
      uint32_t teif7 : 1;  // transfer error (TE) flag
      uint32_t reserved : 4;
    };
    uint32_t full_reg;
  } interrupt_clear;  // Interrupt Flag Clear Register [RM] 11.6.2 (pg 311)
  struct ChannelRegs {
    struct {
      uint32_t enable : 1;
      uint32_t tx_complete_interrupt : 1;
      uint32_t half_tx_interrupt : 1;
      uint32_t tx_error_interrupt : 1;
      uint32_t direction : 1;
      uint32_t circular : 1;
      uint32_t peripheral_increment : 1;
      uint32_t memory_increment : 1;
      uint32_t peripheral_size : 2;
      uint32_t memory_size : 2;
      uint32_t priority : 2;
      uint32_t mem2mem : 1;
      uint32_t reserved : 17;
    } config;
    uint32_t count;
    volatile void *peripheral_address;
    volatile void *memory_address;
    uint32_t reserved;
  } channel[7];
  uint32_t reserved[5];
  union {
    struct {
      uint32_t c1s : 4;
      uint32_t c2s : 4;
      uint32_t c3s : 4;
      uint32_t c4s : 4;
      uint32_t c5s : 4;
      uint32_t c6s : 4;
      uint32_t c7s : 4;
      uint32_t reserved : 4;
    };
    uint32_t full_reg;
  } channel_select;  // Channel Selection Register [RM] 11.6.7 (pg 317)
};
typedef volatile DmaStruct DmaReg;
inline DmaReg *const Dma1Base = reinterpret_cast<DmaReg *>(0x40020000);
inline DmaReg *const Dma2Base = reinterpret_cast<DmaReg *>(0x40020400);

/* Select the source for a DMA channel:
  @param dma        Address of DMA registers
  @param chan       DMA channel to modify.  Channels are numbered from 0
  @param selection  Selects which peripheral request to map to the channel
*/
inline void DmaSelectChannel(DmaReg *dma, DmaChannel chan, int selection) {
  selection &= 0x0F;

  int x = 4 * static_cast<int>(chan);  // 4 bits per channel ([RM] 11.3.2)

  uint32_t val = dma->channel_select.full_reg;
  val &= ~(0xF << x);
  val |= selection << x;
  dma->channel_select.full_reg = val;
}

enum class DmaInterrupt { Global = 1, TransferComplete = 2, HalfTransfer = 4, TransferError = 8 };

// Detect a DMA Interrupt
inline bool DmaIntStatus(DmaReg *dma, DmaChannel chan, DmaInterrupt interrupt) {
  uint32_t x = static_cast<uint32_t>(interrupt);
  x <<= 4 * static_cast<uint8_t>(chan);  // 4 events per channel ([RM] 11.6.2)
  return (x & dma->interrupt_status.full_reg) > 0;
}

// Clear a DMA Interrupt
inline void DmaClearInt(DmaReg *dma, DmaChannel chan, DmaInterrupt interrupt) {
  uint32_t x = static_cast<uint32_t>(interrupt);
  x <<= 4 * static_cast<uint8_t>(chan);  // 4 events per channel ([RM] 11.6.2)
  dma->interrupt_clear.full_reg = x;
}

// [RM] 40.6 SPI Registers (pg 1330)
struct SpiStruct {
  struct {
    uint32_t clock_phase : 1;
    uint32_t clock_polarity : 1;
    uint32_t master : 1;
    uint32_t bitrate : 3;
    uint32_t enable : 1;
    uint32_t lsb_first : 1;
    uint32_t internal_slave_select : 1;
    uint32_t sw_slave_management : 1;
    uint32_t rx_only : 1;
    uint32_t crc_length : 1;
    uint32_t crc_next : 1;
    uint32_t crc_enable : 1;
    uint32_t bidir_output : 1;
    uint32_t bidir_mode : 1;
    uint32_t reserved : 16;
  } control_reg1;  // Control register 1 (SPIx_CR1) [RM] 40.6.1 (pg 1330)
  struct {
    uint32_t rx_dma : 1;
    uint32_t tx_dma : 1;
    uint32_t ss_output : 1;
    uint32_t nss_pulse : 1;
    uint32_t frame_format : 1;
    uint32_t error_interrupt : 1;
    uint32_t rx_not_empty_interrupt : 1;
    uint32_t tx_empty_interrupt : 1;
    uint32_t data_size : 4;
    uint32_t fifo_rx_threshold : 1;
    uint32_t last_dma_rx : 1;
    uint32_t last_dma_tx : 1;
    uint32_t reserved : 17;
  } control2;  // Control register 2 (SPIx_CR2) [RM] 40.6.2
  uint32_t status;
  uint32_t data;
  uint32_t crc_polynomial;
  uint32_t rx_crc;
  uint32_t tx_crc;
};
typedef volatile SpiStruct SpiReg;
inline SpiReg *const Spi1Base = reinterpret_cast<SpiReg *>(0x40013000);
inline SpiReg *const Spi2Base = reinterpret_cast<SpiReg *>(0x40003800);
inline SpiReg *const Spi3Base = reinterpret_cast<SpiReg *>(0x40003C00);

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
