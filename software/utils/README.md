# Utilities

## Raspberry pi deployment scripts

If a proper enclosed unit is built as described under [manufacturing](../../manufacturing), it should be possible to deploy all GUI and controller software directly via the Raspberry pi that is integrated into the ventilator unit.

Scripts for doing so are available under [rpi_config](rpi_config).

## controller debug client

[/debug](debug) subdirectory contains a set of Python scripts that talk to the controller via the virtual serial port that is created when you plug a USB cable into the controller's debug port. The command line interface allows you to examine and directly manipulate a number of low level variables, as well as run calibration routines and automated performance tests.

There is a brief tutorial on how to use it right next to the python code in the [/debug](debug) subdirectory. Script can also be accessed via the common controller utilities script at [software/controller/controller.sh](../controller/controller.sh).

## decoder

[decoder.py](decoder.py)

This is a decoder of serial packets sent from controller to GUI.

## Regenerating python proto bindings

*The Python proto bindings really ought to live in common/, next to the .proto file and the generated C++ code.*

On my Mac, the following works to regenerate the Python proto bindings.

```bash
( cd $(git rev-parse --show-toplevel) && \
  protoc -I/usr/local/Cellar/nanopb-generator/0.4.1_1/libexec/proto \
         -Icommon/generated_libs/network_protocol \
         --python_out=utils \
         common/generated_libs/network_protocol/network_protocol.proto )
```
