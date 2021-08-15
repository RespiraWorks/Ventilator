#!/bin/bash

# Fail if any command fails
set -e
set -o pipefail

# Create virtual environment if it doesn't exist
if [ -d ".venv" ]
then
    read -p "A virtual environment exists. Would you like to delete it and create a new one? [y/n]: " input

    case $input in
        [yY][eE][sS]|[yY])
    echo "Deleting the existing venv..."
    rm -r .venv
    echo "Creating a new venv..."
    python3 -m venv .venv
    ;;
        [nN][oO]|[nN])
    echo "Using the existing venv..."
        ;;
        *)
    echo "Invalid input..."
    exit 1
    ;;
    esac
else
    echo "Creating a new venv..."
    python3 -m venv .venv
fi

source .venv/bin/activate
echo "Installing packages..."
python3 -m pip install -r requirements.txt

pushd "$(dirname "$0")"/debug

./debug_cli.py

popd
