#include "lib.hpp"


class MerkelTree{

    public:

    typedef struct node{

        uint32_t hash1;
        uint32_t hash2;

        struct node** ancestsors;
        struct node* up;


    }Node;

    private:



};