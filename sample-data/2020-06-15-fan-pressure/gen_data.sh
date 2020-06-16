#!/usr/bin/env bash -e

# Measures fan pressure at a series of pressures.  This will not work without
# some changes to the code; see README.md for details.

cd "$(dirname "$0")"

BASEDIR="$(git rev-parse --show-toplevel)"
DEBUG_SCRIPT="$BASEDIR"/utils/controller_debug.py

function get_data {
  POW="$1"
  POW_FRAC="$(echo "$1 / 100.0" | bc -l)"
  POW_STR="$(printf "%03d" $POW)"
  OUTFILE="fan$POW_STR"

  echo "POWER $POW_FRAC"
  "$DEBUG_SCRIPT" -c "set fan_power $POW_FRAC"
  "$DEBUG_SCRIPT" -c 'set gui_mode 1'
  sleep 1
  "$DEBUG_SCRIPT" -c 'trace start pressure'
  sleep 5
  "$DEBUG_SCRIPT" -c "trace download --dest $OUTFILE"
  tail -n1 "$OUTFILE"
}

# Data below power level 50 is not very interesting; the fan is barely
# spinning.  So record with less granularity.
for ((i = 0; i < 50; i += 5)); do
  get_data $i
done

for ((i = 80; i <= 100; i += 5)); do
  get_data $i
done

# Phew, we're done.  Disable the ventilator!
"$DEBUG_SCRIPT" -c 'set gui_mode 0'
