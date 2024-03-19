#pragma once

// tree node 
typedef struct node{
            
    // hash data
    string hash;

    // relative nodes in tree
    struct node* ancestors[2];
}TreeNode;

class MerkelTree{

    public:
        MerkelTree();   

        // hash function
        SHA256 sha256;

        // merkel tree funcs
        vector<string> hashStrings(vector<string> input);
        string computeRootHash(vector<string> input);
        TreeNode* newTreeNode(TreeNode* inputHash1, TreeNode* inputHash2);
        TreeNode* assembleTree(vector<string> input);

};