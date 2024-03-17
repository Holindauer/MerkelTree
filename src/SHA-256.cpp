#include "lib.hpp"

// SHA-256.cpp

/**
 * @note computeHash() fascilitates the processing of each 512 bit chunk of the initial message
 * @param bitVec is a boolean vector representing the message to hash in binary
*/
string SHA256::computeHash(vector<bool> bitVec) {

    // reset initial hash values
    uint32_t h0 = 0x6a09e667, h1 = 0xbb67ae85, h2 = 0x3c6ef372, h3 = 0xa54ff53a,
             h4 = 0x510e527f, h5 = 0x9b05688c, h6 = 0x1f83d9ab, h7 = 0x5be0cd19;

    // Pad the message as per SHA-256 requirements
    pad(bitVec); 

    // Convert bit vector to 32-bit word vector
    vector<uint32_t> messageWords = convertToWords(bitVec); 

    // Process each 512-bit chunk
    for (size_t i = 0; i < messageWords.size(); i += 16) {
        processChunk(&messageWords[i]); // <-- ptr to the start of the next 16 32-bit words
    }

    // Output the final hash value (convert to a desired format, e.g., hexadecimal string)
    std::string hashHex = collectDigest(); 
    // std::cout << "Hash: " << hashHex << std::endl;

    return hashHex;
}

/**
 * @note pad() pads a vector of bits (represented as bools) to a len that is a multiple of 512 bits by 
 * first appending a single '1' bit, then the amount of zeroes such that the big endian representation 
 * of the length of the orignal message can be appended to the end of the padded message and the length 
 * still be a multiple of 512 bits.
 * @param bitVec a vector of bits representing a vector of bools
*/
void SHA256::pad(vector<bool>& bitVec) {

    // Save original length (in bits)
    size_t originalLength = bitVec.size();

    // Pad with a '1' bit
    bitVec.push_back(true);

    // Check if the message is in the critical range between 56 bytes (448 bits) and 64 bytes (512 bits) inclusive
    // After adding the '1' bit, it checks if there's enough space for the 64-bit length representation.
    // If not, the message would overflow into a new 512-bit block, requiring an adjustment in the padding calculation.
    size_t totalLengthWithOneBit = originalLength + 1; // Total length after adding the '1' bit
    bool needsExtraBlock = totalLengthWithOneBit > (512 - 1 - 64);

    // Calculate padding length, adjusting for an extra block if needed
    size_t padLen;
    if (needsExtraBlock) {
        // Calculate extra padding to push length into new block
        padLen = (1024 - 1 - 64 - originalLength) % 512;
    } else {
        // Normal padding calculation
        padLen = (512 - 1 - 64 - originalLength) % 512;
    }

    // Pad with '0's
    for (size_t i = 0; i < padLen; ++i) {
        bitVec.push_back(false);
    }

    // Compute big endian representation of the original length (in bits)
    vector<bool> lenBitsBigEndian(64, false);
    for (int i = 63; i >= 0; --i) {
        lenBitsBigEndian[i] = (originalLength % 2) == 1;
        originalLength /= 2;
    }

    // Append the big endian length of the original message
    bitVec.insert(bitVec.end(), lenBitsBigEndian.begin(), lenBitsBigEndian.end());

    // Ensure the total length is now a multiple of 512 bits
    assert(bitVec.size() % 512 == 0);
}

/**
 * @note convertToWords() converts a vector of bits (represented as bools) to a vector of 32-bit words.
*/
vector<uint32_t> SHA256::convertToWords(const vector<bool>& bitVec) {

    vector<uint32_t> words;
    uint32_t currentWord = 0;

    // Iterate through the bitVec 32 bits at a time
    for (size_t i = 0; i < bitVec.size(); i++) {

        // Set the i-th bit of currentWord by xor-ing it with the i-th bit of bitVec
        // i'th bit is computed as 31 - (i % 32) because we are iterating from left to right
        currentWord |= (uint32_t(bitVec[i]) << (31 - (i % 32)));

        // Every 32 bits, push the currentWord to words and reset currentWord
        if ((i % 32) == 31) {
            words.push_back(currentWord);
            currentWord = 0;
        }
    }

    // Handling the case where the bitVec's size isn't a multiple of 32
    // If there are remaining bits, they are already correctly positioned in currentWord
    if (bitVec.size() % 32 != 0) {
        words.push_back(currentWord);
    }

    return words;
}

/**
 * @note processChunk processes each 512 bit chunk of the initial input bit vector, updating
 * the class variables h0-h7
*/
void SHA256::processChunk(const uint32_t* chunk) {

    // @note w is the msg schedule array. rest are working variables
    uint32_t w[64], a, b, c, d, e, f, g, h, temp1, temp2, S0, S1;

    // Initialize the first 16 words in the array W[0..15]
    for (int i = 0; i < 16; ++i) {
        w[i] = chunk[i];
    }

    // Extend the first 16 words into the remaining 48 words of the message schedule array
    for (int i = 16; i < 64; ++i) {
        uint32_t s0 = sigma0(w[i-15]);
        uint32_t s1 = sigma1(w[i-2]);
        w[i] = w[i-16] + s0 + w[i-7] + s1;
    }

    // Initialize working variables to current hash value
    a = h0; b = h1; c = h2; d = h3;
    e = h4; f = h5; g = h6; h = h7;

    // Compression function main loop
    for (int i = 0; i < 64; ++i) {
        S1 = Sigma1(e);
        uint32_t ch = Choice(e, f, g);
        temp1 = h + S1 + ch + k[i] + w[i];
        S0 = Sigma0(a);
        uint32_t maj = Majority(a, b, c);
        temp2 = S0 + maj;

        h = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;
    }

    // Add the compressed chunk to the current hash value
    h0 += a; h1 += b; h2 += c; h3 += d;
    h4 += e; h5 += f; h6 += g; h7 += h;
}


/**
 * @note collectDigest() returns appends the final hash values into a hexadecimal string
*/
string SHA256::collectDigest() const {

    std::ostringstream result;
    
    // collect current hashg vals into array for iteration
    uint32_t hashVals[8] = {h0, h1, h2, h3, h4, h5, h6, h7};
    for(int i = 0; i < 8; i++) {

        // append hash values into hexadecimal string
        result << std::hex << std::setfill('0') << std::setw(8) << hashVals[i];
    }

    return result.str();
}

/**
 * @note A right rotation bit manipulation shifts bits to the right a set number of times.
 * to which the bits that "fall off" the right end are reinserted at the left.
 * @param n The number of bits to rotate.
 * @param x The value to rotate.
 */
uint32_t SHA256::rightRotate(uint32_t n, uint32_t x) {
    return (x >> n) | (x << (32 - n));
}

/**
 * @note  The Choice function selects between two inputs based on the value of a third input.
 * It operates bit by bit: for each bit position, if the bit in x is 1, it selects the 
 * corresponding bit from y; otherwise, it chooses the bit from z.
*/
uint32_t SHA256::Choice(uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) ^ (~x & z);
}

/**
 * @note The Majority function works by considering the bits of three inputs at each position
 * and outputs the majority value of the three for each bit position. If at least two of the 
 * three bits are 1, the result is 1; otherwise, it's 0. and outputs the majority value of the
 * three for each bit position. If at least two of the three bits are 1, the result is 1; 
 * otherwise, it's 0.
*/
uint32_t SHA256::Majority(uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) ^ (x & z) ^ (y & z);
}

/**
 * @note Sigma0 is a bit manipulation function that operates on 32-bit words and returns a new 32-bit word.
 * It is used in the SHA-256 hash function. Sigma0 uses 2, 13, and 22 bit rotations to manipulate the input.
*/
uint32_t SHA256::Sigma0(uint32_t x) {
    return rightRotate(2, x) ^ rightRotate(13, x) ^ rightRotate(22, x);
}

/**
 * @note Sigma1 is a bit manipulation function that operates on 32-bit words and returns a new 32-bit word.
 * It is used in the SHA-256 hash function. Sigma1 uses 6, 11, and 25 bit rotations to manipulate the input.
*/
uint32_t SHA256::Sigma1(uint32_t x) {
    return rightRotate(6, x) ^ rightRotate(11, x) ^ rightRotate(25, x);
}

/**
 * @note sigma0 is a bit manipulation function that operates on 32-bit words and returns a new 32-bit word.
 * It is used in the SHA-256 hash function. sigma0 uses 7, 18, and 3 bit rotations to manipulate the input.
 * The result is then XORed with the right shift of x by 3.
*/
uint32_t SHA256::sigma0(uint32_t x) {
    return rightRotate(7, x) ^ rightRotate(18, x) ^ (x >> 3);
}

/**
 * @note sigma1 is a bit manipulation function that operates on 32-bit words and returns a new 32-bit word.
 * It is used in the SHA-256 hash function. sigma1 uses 17, 19, and 10 bit rotations to manipulate the input.
 * The result is then XORed with the right shift of x by 10.
*/
uint32_t SHA256::sigma1(uint32_t x) {
    return rightRotate(17, x) ^ rightRotate(19, x) ^ (x >> 10);
}

// ! TODO write a test for this
vector<bool> SHA256::stringToBinary(const std::string& input) {
    std::vector<bool> binaryRepresentation;

    for (char character : input) {
        // Convert each character to its ASCII binary representation.
        // std::bitset<8> represents an 8-bit binary number.
        std::bitset<8> bits(character);
        
        // Add each bit to the vector.
        for (int i = 0; i < 8; ++i) {
            binaryRepresentation.push_back(bits[i]);
        }
    }

    // Reverse each byte's bits to match the endianness (std::bitset starts from the least significant bit).
    for (size_t i = 0; i < binaryRepresentation.size(); i += 8) {
        std::reverse(binaryRepresentation.begin() + i, binaryRepresentation.begin() + i + 8);
    }

    return binaryRepresentation;
}

