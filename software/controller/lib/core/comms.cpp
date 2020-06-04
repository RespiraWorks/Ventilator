#include "comms.h"

#include "frame_detector.h"
#include "framing.h"
#include "interrupts.h"
#include "network_protocol.pb.h"
#include "system_timer.h"
#include "uart_dma_stream.h"

RxBufferUartDma<RxFrameLengthMax> rx_buffer(uart_dma);
FrameDetector<RxBufferUartDma<RxFrameLengthMax>, RxFrameLengthMax> frame_detector(rx_buffer);

Comms::Comms() : uart_dma_(uart_dma), frame_detector_(frame_detector) {
  last_rx_ = SystemTimer::singleton().now();
}

// Note that the initial value of last_tx has to be invalid; changing it to 0
// wouldn't work.  We immediately transmit on boot, and after
// we do that, we want to wait a full kTxInterval.  If we initialized
// last_tx to 0 and our first transmit happened at time millis() == 0, we
// would set last_tx back to 0 and then retransmit immediately.
bool Comms::is_time_to_transmit() {
  return (last_tx_ == InvalidTime || SystemTimer::singleton().now() - last_tx_ > TxInterval);
}

bool Comms::is_transmitting() { return uart_dma_.tx_in_progress(); }

void Comms::on_tx_complete() {
  // debug.Print("$");
}

void Comms::on_tx_error() {
  // debug.Print("E");
}

DmaStream<(ControllerStatus_size + 2) * 2 + 2> dma_stream;
static auto encode_conrtoller_status_frame = EncodeFrame<ControllerStatus>;

void Comms::process_tx(const ControllerStatus &controller_status) {
  // Serialize our current state into the buffer if
  //  - we're not currently transmitting,
  //  - it's been a while since we last transmitted.
  if (is_transmitting() || !is_time_to_transmit()) {
    // TODO: Alarm if we haven't been able to send a status in a certain amount
    // of time.
    return;
  }

  {
    BlockInterrupts block_interrupts;
    auto frame_length = encode_conrtoller_status_frame(controller_status, dma_stream);
    if (0 == frame_length) {
      // TODO log an error
      return;
    }
  }

  last_tx_ = SystemTimer::singleton().now();
}

static auto decode_gui_status_frame = DecodeFrame<GuiStatus>;

void Comms::process_rx(GuiStatus *gui_status) {
  if (frame_detector_.frame_available()) {
    auto *buf = frame_detector_.take_frame();
    size_t len = frame_detector_.frame_length();

    GuiStatus new_gui_status = GuiStatus_init_zero;
    auto result = decode_gui_status_frame(buf, len, &new_gui_status);
    if (DecodeResult::Success == result) {
      *gui_status = new_gui_status;
      last_rx_ = SystemTimer::singleton().now();
    }
  }
}

bool Comms::init() { return frame_detector_.begin(); }

void Comms::handler(const ControllerStatus &controller_status, GuiStatus *gui_status) {
  process_tx(controller_status);
  process_rx(gui_status);
}
