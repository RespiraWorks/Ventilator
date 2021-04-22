#include "hal.h"
#include "interface.h"
#include "uart_dma.h"
#include <string.h>

char r[20];

extern UartDma dma_uart;

class DummyTxListener : public UartDmaTxListener {
  void OnTxComplete() override { debugPrint("$"); }
  void OnTxError() override{};
};

class DummyRxListener : public UartDmaRxListener {
public:
  void OnRxComplete() override {
    debugPrint("&");
    debugPrint(r);
  }
  void OnCharacterMatch() override { debugPrint("@"); }
  void OnRxError(RxError e) override {
    if (kRxTimeout == e) {
      debugPrint("T");
    } else {
      debugPrint("#");
    };
  }
};

// FramingRxFSM listener = FramingRxFSM();
DummyRxListener rxlistener;
DummyTxListener txlistener;

DMACtrl dma_controller(kDma1Base);
constexpr uint8_t kTxCh = 1;
constexpr uint8_t kRxCh = 2;
UartDma dma_uart(kUart3Base, kDma1Base, kTxCh, kRxCh, &rxlistener, &txlistener,
                 '.');

int main() {
  hal.Init();
  dma_controller.Init();

  debugPrint("*");
  char s[] = "ping ping ping ping ping ping ping ping ping ping ping ping\n";
  bool dma_started = false;

  dma_started = dma_uart.StartTX(s, strlen(s));
  if (dma_started) {
    debugPrint("!");
  }

  dma_uart.CharMatchEnable();

  dma_started = dma_uart.StartRX(r, 10, 115200 * 2);
  if (dma_started) {
    debugPrint("!");
  }

  while (true) {
    hal.WatchdogHandler();
    char i[1];
    if (1 == debugRead(i, 1)) {
      hal.ResetDevice();
    }
    hal.Delay(milliseconds(10));
  }
}
