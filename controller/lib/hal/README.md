# Hardware Abstraction Layer

All calls to the hardware should go through this
interface, so that we can mock the hardware in tests.  Once work here is completed, we won't need to include "Arduino.h" outside of this directory.

A design criterion is that this should be zero-overhead when compiling for the microcontroller.

## SOURCES OF TRUTHS

The code refers to the following references by their associated abbreviations

`[RM]` [Reference Manual for the STM32L452 processor](https://www.st.com/resource/en/reference_manual/dm00151940-stm32l41xxx42xxx43xxx44xxx45xxx46xxx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)

`[PM]` [Programmer's manual for the Cortex M4 line of processors](https://www.st.com/resource/en/programming_manual/dm00046982-stm32-cortexm4-mcus-and-mpus-programming-manual-stmicroelectronics.pdf)

`[PCB]` [RespiraWorks custom printed circuit board schematic](https://github.com/RespiraWorks/pcbreathe)

*Note: The latest revision of PCB release candidates can be found under: NIGHTLY/YYYYMMDDvI-RELEASE-CANDIDATE-J*

`[PCBsp]` [Rev 1.0 Electrical Hardware Requirements and PCB Interface Control Document](https://docs.google.com/spreadsheets/d/1JOSQKxkQxXJ6MCMDI9PwUQ6kiuGdujR4D6EJN9u2LWg/edit#gid=0)

`[DS]` [Data Sheet for the STM32L452RE nucleo we use](https://www.st.com/resource/en/datasheet/stm32l452re.pdf)
