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
    
            // check if block is 128 bytes, split into two 64 byte blocks and process each (This will  
            // happen if last block is between 57 and 64 bytes bc padding will extend it to 128 bytes)
            if (block.size() == 128) {
                vector<unsigned char> block1(block.begin(), block.begin() + 64);
                vector<unsigned char> block2(block.begin() + 64, block.end());
                processBlock(block1);
                processBlock(block2);
            } else {
                processBlock(block);
                break;
            }
        }

        // Process block into hash value
        processBlock(block);

    } file.close();

    // final hash value 
    string digest = getDigest();
}

/**
 * @note processBlock() applies the 'rolling hash function' to a 512-bit block of data. The block is divided into 16
 * 32-bit words, which are then extended into 64 32-bit words using a message schedule array. The extended words are
 * then used to update the hash value through a series of bitwise operations and additions. The final hash value is
 * then used to update the current hash value. 
*/
void SHA256::processBlock(const std::vector<unsigned char>& block) {
    assert(block.size() == 64); // Ensure the block is 512 bits (64 bytes)

    // Init message schedule array
    std::array<uint32_t, 64> msgSched = {0};

    // Copy the first 16 words directly from the block into the message schedule array
    for (size_t i = 0; i < 16; ++i) {

        // Big endian format computed by shifting left by 24, 16, 8, 0 then mask with 0xFF (0b11111111)
        msgSched[i] = (block[i*4] << 24) | (block[i*4 + 1] << 16) | (block[i*4 + 2] << 8) | (block[i*4 + 3]);
    }

    // Extend the first 16 words into the remaining 48 words of the message schedule array
    for (size_t i = 16; i < 64; ++i) {
        uint32_t s0 = sigma0(msgSched[i-15]);
        uint32_t s1 = sigma1(msgSched[i-2]);
        msgSched[i] = msgSched[i-16] + s0 + msgSched[i-7] + s1;
    }

    // Initialize working variables with current hash value
    uint32_t a = _h[0], b = _h[1], c = _h[2], d = _h[3], 
             e = _h[4], f = _h[5], g = _h[6], h = _h[7];

    // Compression function main loop
    for (size_t i = 0; i < 64; ++i) {

        // Compute next working variables' state
        uint32_t S1 = Sigma1(e);
        uint32_t ch = Choice(e, f, g);
        uint32_t temp1 = h + S1 + ch + k[i] + msgSched[i];
        uint32_t S0 = Sigma0(a);
        uint32_t maj = Majority(a, b, c);
        uint32_t temp2 = S0 + maj;

        // Update the working variables
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
    _h[0] += a; _h[1] += b; _h[2] += c; _h[3] += d;
    _h[4] += e; _h[5] += f; _h[6] += g; _h[7] += h;
}

/**
 * @note getDigest() returns the final hash value as a string of hexadecimal 
 * characters by combining the 8 32-bit words of the hash value.
*/
string SHA256::getDigest() const {

    std::ostringstream result;

    for(int i = 0; i < 8; i++) { // Loop through the final hash values

        // Convert the hash value to a hex string and append it to the result
        // (A hex string is a string representation of a number in base 16.)
        result << std::hex << std::setfill('0') << std::setw(8) << _h[i];
    }

    return result.str();
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

    // Original size in bytes
    size_t originalSize = block.size();

    // Calculate how many bytes we need to add to include the '1' bit and reach a size
    // that leaves exactly 8 bytes available for the total length encoding.
    size_t paddingSize = 64 - ((originalSize + 8) % 64);

    // Ensure there's space for at least one bit of padding ('1') by adding 64 bytes
    // If true, the block will become 128 bytes long, which is technically two blocks
    if (paddingSize < 1) {
        paddingSize += 64; 
    }

    // appened the '1'  bit followed by '0' bits (0x80 == 10000000)
    block.push_back(0x80); 
    
    // Append the '0' bits (0x00 == 00000000)
    for (size_t i = 1; i < paddingSize; ++i) {
        block.push_back(0x00);
    }

    // Ensure the block is the correct size before adding the length
    assert(block.size() == originalSize + paddingSize);

    // Add the total message length in bits at the end of the padding
    for (int i = 7; i >= 0; --i) {

        // Big endian format is computed by shifting right by 56, 48, 40, 32, 24, 16, 8, 0 then mask
        // with 0xFF (0b11111111) to get the last 8 bits (1 byte) of the total length. Each byte mask
        // is then stored in the last 8 bytes of the block, starting from index 56.
        block.push_back((totalLength >> (i * 8)) & 0xFF);
    }
}



