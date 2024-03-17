#pragma once

class MerkelTree{

    public:
        MerkelTree();   

        // hash function
        SHA256 sha256;

        // merkel tree funcs
        vector<string> hashStrings(vector<string> inputMessages);
        vector<string> pairHashes(vector<string> hashes);


        // tree node 
        typedef struct node{

            string hash1;
            string hash2;

            struct node** ancestsors;
            struct node* up;
        }Node;

    private:
};