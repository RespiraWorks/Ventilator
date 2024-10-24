# Utilities

## Raspberry pi deployment scripts

If a proper enclosed unit is built as described under [manufacturing](../../manufacturing), it should be possible to deploy all GUI and controller software directly via the Raspberry pi that is integrated into the ventilator unit.

Scripts for doing so are available under [rpi_config](rpi_config).

## decoder

[decoder.py](decoder.py)

This is a decoder of serial packets sent from controller to GUI.

## Regenerating python proto bindings

Use the [common.sh](../common/common.sh) with the `generate` command.
