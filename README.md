# MerkleTree
SHA-256 and Merkel Tree implementation in C++

# Overview

This repository implements the SHA-256 hash function and uses it in the implementation of a merkle tree.

# SHA-256


SHA-256, is a cryptographic hash function of the SHA-2 family. It was designed by the NSA and published in 2001 by the National Institute of Standards and Technology as a U.S. Federal Information Processing Standard. SHA-256 accepts any format of data (as long as it can be represented in binary) and produces a 256-bit hash value represented as a 64 digit hexadecimal number. 

# Merkel Trees

A Merkle Tree is a type of binary tree data structure that is used to efficiently verify the integrity of large datasets. The tree is constructed by recursively hashing pairs of nodes until a single hash is left at the root. Because it is a tree structure, where individiual components of the dataset are hashed individually and combined, the location of tampering of a dataset can be identified by traversing the tree and finding where the hashes do not match.

# Running the Merkel Tree on Data

To run the merkel tree on a collection of data, run the following shell script with all string data to include:

    ./runMerkelTree.sh "1" "2" "3" "4" "5" "6" "7" "8" "9"  // and so on...

This will output the root hash for the inputted data. (Note that order matters)

    Root hash: b0c6663b9e2f633ddf0ff0c34e9c8419a3fda39d4dbed018af81da0e9c150860