# Boards

This directory holds files used by platformio when building the stm32 controller
software.

 * `custom_stm32.json` - Board file, copied from the platformio board file
   `.platformio/platforms/ststm32/boards/nucleo_l452re.json`

   The main difference is the removal of the variant line to prevent
   platformio from including the stm32 HAL.

   platformio.ini points to this directory as its `boards_dir` specifically so
   it can find this file.

 * `stm32_ldscript.ld` - linker script, copied from
   `~/.platformio/packages/framework-arduinoststm32/variants/NUCLEO_L452RE/ldscript.ld`
   with some minor changes such as removing the heap (not used) and stack
   (defined in the HAL cpp file).

 * `stm32_scripts.py` - A short Python script used by the platformio build to
   add some linker flags that can't be added in the platformio.ini file.

 * `platformio_sanitizers.py` - Python script used by platformio build to add
   sanitizers (asan, msan, etc.) when building for native platform (i.e. your
   laptop).
