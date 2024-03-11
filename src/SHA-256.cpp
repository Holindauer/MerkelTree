#include "lib.hpp"

// SHA-256.cpp

// init array of round constants
const uint32_t SHA256::k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

/**
 * @note read512Bits() reads 512 bits (64 bytes) from a file and stores them in a byte array.
 * @param file Reference to an open ifstream from which to read the data.
 * @return A vector of unsigned char containing the read bytes.
 */
vector<unsigned char> SHA256::read512Bits(ifstream &file) {
    
    std::vector<unsigned char> block(64); // 512 bits = 64 bytes
    file.read(reinterpret_cast<char*>(&block[0]), 64); // Cast to char* for compatibility with read()

    return block;
}

/**
 * @note Processes the given file, reads in 512-bit chunks, and applies padding if necessary.
 * @param filename Path to the file to be processed.
 */
void SHA256::processFile(const string& filename) {

    // Open the file in binary mode
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Failed to open file: " << filename << endl;
        return;
    }

    // block of 512 bit chunks from file
    vector<unsigned char> block;
    uint64_t totalLength = 0; // track len in bits

    // Read the file in 512-bit chunks
    while (file.good()) {

        // read 512 bits from file and determine the number of bytes read
        block = read512Bits(file);
        size_t bytesRead = file.gcount();
        totalLength += bytesRead * 8; 

        // Check if this is the last chunk
        if (bytesRead < 64) { 
            applyPadding(block, totalLength);
    
            // Process padded block here (e.g., hash it)

            break;
        }

        // Process block here (e.g., hash it)
    }

    file.close();
}

/**
 * @note Applies the SHA-256 padding to the last block of the message. This process includes appending a single '1' bit,
 * followed by '0' bits, such that the total length (including padding) becomes congruent to 448 mod 512. The final
 * 64 bits of the block encode the total message length in big-endian format, ensuring compliance with SHA-256 specs.
 * This guarantees that the padded message's length is a multiple of 512 bits, a prerequisite for SHA-256 processing.
 * @param block The last (or only) block of data read from the file, to be padded.
 * @param totalLength The total length of the message in bits, used for encoding in the padding.
 */
void SHA256::applyPadding(vector<unsigned char>& block, uint64_t totalLength) {
    size_t originalSize = block.size();

    // Add the '1' bit
    if (originalSize < 64) {
        block.push_back(0x80); // 0x80 = 10000000 in binary
    }

    // Pad with '0' bits
    while (block.size() < 64) {
        block.push_back(0x00);
    }

    // Overwrite the last 8 bytes with the total message length in bits
    for (int i = 0; i < 8; i++) {

        // Big endian format is computed by shifting right by 56, 48, 40, 32, 24, 16, 8, 0 then mask
        // with 0xFF (0b11111111) to get the last 8 bits (1 byte) of the total length. Each byte mask
        // is then stored in the last 8 bytes of the block, starting from index 56.
        block[56 + i] = (totalLength >> (56 - i * 8)) & 0xFF;
    }
}