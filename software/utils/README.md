# Testing utilities

**#TODO: document other utilities**

## General considerations

For some of these communication tests, it is possible to set
```c++
#define NO_GUI_DEV_MODE
```
in `main.cpp` so that serial data is sent over the same USB cable that is
used to program the nucleo.

Otherwise, a more "honest" test would be to check communication on the
actual serial pins of the Nucleo. To do so, you can use a serial dongle such as
[this](https://www.amazon.com/gp/product/B00QT7LQ88),
or, for a more scalable setup, [this](https://www.amazon.com/dp/B07XF2SLQ1).


## Utilities

### controller_debug

[controller_debug.py](controller_debug.py)

This Python script can be used to interface with the controller via the virtual serial
port that is created when the USB cable to the Nucleo board is plugged in.

There is a [brief tutorial](controller_debug.md) on how to use it.

This interface is the basis for automated performance tests, such as [run_covent_tests.py](run_covent_tests.py).

### decoder

[decoder.py](decoder.py)

This is a decoder of serial packets sent from controller to GUI.

### DMA UART test

[dma_uart_test.py](dma_uart_test.py)

This is an integration test. Test code is deployed to the
controller, and a python script is run locally to confirm that the controller
is running as expected and not resetting.

The controller test code is [here](../controller/src_test).

It is deployed like this:
```bash
pio run -e stm32-test --target upload
```

Then you can run:
```bash
python dma_uart_test.py
```

### Regenerating python proto bindings

*The Python proto bindings really ought to live in common/, next to the .proto
file and the generated C++ code.*

On my Mac, the following works to regenerate the Python proto bindings.

```bash
( cd $(git rev-parse --show-toplevel) && \
  protoc -I/usr/local/Cellar/nanopb-generator/0.4.1_1/libexec/proto \
         -Icommon/generated_libs/network_protocol \
         --python_out=utils \
         common/generated_libs/network_protocol/network_protocol.proto )
```
