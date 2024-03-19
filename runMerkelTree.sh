#!/bin/bash

# Compile the main program
make run_main

# Check if make succeeded
if [ $? -ne 0 ]; then
  echo "Compilation Failed."
  exit 1
fi

echo 
echo 
echo


# Directory where binaries are located
BIN_DIR="bin"

# Run the main program, passing all script arguments
./$BIN_DIR/run_main "$@"
