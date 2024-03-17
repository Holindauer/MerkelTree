#include "lib.hpp"


/**
 * @test test_bitwiseManipulations() tests the bitwise manipulation functions 
 * used in the SHA-256 algorithm.
 
*/
void test_bitwiseManipulations() {

    // instantiate SHA256 class
    SHA256 sha256;

    // Testing rightRotate
    assert(sha256.rightRotate(2, 0b10110000) == 0b00101100);
    assert(sha256.Choice(0b1010, 0b1100, 0b0110) == 0b1100);
    assert(sha256.Majority(0b1010, 0b1100, 0b0110) == 0b1110);    
    assert(sha256.Sigma0(0b00100010) == (sha256.rightRotate(2, 0b00100010) ^ sha256.rightRotate(13, 0b00100010) ^ sha256.rightRotate(22, 0b00100010)));
    assert(sha256.Sigma1(0b11000110) == (sha256.rightRotate(6, 0b11000110) ^ sha256.rightRotate(11, 0b11000110) ^ sha256.rightRotate(25, 0b11000110)));
    assert(sha256.sigma0(0b10101010) == (sha256.rightRotate(7, 0b10101010) ^ sha256.rightRotate(18, 0b10101010) ^ (0b10101010 >> 3)));
    assert(sha256.sigma1(0b01010101) == (sha256.rightRotate(17, 0b01010101) ^ sha256.rightRotate(19, 0b01010101) ^ (0b01010101 >> 10)));
    
    std::cout << "test_bitwiseManipulations()...Pass!" << std::endl;
}

/**
 * @test test_paddingEmptyString() tests the padding of an empty string
*/
void test_paddingEmptyString() {
    SHA256 sha256;

    // Step 1: Create an empty vector<bool> to simulate an empty string
    vector<bool> bitVec;

    // Step 2: Call the pad function
    sha256.pad(bitVec);

    // Step 3: Verify the total length is 512 bits
    if (bitVec.size() != 512) {
        std::cerr << "Padding test failed: Total length is not 512 bits." << std::endl;
        return;
    }

    // Step 4: Verify the first bit of padding is '1'
    if (!bitVec[0]) {
        std::cerr << "Padding test failed: The first bit of padding is not '1'." << std::endl;
        return;
    }

    // Step 5: Verify the last 64 bits are all '0' (big endian representation of 0)
    bool lengthFieldCorrect = true;
    for (size_t i = bitVec.size() - 64; i < bitVec.size(); i++) {
        if (bitVec[i]) {
            lengthFieldCorrect = false;
            break;
        }
    }

    if (!lengthFieldCorrect) {
        std::cerr << "Padding test failed: The length field is not correctly set to 0." << std::endl;
        return;
    }

    std::cout << "test_paddingEmptyString()...Pass!" << std::endl;
}

/**
 * @test test_padding_56byte_message() tests the padding of a 56-byte message. This is an edge
 * case because the padding must add an extra 512-bit block to the message in order to reach a
 * multiple of 512 bits. 
*/
void test_padding_56byte_message() {
    SHA256 sha256;
    // Simulating a 56-byte message with all bits set to 0 for simplicity
    vector<bool> bitVec(56 * 8, false); 

    // Apply padding
    sha256.pad(bitVec);

    // Check total length is 1024 bits (2 * 512-bit blocks)
    assert(bitVec.size() == 1024);

    // Check that the '1' bit is correctly placed after the original 448 bits (56 bytes)
    assert(bitVec[56 * 8] == true);

    // All bits between the '1' bit and the 64-bit length should be 0
    for (size_t i = (56 * 8) + 1; i < 1024 - 64; ++i) {
        assert(bitVec[i] == false);
    }

    // Check the last 64 bits for the big endian representation of 448 bits (56 bytes)
    // The original message length in bits is 448, not 56, so we convert 448 to its big endian binary representation
    vector<bool> expectedLengthBits(64, false);
    size_t lengthInBits = 56 * 8; // 448 bits
    for (int i = 63; i >= 0; --i) {
        expectedLengthBits[i] = (lengthInBits % 2) == 1;
        lengthInBits /= 2;
    }

    bool lengthMatch = std::equal(bitVec.end() - 64, bitVec.end(), expectedLengthBits.begin());
    assert(lengthMatch);

    std::cout << "test_padding_56byte_message()...Pass!" << std::endl;
}

/**
 * @test test_convertToWords() tests the conversion of a vector of bits to a vector of 32-bit words
*/
void test_convertToWords() {

    // init SHA256 class
    SHA256 sha256;

    // Test case 1: Exactly 32 bits
    vector<bool> bitVec32 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
    vector<uint32_t> expectedWords32 = {1}; // 0...01 (1 in 32-bit)
    auto resultWords32 = sha256.convertToWords(bitVec32);
    assert(resultWords32 == expectedWords32);

    // Test case 2: 64 bits
    vector<bool> bitVec64 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                             0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0};
    vector<uint32_t> expectedWords64 = {1, 2}; // 0...01 and 0...010
    auto resultWords64 = sha256.convertToWords(bitVec64);
    assert(resultWords64 == expectedWords64);

    // Test case 3: Not a multiple of 32 bits (e.g., 34 bits)
    vector<bool> bitVec34 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                             0,0}; // 34 bits, last two are 0
    vector<uint32_t> expectedWords34 = {1, 0}; // The first word is 0...01, and the second is 0...0 (32 zeros)
    auto resultWords34 = sha256.convertToWords(bitVec34);
    assert(resultWords34.size() == 2 && resultWords34[0] == expectedWords34[0] && resultWords34[1] == expectedWords34[1]);

    std::cout << "test_convertToWords()...Pass!" << std::endl;
}

void test_hashEmptyString() {
    SHA256 sha256;

    // Create an empty vector<bool> to simulate an empty string
    vector<bool> bitVec;

    // Compute hash
    string computedHash = sha256.computeHash(bitVec); // Note: computeHash needs to return the hash string for this to work

    // Known correct hash for an empty string in SHA-256
    string correctHash = "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855";

    // Check if the computed hash matches the known correct hash
    if (computedHash != correctHash) {
        std::cerr << "Hash test for empty string failed. Expected: " << correctHash << ", Got: " << computedHash << std::endl;
        return;
    }
}


// test driver
int main() {
    // test_applyPadding();
    test_bitwiseManipulations();
    test_paddingEmptyString();
    test_padding_56byte_message();
    test_convertToWords();
    test_hashEmptyString();


    return 0;
}





/**
Test Vectors

SHA256("")
0x e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855
*/