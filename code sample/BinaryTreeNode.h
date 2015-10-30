#ifndef BINARYTREENODE_H
#define BINARYTREENODE_H
#include "Element.h"
class BinaryTreeNode {
    public:
        Element pinfo;
        BinaryTreeNode *left_son, *right_son;

        // constructor
        BinaryTreeNode(){
            left_son = right_son = NULL;
        }
        
        // destructor
        ~BinaryTreeNode() {
        if (left_son != NULL)
            delete left_son;
        if (right_son != NULL)
            delete right_son;
    }
};
#endif // BINARYTREENODE_H
