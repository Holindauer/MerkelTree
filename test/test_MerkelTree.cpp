#include "lib.hpp"


/**
 * @test test_hashLeadNodes() ensures that the hashLeafNodes() function hashes all strings
 * input into the merkel tree computation.
 * @note SHA256 is assumed to be tested elsewhere and passing.
*/
void test_hashInputStrings(void){  

    // instantiate merkel tree and sha classes
    MerkleTree merkelTree = MerkleTree();
    SHA256 sha256 = SHA256();

    // input strings for merkel tree
    vector<string> inputs = {"Hello", "World", "!"};

    // hash inputs
    vector<string> hashes = merkelTree.hashStrings(inputs);

    // assert hashes in output vector match hash computaiton from sha256
    for (int i=0; i<inputs.size(); i++){
        assert(hashes[i] == sha256.computeHash(sha256.stringToBinary(inputs[i])));
    }

    cout << "test_hashLeafNodes()...Pass!" << endl;
}


/**
 * @test test_assembleTree() tests to make sure that the assembleTree function correctly constructs
 * a Merkle tree from a vector of strings
 */
void test_assembleTree(){

    MerkleTree merkelTree = MerkleTree();

    vector<string> inputs = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};

    // Assemble the tree
    TreeNode* root = merkelTree.assembleTree(inputs);

    // cleanup
    merkelTree.freeTree(&root);
    assert(root == nullptr);


    cout << "test_assembleTree()...PASS!" << endl;
}


int main(void){
    test_hashInputStrings();
    test_assembleTree();
    return 0;
}