#include "comms.h"

#include <algorithm>
#include "checksum.h"
#include "frame_detector.h"
#include "framing.h"
#include "hal.h"
#include "network_protocol.pb.h"
#include "uart_dma.h"
#include "uart_dma_stream.h"

//#include "debug.h"

extern UartDma uart_dma;

RxBufferUartDma<RX_FRAME_LEN_MAX> rx_buffer(uart_dma);
FrameDetector<RxBufferUartDma<RX_FRAME_LEN_MAX>, RX_FRAME_LEN_MAX>
    frame_detector(rx_buffer);

// Note that the initial value of last_tx has to be invalid; changing it to 0
// wouldn't work.  We immediately transmit on boot, and after
// we do that, we want to wait a full TX_INTERVAL_MS.  If we initialized
// last_tx to 0 and our first transmit happened at time millis() == 0, we
// would set last_tx back to 0 and then retransmit immediately.
bool Comms::is_time_to_transmit() {
  return (last_tx == kInvalidTime || hal.Now() - last_tx > TX_INTERVAL);
}

bool Comms::is_transmitting() { return uart_dma_.tx_in_progress(); }

void Comms::on_tx_complete() {
  // debug.Print("$");
}

void Comms::on_tx_error() {
  // debug.Print("E");
}

DmaStream<(ControllerStatus_size + 2) * 2 + 2> dma_stream;
static auto EncodeControllerStatusFrame = EncodeFrame<ControllerStatus>;

void Comms::process_tx(const ControllerStatus &controller_status) {
  // Serialize our current state into the buffer if
  //  - we're not currently transmitting,
  //  - it's been a while since we last transmitted.
  if (is_transmitting() || !is_time_to_transmit()) {
    // TODO: Alarm if we haven't been able to send a status in a certain amount
    // of time.
    return;
  }

  hal.DisableInterrupts();
  uint32_t frame_len =
      EncodeControllerStatusFrame(controller_status, dma_stream);
  hal.EnableInterrupts();

  if (0 == frame_len) {
    // TODO log an error
    return;
  }

  last_tx = hal.Now();
}

static auto DecodeGuiStatusFrame = DecodeFrame<GuiStatus>;

void Comms::process_rx(GuiStatus *gui_status) {
  if (frame_detector_.frame_available()) {
    uint8_t *buf = frame_detector_.take_frame();
    uint32_t len = frame_detector_.frame_length();

    GuiStatus new_gui_status = GuiStatus_init_zero;
    DecodeResult result = DecodeGuiStatusFrame(buf, len, &new_gui_status);
    if (DecodeResult::Success == result) {
      *gui_status = new_gui_status;
      last_rx = hal.Now();
    }
  }
}

bool Comms::init() { return frame_detector_.begin(); }

void Comms::handler(const ControllerStatus &controller_status,
                    GuiStatus *gui_status) {

  process_tx(controller_status);
  process_rx(gui_status);
}
