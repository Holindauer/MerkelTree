#include "lib.hpp"

// test_SHA-256.cpp


void test_read512Bits() {

    // Step 1: Create a test file with known pattern
    const string filename = "test_pattern.bin";
    ofstream testFile(filename, ios::binary | ios::out);

    if (!testFile) {
        cerr << "Failed to create test file." << endl;
        return;
    }

    // Write 64 bytes with alternating 1s and 0s
    for (int i = 0; i < 64; ++i) {
        char byte = (i % 2 == 0) ? 0xAA : 0x55; // 0xAA = 10101010, 0x55 = 01010101
        testFile.write(&byte, sizeof(byte));
    }
    testFile.close();

    // Step 2: Read the file using read512Bits
    ifstream file(filename, ios::binary);
    SHA256 sha256; // Create an instance of the SHA256 class
    auto block = sha256.read512Bits(file); // Call the read512Bits method on the instance
    file.close();

    // Step 3: Verify the content of the block
    bool isCorrect = true;
    for (size_t i = 0; i < block.size(); ++i) {
        unsigned char expectedByte = (i % 2 == 0) ? 0xAA : 0x55;
        if (block[i] != expectedByte) {
            isCorrect = false;
            break;
        }
    }

    // Print result
    if (isCorrect) {
        cout << "Test passed: read512Bits correctly read the file content." << endl;
    } else {
        cout << "Test failed: read512Bits did not read the file content correctly." << endl;
    }

    // Optionally, assert for automatic testing environments
    assert(isCorrect);

    // Clean up: Remove the test file
    std::remove(filename.c_str());
}

// Your SHA256 class definition and read512Bits method implementation here

int main() {
    // Run the test
    test_read512Bits();
    return 0;
}





/**
Test Vectors for SHA
 
SHA224("")
0x d14a028c2a3a2bc9476102bb288234c415a2b01f828ea62ac5b3e42f

SHA256("")
0x e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855

SHA384("")
0x 38b060a751ac96384cd9327eb1b1e36a21fdb71114be07434c0cc7bf63f6e1da274edebfe76f65fbd51ad2f14898b95b

SHA512("")
0x cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e

SHA512/224("")
0x 6ed0dd02806fa89e25de060c19d3ac86cabb87d6a0ddd05c333b84f4

SHA512/256("")
0x c672b8d1ef56ed28ab87c3622c5114069bdd3ad7b8f9737498d0c01ecef0967a
*/