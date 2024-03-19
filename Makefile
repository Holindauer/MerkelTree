CXX=g++
CXXFLAGS=-I include
SRC_DIR=src
TEST_DIR=test
BIN_DIR=bin
LIB_SOURCES=$(SRC_DIR)/SHA-256.cpp $(SRC_DIR)/MerkelTree.cpp
MAIN_SOURCE=$(SRC_DIR)/main.cpp 

# Create bin directory if it doesn't exist
$(shell mkdir -p $(BIN_DIR))

all: run_main test_SHA256 test_MerkelTree 

# Main program target
run_main: $(MAIN_SOURCE) $(LIB_SOURCES)
	$(CXX) $(CXXFLAGS) $^ -o $(BIN_DIR)/$(@F) $(LDFLAGS)

# Test Target for SHA-256
test_SHA256: $(TEST_DIR)/test_SHA-256.cpp $(LIB_SOURCES)
	$(CXX) $(CXXFLAGS) $^ -o $(BIN_DIR)/$(@F) $(LDFLAGS)

# Test Target for MerkleTree
test_MerkelTree: $(TEST_DIR)/test_MerkelTree.cpp $(LIB_SOURCES)
	$(CXX) $(CXXFLAGS) $^ -o $(BIN_DIR)/$(@F) $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(BIN_DIR)/*
