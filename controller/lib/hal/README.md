Hardware abstraction layer.  All calls to the hardware should go through this
interface, so that we can mock the hardware in tests.  Once work here is
completed, we won't need to include "Arduino.h" outside of this directory.

A design criterion is that this should be zero-overhead when compiling for the
microcontroller.
