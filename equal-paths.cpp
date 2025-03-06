#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
//I'm not taking chances lmao
#endif

#include "equal-paths.h"

using namespace std;

// You may add any prototypes of helper functions here

int pathLengthToNull(Node* node);

int max(const int& l, const int& r);

bool equalPaths(Node * root)
{
    // Add your code below

    if (root == nullptr) {
        return true;
    }

    int left_length = pathLengthToNull(root->left);
    int right_length = pathLengthToNull(root->right);

    bool leftNull = root->left == nullptr;
    bool rightNull = root->right == nullptr;

    return (left_length == right_length
            || (leftNull != rightNull))//cheeky logical XOR
            && equalPaths(root->left)
            && equalPaths(root->right);

}

int pathLengthToNull(Node* node) {
    if (node == nullptr) {
        return 0;
    }

    return max(pathLengthToNull(node->left), pathLengthToNull(node->right)) + 1;
}

int max(const int& l, const int& r) {
    if (l > r) {
        return l;
    }
    return r;
}



