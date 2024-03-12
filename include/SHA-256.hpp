#pragma once

// SHA-256.hpp

class SHA256 {
    /**
     * @notice The SHA256 class implements the SHA-256 hashing algorithm. It provides methods for 
     * loading a a file into the program in binary mode, hashing the file, and returning the hash
     * 
     * This implementation is based on the Pseudocode section of this wikipedia page: 
     * https://en.wikipedia.org/wiki/SHA-2#References
    */

    public:

        // Constructor
        SHA256() = default;

        // Destructor
        ~SHA256() = default;

        // reading from files
        vector<unsigned char> read512Bits(ifstream &file);

        // pre hash processing
        void processFile(const std::string& filename);
        void applyPadding(vector<unsigned char>& block, uint64_t totalLength);

        // bit manipulations for SHA-256 
        /**
         * @note A right rotation bit manipulation shifts bits to the right a set number of times.
         * to which the bits that "fall off" the right end are reinserted at the left.
         * @param n The number of bits to rotate.
         * @param x The value to rotate.
        */
        static inline uint32_t rightRotate(uint32_t n, uint32_t x) {
            return (x >> n) | (x << (32 - n));
        }

        /**
         * @note  The Choice function selects between two inputs based on the value of a third input.
         * It operates bit by bit: for each bit position, if the bit in x is 1, it selects the 
         * corresponding bit from y; otherwise, it chooses the bit from z.
        */
        static inline uint32_t Choice(uint32_t x, uint32_t y, uint32_t z) {
            return (x & y) ^ (~x & z);
        }

        /**
         * @note The Majority function works by considering the bits of three inputs at each position
         * and outputs the majority value of the three for each bit position. If at least two of the 
         * three bits are 1, the result is 1; otherwise, it's 0. and outputs the majority value of the
         * three for each bit position. If at least two of the three bits are 1, the result is 1; 
         * otherwise, it's 0.
        */
        static inline uint32_t Majority(uint32_t x, uint32_t y, uint32_t z) {
            return (x & y) ^ (x & z) ^ (y & z);
        }

        /**
         * @note Sigma0 is a bit manipulation function that operates on 32-bit words and returns a new 32-bit word.
         * It is used in the SHA-256 hash function. Sigma0 uses 2, 13, and 22 bit rotations to manipulate the input.
        */
        static inline uint32_t Sigma0(uint32_t x) {
            return rightRotate(2, x) ^ rightRotate(13, x) ^ rightRotate(22, x);
        }

        /**
         * @note Sigma1 is a bit manipulation function that operates on 32-bit words and returns a new 32-bit word.
         * It is used in the SHA-256 hash function. Sigma1 uses 6, 11, and 25 bit rotations to manipulate the input.
        */
        static inline uint32_t Sigma1(uint32_t x) {
            return rightRotate(6, x) ^ rightRotate(11, x) ^ rightRotate(25, x);
        }

        /**
         * @note sigma0 is a bit manipulation function that operates on 32-bit words and returns a new 32-bit word.
         * It is used in the SHA-256 hash function. sigma0 uses 7, 18, and 3 bit rotations to manipulate the input.
         * The result is then XORed with the right shift of x by 3.
        */
        static inline uint32_t sigma0(uint32_t x) {
            return rightRotate(7, x) ^ rightRotate(18, x) ^ (x >> 3);
        }

        /**
         * @note sigma1 is a bit manipulation function that operates on 32-bit words and returns a new 32-bit word.
         * It is used in the SHA-256 hash function. sigma1 uses 17, 19, and 10 bit rotations to manipulate the input.
         * The result is then XORed with the right shift of x by 10.
        */
        static inline uint32_t sigma1(uint32_t x) {
            return rightRotate(17, x) ^ rightRotate(19, x) ^ (x >> 10);
        }


    private:

        // hash values: (first 32 bits of the fractional parts of the square roots of the first 8 primes 2..19):
        uint32_t h0 = 0x6a09e667;
        uint32_t h1 = 0xbb67ae85;
        uint32_t h2 = 0x3c6ef372;
        uint32_t h3 = 0xa54ff53a;
        uint32_t h4 = 0x510e527f;
        uint32_t h5 = 0x9b05688c;
        uint32_t h6 = 0x1f83d9ab;
        uint32_t h7 = 0x5be0cd19;

        // array of round constants (initialized outside class def)
        static const uint32_t k[64];

};

