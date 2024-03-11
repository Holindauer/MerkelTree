#!/bin/bash

# Compile the tests
echo
echo "Compiling Tests..."
echo
make

# Check if make succeeded
if [ $? -ne 0 ]; then
  echo "Compilation Failed."
  exit 1
fi

echo 
echo "Running All Tests..."

# Define your test binary here
tests=("test_SHA256")

# Directory where binaries are located
BIN_DIR="bin"

# Iterate over the tests array and execute each test
for test in "${tests[@]}"; do   
  echo 
  echo "Running $test..."
  ./$BIN_DIR/$test
  if [ $? -ne 0 ]; then
    echo "$test failed!"
    exit 1
  else
    echo "$test passed!"
  fi
done

echo
echo "All tests passed successfully!"
