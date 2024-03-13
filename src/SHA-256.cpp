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
 * @note readFileBits() reads all bits  from a file and stores them in a bit vector.
*/
vector<bool> SHA256::readFileBits(const string& fileName) {

    // open file
    ifstream file(fileName, ios::binary); 
    assert(file.good());

    vector<bool> bitArr;
        
    while (file.good()) {

        // Read 64 bytes from the file into a buffer
        vector<unsigned char> buffer(64); 
        file.read(reinterpret_cast<char*>(&buffer[0]), buffer.size());
        
        // Iterate through the bytes in the buffer
        for (int i = 0; i < file.gcount(); ++i) {

            // iterate backwards through the bits of the current byte
            for (int j = 7; j >= 0; --j) {

                // Push the bits of the current byte onto the bit vector by
                // shifting the bits to the right and masking with 1
                bitArr.push_back((buffer[i] >> j) & 1);
            }
        }
    }

     return bitArr; 
}

/**
 * @helper bitToByteVector() converts a bit vector (represented as vec of bools) to a byte vector.
*/
vector<unsigned char> bitToByteVector(const vector<bool>& bitVec) {

    vector<unsigned char> byteVec;
    unsigned char currentByte = 0; // temp for bit/btye conversion

    // iterate bits in bit vector
    for (size_t i = 0; i < bitVec.size(); ++i) {

        // if the i'th bit is true, set the corresponding bit to '1' in the current byte
        if (bitVec[i]) {    

            // (1 << (7 - (i % 8))) calculates how many positions away from the LSB the '1' should be placed in
            // the current byte. The placement itself is done at that position with the bitwise OR operation.
            currentByte |= (1 << (7 - (i % 8)));
        }

        // Every 8 bits, push the current byte to the byte vector and reset
        if (i % 8 == 7) {
            byteVec.push_back(currentByte);
            currentByte = 0; 
        }
    }

    // Handle any remaining bits if bitVec.size() is not a multiple of 8
    if (bitVec.size() % 8 != 0) {
        byteVec.push_back(currentByte);
    }

    return byteVec;
}

/**
 * @helper computeBigEndianBits() computes the big endian representation of a size_t and stores it in a bit vector.
 * @param size The size_t to convert to big endian.
 * @param endianVec The bit vector to store the big endian representation in.
*/
void SHA256::computeBigEndian(size_t size, vector<bool>& endianVec) {

    // iterate from MSB, fill endianVec with the bits of 'size'
    for (int i = 63; i >= 0; --i) {

        // Extract each bit by shifting 'size' right by 'i' positions and isolate the LSB
        bool bit = (size >> i) & 1;

        // populate bit vector from MSB to LSB
        endianVec[63 - i] = bit;
    }
}


/**
 * @note applyPadding() pads a vector of bits (represented as a vector of bools) to a len that is a multiple of 512 bits
*/
void SHA256::applyPadding(vector<bool>& bitVec) {

    // Pad with a '1' bit
    bitVec.push_back(true);

    uint padLen = 512 - bitVec.size() - 64; // 64 for the length of the original message

    // compute big endian len of bitVec
    vector<bool> lenBitsBigEndian(64, false);
    computeBigEndian(bitVec.size(), lenBitsBigEndian);

    // pad with 0's
    for (uint i = 0; i < padLen; ++i) { bitVec.push_back(false); }

    // append the big endian len of bitVec
    bitVec.insert(bitVec.end(), lenBitsBigEndian.begin(), lenBitsBigEndian.end());

    assert(bitVec.size() % 512 == 0);
}


/**
 * @note hashBytes() applies the SHA-256 hashing algorithm to a vector of bytes. The bytes are padded if the length
 * of the byte vector is not a multiple of 64. 
*/
void SHA256::hashBits(vector<bool>& bitVec){

    // check if arr len is a multple of 64x
    if (bitVec.size() % 64 != 0) {
        applyPadding(bitVec);
    }
    
    // convert bit vector to byte vector
    vector<unsigned char> byteVec = bitToByteVector(bitVec);

    // iterate through the byte vector in 512-bit blocks
    for (size_t i = 0; i < byteVec.size(); i += 64) {

        // process each 512-bit block
        processBlock(vector<unsigned char>(byteVec.begin() + i, byteVec.begin() + i + 64));
    

}


/**
 * @note processBlock() applies the 'rolling hash function' to a 512-bit block of data. The block is divided into 16
 * 32-bit words, which are then extended into 64 32-bit words using a message schedule array. The extended words are
 * then used to update the hash value through a series of bitwise operations and additions. The final hash value is
 * then used to update the current hash value. 
 * @
*/
void SHA256::processBlock(const std::vector<unsigned char>& byteBlock) {
    assert(byteBlock.size() == 64); // Ensure the block is 512 bits (64 bytes)

    // Init message schedule array
    std::array<uint32_t, 64> msgSched = {0};

    // Copy the first 16 words directly from the block into the message schedule array
    for (size_t i = 0; i < 16; ++i) {

        // Big endian format computed by shifting left by 24, 16, 8, 0 then mask with 0xFF (0b11111111)
        msgSched[i] = (byteBlock[i*4] << 24) | (byteBlock[i*4 + 1] << 16) | (byteBlock[i*4 + 2] << 8) | (byteBlock[i*4 + 3]);
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




