#include "lib.hpp"

// the merkel tree will start with hashing all strings input to the merkel tree. 
// Then pairs of hashes will be formed and appened together. These will be further 
// hashed until a single hash is formed. This hash will be the root of the merkel tree.


/**
 * @note constructor initializes SHA256 class as hash func for merkel tree
*/
MerkelTree::MerkelTree(){
    sha256 = SHA256();
}

/**
 * @note hashInputStrings() hashes all inputs (original leaf nodes) input into the merkel tree
 * @param input is a vector of strings containing the data to be hashed
 * @returns vector of hash strings
*/
vector<string> MerkelTree::hashInputStrings(vector<string> input){

    // hash all input strings after converting to binary vectors
    vector<string> hashes;
    for (int i=0; i<input.size(); i++){
        vector<bool> bin = sha256.stringToBinary(input[i]);
        hashes.push_back(sha256.computeHash(bin));
    }
    return hashes;
}
