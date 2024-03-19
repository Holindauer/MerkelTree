#include "lib.hpp"

/**
 * Merkel Tree Protocol
 * 
 * Step 1:
 * The assembly of the merkel tree requires that the inital string inputs are all hashed. Then
 * these hashes must be grouped into pairs and formed into single strings by concatenating each
 * hash string together. 
 * 
 * Step 2:
 * Then until the final route node is created, these hash pair concatenations must themselves be 
 * hashed, and paired.
*/

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
 * @note newTreeNode() allocates memory for a new tree node and computes its hash from the 
 * concatenated hashes of its ancestors.
 * @param ancestor1 is the first ancestor of the new node
 * @param ancestor2 is the second ancestor of the new node
*/
TreeNode* MerkelTree::newTreeNode(TreeNode* ancestor1, TreeNode* ancestor2){

    // allocate mem for new node
    TreeNode* treeNode = new TreeNode;

    // cat hashes if they exist
    string hashedPair = "";
    if (ancestor1 != nullptr) { hashedPair.append(ancestor1->hash); }
    if (ancestor2 != nullptr) { hashedPair.append(ancestor2->hash); }

    // hash catted hashes, and set into treeNode
    treeNode->hash = sha256.computeHash(sha256.stringToBinary(hashedPair));

    // set ancestors
    treeNode->ancestors[0] = ancestor1;
    treeNode->ancestors[1] = ancestor2;

    return treeNode;
}   

/**
 * @freeTree recursively deallocates all nodes in the tree from the root down
 * @param root is a ptr to a ptr to the root node
*/
void MerkelTree::freeTree(TreeNode** root){ 

    // base case
    if (*root == nullptr){
        return;
    }

    // Recursively free remaining subtrees
    if ((*root)->ancestors[0] != nullptr){
        freeTree(&((*root)->ancestors[0]));
    }
    if ((*root)->ancestors[1] != nullptr){
        freeTree(&((*root)->ancestors[1]));
    }

    // deallocate the current node, set to nullptr
    delete *root;
    *root = nullptr;        
}

/**
 * @note assembleTree() assembles a merkel tree out of treeNode structs by first hashing all string inputs. Then
 * these hashes are packaged within dynamically allocated treeNode structs, forming the base layer of the tree. 
 * the binary tree is then from the lowest level until the root node is formed.
 * */
TreeNode* MerkelTree::assembleTree(vector<string> input){
    assert(input.size() > 0);

    // compute initial hashes from input vector
    vector<string> hashes = hashStrings(input);

    // create base layer of the tree w/ hashes of original str data
    vector<TreeNode*> nodesVec;
    for (int i=0; i<hashes.size(); i++){

        // allocate mem for tree node without ancestors and set hash
        TreeNode* node = newTreeNode(nullptr, nullptr);

        // set hash of nodes manually
        node->hash = hashes[i];
        nodesVec.push_back(node);
    }

    // assemble tree from base up until root node established
    while (nodesVec.size() != 1){
        
        // build next layer of tree, hashing concatted hashes of paired nodes 
        vector<TreeNode*> tempNodesVec;
        for (int i=0; i<nodesVec.size(); i++){
            
            // new tree node, func sets cat hash within node. 
            TreeNode* newNode;
            if (i+1 < nodesVec.size()){ // handle even num nodes
                newNode = newTreeNode(nodesVec[i], nodesVec[i+1]); 
                i++; // increment for two nodes added
            }
            else { // handle odd num nodes
                newNode = newTreeNode(nodesVec[i], nullptr);
            }

            // push to temp vec
            tempNodesVec.push_back(newNode);
        }

        // reset nodesVec for next iter
        nodesVec.clear();
        for(int i=0; i<tempNodesVec.size(); i++){
            nodesVec.push_back(tempNodesVec[i]);
        }
    }

    return nodesVec[0]; // root node
}