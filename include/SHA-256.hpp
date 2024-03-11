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

