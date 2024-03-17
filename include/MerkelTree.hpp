#pragma once

// tree node 
typedef struct node{
            
    // hash data
    string inputHash1;
    string inputHash2;
    string combinedHash;

    // relative nodes in tree
    struct node** ancestors;

}TreeNode;

class MerkelTree{

    public:
        MerkelTree();   

        // hash function
        SHA256 sha256;

        // merkel tree funcs
        vector<string> hashStrings(vector<string> input);
        vector<string> pairHashes(vector<string> hashes);
        string assembleTree(vector<string> input);

};