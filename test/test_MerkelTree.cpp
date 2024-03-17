#include "lib.hpp"


/**
 * @test test_hashLeadNodes() ensures that the hashLeafNodes() function hashes all strings
 * input into the merkel tree computation.
 * @note SHA256 is assumed to be tested elsewhere and passing.
*/
void test_hashInputStrings(void){  

    // instantiate merkel tree and sha classes
    MerkelTree merkelTree = MerkelTree();
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
 * @test test_pairHashes() checks to make sure that hashes are being paired up and appended 
 * when pairHashes() is called 
*/
void test_pairHashes(){

    // instantiate merkel tree and sha classes
    MerkelTree merkelTree = MerkelTree();
    SHA256 sha256 = SHA256();

    // input strings for merkel tree
    vector<string> inputs = {"1", "2", "3", "4", "5", "6"};

    // hash inputs
    vector<string> hashes = merkelTree.hashStrings(inputs);

    // pair hashes
    vector<string> pairedHashes = merkelTree.pairHashes(hashes);

    // ensure hashes are paired as expected
    assert(hashes[0].append(hashes[1]) == pairedHashes[0]);
    assert(hashes[2].append(hashes[3]) == pairedHashes[1]);
    assert(hashes[4].append(hashes[5]) == pairedHashes[2]);

    cout << "test_pairHashes()...Pass!" << endl;
}

/**
 * @test test_assembleTree() tests to make sure that the assembleTree function is able to 
 * bring a collection of input strings to a single hash via the assembleTree() method
 * 
*/
void test_assembleTree(){

    MerkelTree merkelTree = MerkelTree();

    vector<string> inputs = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};

    string rootHash = merkelTree.assembleTree(inputs);

    cout << "test_assembleTree()...PASS!" << endl;
}


int main(void){
    test_hashInputStrings();
    test_pairHashes();
    test_assembleTree();
    return 0;
}