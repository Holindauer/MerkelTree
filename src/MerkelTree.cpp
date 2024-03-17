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
 * @note hashStrings() hashes all string inputs (current leaf nodes) 
 * @param input is a vector of strings containing the data to be hashed
 * @returns vector of hash strings
*/
vector<string> MerkelTree::hashStrings(vector<string> input){

    // hash all input strings after converting to binary vectors
    vector<string> hashes;
    for (int i=0; i<input.size(); i++){
        vector<bool> bin = sha256.stringToBinary(input[i]);
        hashes.push_back(sha256.computeHash(bin));
    }
    return hashes;
}

/**
 * @note pairHashes() groups hashes into pairs and appends them for further hashing
 * @param hashes a vector of pre computed hash outputs to group up
 * @return a vector half the size of the input containing str appended hash pairs
*/
vector<string> MerkelTree::pairHashes(vector<string> hashes){

    // group hashes into pairs
    vector<string> pairedHashes;
    for (int i=0; i<hashes.size(); i++){ 

        // get first hash in pair
        string hash = hashes[i];

        // get second hash if exists
        if (i+1 <= hashes.size()){

            // append first hash to second
            hash.append(hashes[i+1]);
            i++;
        }        

        // push hash pair
        pairedHashes.push_back(hash);
    }

    return pairedHashes;
}

/**
 * @note assembleTree() creates computes a merkel tree from a vector of input strings
 * @dev by default, the nodes vector is set to nullptr so non recursive calls do not need 
 * to specify an argument.
*/
string MerkelTree::assembleTree(vector<string> input){

    // hash inputs and pair
    vector<string> hashes = hashStrings(input);
    vector<string> hashPairs = pairHashes(hashes);

    // hash pairs until root node is established
    while (hashPairs.size() != 1){
        
        // hash together hash pairs and further pair them
        hashes = hashStrings(hashPairs);
        hashPairs = pairHashes(hashes);
    }

    // hash the final concatenation
    return hashStrings(hashPairs)[0];    
}