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
