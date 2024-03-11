CXX=g++
CXXFLAGS=-I include
LDFLAGS= # Add any linker flags here. Example: -lssl -lcrypto for OpenSSL
SRC_DIR=src
TEST_DIR=test
BIN_DIR=bin
LIB_SOURCES=$(wildcard $(SRC_DIR)/*.cpp)

# Create bin directory if it doesn't exist
$(shell mkdir -p $(BIN_DIR))

all: test_SHA256

# Test Target
test_SHA256: $(TEST_DIR)/test_SHA-256.cpp $(LIB_SOURCES)
	$(CXX) $(CXXFLAGS) $^ -o $(BIN_DIR)/$(@F) $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(BIN_DIR)/*
