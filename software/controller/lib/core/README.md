This "library" will eventually contain all of the controller code, other than
the main method, which lives in src/.

We do it this way because platformio tests can only depend on libraries; they
can't depend on code in src/.

Controller code can only be moved here once it no longer contains references to
Arduino.h and instead all of its hardware calls go through HAL.  This is because
when compiling for test on x86, this whole library must compile cleanly.

We could split this library up into smaller components, but then they have to be
a well-formed DAG, which in my experience ends up being more trouble than it's
worth.  It's not like we have a lot of code now or expect to have a lot in the
future.
