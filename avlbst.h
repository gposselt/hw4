#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    static int getHeight(AVLNode<Key, Value>* node);

    void insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n);
    void finishLeftInsert(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n, AVLNode<Key, Value>* g);
    void finishRightInsert(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n, AVLNode<Key, Value>* g);

    static bool isZigZag(AVLNode<Key, Value>* g, AVLNode<Key, Value>* p, AVLNode<Key, Value>* n);

    void rotateRight(AVLNode<Key, Value>* z);
    void rotateLeft(AVLNode<Key, Value>* x);


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO

    AVLNode<Key, Value>* val = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);

    if (this->root_ == nullptr) {
        this->root_ = val;
        return;
    }

    AVLNode<Key, Value>* p = nullptr;
    AVLNode<Key, Value>* current = static_cast<AVLNode<Key, Value>*>(this->root_);


    while (current != nullptr) {
        p = current;
        if (val->getKey() < current->getKey()) {
            current = current->getLeft();
        }else if (val->getKey() == current->getKey()) {
            current->setValue(new_item.second);
            delete val;
            return;
        }else /*if (current->getRight() != nullptr)*/{
            current = current->getRight();
        }
    }



    val->setParent(p);

    if (p == nullptr) {
        this->root_ = val;
    } else if (val->getKey() < p->getKey()) {
        p->setLeft(val);
    } else {
        p->setRight(val);
    }

    val->setBalance(0);

    int8_t balance = p->getBalance();
    if (balance == -1 || balance == 1) {
        p->setBalance(0);

    }else if (balance == 0) {
        int leftHeight  = getHeight(p->getLeft());
        int rightHeight = getHeight(p->getRight());

        int newBalance = rightHeight - leftHeight;

        p->setBalance(newBalance);

        insertFix(p, val);
    }

}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

//safety wrapper around a node that can be null
template <class Key, class Value>
int AVLTree<Key, Value>::getHeight(AVLNode<Key, Value>* node) {
    if (node == nullptr) {
        return 0;
    }
    return node->height();
}

template <class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n) {
    AVLNode<Key, Value>* g = p->getParent();

    //root base conditions
    if (p == nullptr) {
        return;
    }
    if (g == nullptr) {
        return;
    }

    bool leftP = g->getLeft() != nullptr && g->getLeft()->getKey() == p->getKey();
    // if (g->getLeft() != nullptr)
    //     leftP = g->getLeft()->getKey() == p->getKey();


    if (leftP) {
        g->setBalance(g->getBalance() - 1);

        //case 1: We're done!
        if (g->getBalance() == 0) {
            return;
        }

        //case 2: recurse
        if (g->getBalance() == -1) {
            insertFix(g, p);
        }

        //case 3: swap and finalize
        if (g->getBalance() == -2) {
            finishLeftInsert(p, n, g);
        }
    }else {
        g->setBalance(g->getBalance() + 1);

        //case 1: We're done!
        if (g->getBalance() == 0) {
            return;
        }

        //case 2: recurse
        if (g->getBalance() == 1) {
            insertFix(g, p);
        }

        //case 3: swap and finalize
        if (g->getBalance() == 2) {
            finishRightInsert(p, n, g);
        }

    }

}


template <class Key, class Value>
void AVLTree<Key, Value>::finishLeftInsert(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n, AVLNode<Key, Value>* g) {
    bool zigzag = isZigZag(g, p, n);
    if (zigzag) {
        rotateLeft(p);
    }
    rotateRight(g);

    if (zigzag && n->getBalance() != 0) {
        if (n->getBalance() == -1) {
            p->setBalance(0);
            g->setBalance(1);
        }else if (n->getBalance() == 1) {
            p->setBalance(-1);
            g->setBalance(0);
        }
        n->setBalance(0);
    }else {
        p->setBalance(0);
        g->setBalance(0);
        if (zigzag)
            n->setBalance(0);
    }
}

template <class Key, class Value>
void AVLTree<Key, Value>::finishRightInsert(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n, AVLNode<Key, Value>* g) {
    bool zigzag = isZigZag(g, p, n);
    if (zigzag) {
        rotateRight(p);
    }
    rotateLeft(g);

    if (zigzag && n->getBalance() != 0) {
        if (n->getBalance() == 1) {
            p->setBalance(0);
            g->setBalance(-1);
        }else if (n->getBalance() == -1) {
            p->setBalance(1);
            g->setBalance(0);
        }
        n->setBalance(0);
    }else {
        p->setBalance(0);
        g->setBalance(0);
        if (zigzag)
            n->setBalance(0);
    }
}

/**
 * @pre g, p, and n are not null and:
 * @param g is the parent of p
 * @param p is the parent of n
 * @param n is the inserted value
 * @return true iff the g->p->n relation is zig zag, false otherwise
 */
template <class Key, class Value>
bool AVLTree<Key, Value>::isZigZag(AVLNode<Key, Value>* g, AVLNode<Key, Value>* p, AVLNode<Key, Value>* n) {
    bool gpLeft = /*g->getLeft() == nullptr ||*/ ( g->getLeft() != nullptr && g->getLeft()->getKey() == p->getKey());
    bool pnLeft = /*p->getLeft() == nullptr ||*/ ( p->getLeft() != nullptr && p->getLeft()->getKey() == n->getKey());

    return gpLeft != pnLeft;
}

template <class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* z) {

    if (z == nullptr) {
        return;
    }

    AVLNode<Key, Value>* zP = z->getParent();
    AVLNode<Key, Value>* y  = z->getLeft();
    if (y == nullptr) {
        return;
    }
    AVLNode<Key, Value>* yR = y->getRight();

    bool zPRoot = zP == nullptr;
    bool isLeft = /*(!zPRoot && zP->getLeft() == nullptr) || */(!zPRoot && zP->getLeft() != nullptr && zP->getLeft()->getKey() == z->getKey());

    if (!zPRoot && isLeft) {
        zP->setLeft(y);
    }else if (!zPRoot) {
        zP->setRight(y);
    }
    y->setParent(zP);
    if (zPRoot) {
        this->root_ = y;
    }

    y->setRight(z);
    z->setParent(y);
    z->setLeft(yR);
    if (yR != nullptr) {
        yR->setParent(z);
    }

    //update balance factors for y
    int yHeightLeft  = getHeight(y->getLeft());
    int yHeightRight = getHeight(y->getRight());
    int yBalance = yHeightRight - yHeightLeft;
    y->setBalance(yBalance);

    int zHeightLeft  = getHeight(z->getLeft());
    int zHeightRight = getHeight(z->getRight());
    int zBalance = zHeightRight - zHeightLeft;
    z->setBalance(zBalance);

}

template <class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* x) {

    if (x == nullptr) {
        return;
    }

    AVLNode<Key, Value>* xP = x->getParent();
    AVLNode<Key, Value>* y  = x->getRight();
    AVLNode<Key, Value>* yL = y->getLeft();

    bool xPRoot = xP == nullptr;
    bool isLeft = /*(!xPRoot && xP->getLeft() == nullptr) || */(!xPRoot && xP->getLeft() != nullptr && xP->getLeft()->getKey() == x->getKey());

    if (!xPRoot && isLeft) {
        xP->setLeft(y);
    }else if (!xPRoot) {
        xP->setRight(y);
    }
    y->setParent(xP);
    if (xPRoot) {
        this->root_ = y;
    }

    y->setLeft(x);
    x->setParent(y);
    x->setRight(yL);
    if (yL != nullptr) {
        yL->setParent(x);
    }

    //update balance factors for y
    int yHeightLeft  = getHeight(y->getLeft());
    int yHeightRight = getHeight(y->getRight());
    int yBalance = yHeightRight - yHeightLeft;
    y->setBalance(yBalance);

    int xHeightLeft  = getHeight(x->getLeft());
    int xHeightRight = getHeight(x->getRight());
    int xBalance = xHeightRight - xHeightLeft;
    x->setBalance(xBalance);

}


#endif
