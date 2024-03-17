#include "lib.hpp"


/**
 * @test test_hashLeadNodes() ensures that the hashLeafNodes() function hashes all strings
 * input into the merkel tree computation.
 * @note SHA256 is assumed to be tested elsewhere and passing.
*/
void test_hashInputStrings(void){  

    // instantiate merkel tree class
    MerkelTree merkelTree = MerkelTree();
    SHA256 sha256 = SHA256();

    // input strings for merkel tree
    vector<string> inputs = {"Hello", "World", "!"};

    // hash inputs
    vector<string> hashes = merkelTree.hashInputStrings(inputs);

    // assert hashes in output vector match hash computaiton from sha256
    for (int i=0; i<inputs.size(); i++){
        assert(hashes[i] == sha256.computeHash(sha256.stringToBinary(inputs[i])));
    }

    cout << "test_hashLeafNodes()...Pass!" << endl;
}


int main(void){
    test_hashInputStrings();
    return 0;
}