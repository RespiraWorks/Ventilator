# Utils

**#TODO: document other utilities**

### DMA UART test

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
