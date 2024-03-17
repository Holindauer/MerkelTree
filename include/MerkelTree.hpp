#pragma once

class MerkelTree{

    public:

        MerkelTree();   

        SHA256 sha256;

        vector<string> hashInputStrings(vector<string> inputMessages);



        typedef struct node{

            string hash1;
            string hash2;

            struct node** ancestsors;
            struct node* up;


        }Node;

    private:
};