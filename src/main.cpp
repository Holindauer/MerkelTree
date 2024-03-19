#include "lib.hpp"


int main(int argc, char* argv[]) {
    // Check if there are any string arguments
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " [string1] [string2] ..." << std::endl;
        return 1;
    }

    // Read arguments into a vector of strings
    std::vector<std::string> inputs;
    for (int i = 1; i < argc; ++i) {
        inputs.push_back(argv[i]);
    }

    // Instantiate a MerkelTree object
    MerkleTree merkelTree;

    // Assemble the Merkle tree with the input strings
    TreeNode* root = merkelTree.assembleTree(inputs);

    // handle no inputs
    if (root == nullptr) {
        std::cout << "Failed to assemble the Merkle tree." << std::endl;
        return 1;
    }

    // print
    std::cout << "Root hash: " << root->hash << std::endl;

    // cleanup
    merkelTree.freeTree(&root);

    return 0;
}
