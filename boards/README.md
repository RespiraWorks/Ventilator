# Boards

This directory holds the board file used by platformio to build the stm32 controller software.

The board file (custom\_stm32.json) was copied from the platformio board file nucleo\_l452.json .platformio/platforms/ststm32/boards/nucleo\_l452re.json

The main difference being the removal of the variant line to prevent platformio from including the stm32 supporting HAL.

Additionally, a few additional files related to the stm32 build have been added to this directory as a convenient place to keep them:

stm32\_ldscript.ld - This is the linker script copied from: \~/.platformio/packages/framework-arduinoststm32/variants/NUCLEO\_L452RE/ldscript.ld with some minor changes such as removing the heap (not used) and stack (defined in the HAL cpp file).

stm32\_scripts.py - A short Python script used by the platformio build to add some linker flags that can't be added in the platformio.ini file.
