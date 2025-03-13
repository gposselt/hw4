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
    void insert (const std::pair<const Key, Value> &new_item) override;
    void remove(const Key& key) override;  // TODO

    virtual void insert(const Key& k, const Value& v);

    void print_placeholders(std::ios::fmtflags origCoutState, std::map<Key, uint8_t> valuePlaceholders) const override;

    //todo: remove public helper
    int calcBalance(const Key& node);

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


    void removeFix(AVLNode<Key, Value>* n, uint8_t diff);

    static AVLNode<Key, Value>* nodeMin(AVLNode<Key, Value>* current);
    static AVLNode<Key, Value>* nodeMax(AVLNode<Key, Value>* current);
    static AVLNode<Key, Value>* predecessor(AVLNode<Key, Value>* current);


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
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
        }else if (val->getKey() > current->getKey()){
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

    int8_t opb = p->getBalance();


    if (opb == 0) {
        int leftHeight  = getHeight(p->getLeft());
        int rightHeight = getHeight(p->getRight());

        int newBalance = rightHeight - leftHeight;

        p->setBalance(newBalance);

        // Key* a = &(new_item.first);

        // std::cout << val->getKey() << ": " << (int)val->getBalance() << "/0" << std::endl;
        // std::cout << p->getKey() << ": " << (int)p->getBalance() << "/" << newBalance << std::endl;
        insertFix(p, val/*, dynamic_cast<int*>(&(new_item.first)) && new_item.first == -109*/);
        return;
    }else {
        p->setBalance(0);
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

    //alright let's do this >:(

    AVLNode<Key, Value>* n = dynamic_cast<AVLNode<Key, Value>*>(this->find(key).current_);

    if (n == nullptr) {
        return;
    }

    if (n == this->root_ && this->root_->isLeaf()) {
        this->clear();
        return;
    }

    bool leftNull = n->getLeft() == nullptr;
    bool rightNull = n->getRight() == nullptr;

    bool leftChild = n->getParent() != nullptr &&
                    n->getParent()->getLeft() != nullptr &&
                    n->getParent()->getLeft()->getKey() == key;

    if (leftNull) {
        this->promoteSingleSubtree( n, n->getRight());
    }else if (rightNull) {
        this->promoteSingleSubtree(n, n->getLeft());
    }else {
        AVLNode<Key, Value>* pred = predecessor(n);
        nodeSwap(n, pred);

        if (n->getLeft() != nullptr) {
            this->promoteSingleSubtree(n, n->getLeft());
        }else if (n->getRight() != nullptr) {
            this->promoteSingleSubtree(n, n->getRight());
        }else {
            AVLNode<Key, Value>* parent = n->getParent();

            //remove references to deleted memory
            if (parent != nullptr && parent->getRight() != nullptr && parent->getRight()->getKey() == key) {
                parent->setRight(nullptr);
            }else if (parent != nullptr && parent->getLeft() != nullptr && parent->getLeft()->getKey() == key) {
                parent->setLeft(nullptr);
            }
        }
    }

    AVLNode<Key, Value>* parent = n->getParent();
    AVLNode<Key, Value>* &p = parent;

    if (p == nullptr) {
        delete n;
    }else {

        int8_t diff = 0;

        if (leftChild) {
            diff = 1;
        }else {
            diff = -1;
        }

        delete n;

        removeFix(p, diff);
    }


}

template <class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* n, uint8_t diff) {
    if (n == nullptr) {
        return;
    }

    int8_t ndiff = 0;

    AVLNode<Key, Value>* p = n->getParent();

    if (p != nullptr) {
        bool leftChild = p->getLeft() != nullptr && p->getLeft()->getKey() == n->getKey();
        if (leftChild) {
            diff = 1;
        }else {
            diff = -1;
        }
    }

    int nLeftHeight  =  getHeight(n->getLeft());
    int nRightHeight = getHeight(n->getRight());
    int nBalance = nRightHeight - nLeftHeight;

    n->setBalance(nBalance);

    if (nBalance == -2) {
        AVLNode<Key, Value>* c = n->getLeft();
        int cLeftHeight  =  getHeight(c->getLeft());
        int cRightHeight = getHeight(c->getRight());
        int cbal = cRightHeight - cLeftHeight;

        if (cbal == -1) {
            rotateRight(n);
            c->setBalance(0);
            n->setBalance(0);
            removeFix(p, ndiff);
        }else if (cbal == 0) {
            rotateRight(n);
            n->setBalance(-1);
            c->setBalance(1);
            //done!
        }else if (cbal == 1) {
            AVLNode<Key, Value>* g = c->getRight();
            rotateLeft(c);
            rotateRight(n);

            if (g->getBalance() == +1) {
                n->setBalance(0); c->setBalance(-1); g->setBalance(0);
            }else if (g->getBalance() == +0) {
                n->setBalance(0); c->setBalance(0); g->setBalance(+0);
            } else if (g->getBalance() == -1) {
                n->setBalance(+1); c->setBalance(0); g->setBalance(0);
            }

            removeFix(p, ndiff);
        }
    }if (nBalance == 2) {

        AVLNode<Key, Value>* c = n->getRight();
        int cLeftHeight  =  getHeight(c->getLeft());
        int cRightHeight = getHeight(c->getRight());
        int cbal = cRightHeight - cLeftHeight;

        if (cbal == 1) {
            rotateLeft(n);
            c->setBalance(0);
            n->setBalance(0);
            removeFix(p, ndiff);
        }else if (cbal == 0) {
            rotateLeft(n);
            n->setBalance(1);
            c->setBalance(-1);
            //done!
        }else if (cbal == -1) {
            AVLNode<Key, Value>* g = c->getLeft();
            rotateRight(c);
            rotateLeft(n);

            if (g->getBalance() == -1) {
                n->setBalance(0); c->setBalance(+1); g->setBalance(0);
            }else if (g->getBalance() == +0) {
                n->setBalance(0); c->setBalance(0); g->setBalance(+0);
            } else if (g->getBalance() == 1) {
                n->setBalance(-1); c->setBalance(0); g->setBalance(0);
            }

            removeFix(p, ndiff);
        }

    }else if (nBalance == 0) {
        removeFix(p, ndiff);
    }
}

template <typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::nodeMin(AVLNode<Key, Value>* current) {
    AVLNode<Key, Value>* temp = current;
    while (temp->getLeft() != nullptr) {
        temp = temp->getLeft();
    }
    return temp;
}

template <typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::nodeMax(AVLNode<Key, Value>* current) {
    AVLNode<Key, Value>* temp = current;
    while (temp->getRight() != nullptr) {
        temp = temp->getRight();
    }
    return temp;
}

template <class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::predecessor(AVLNode<Key, Value>* current) {
    //if we have a left child, it's the max in that subtree
    if (current->getLeft() != nullptr) {
        return nodeMax(current->getLeft());
    }
    //if not, walk up
    AVLNode<Key, Value>* node = current->getParent();
    //keep walking up until either the parent is null or we are not the left child
    while (node != nullptr && current == node->getLeft()) {
        current = node;
        node = node->getParent();
    }

    return node;

}

template <class Key, class Value>
void AVLTree<Key, Value>::insert(const Key& k, const Value& v) {
    this->insert(std::make_pair(k, v));
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

    //recalculate g balance because I don't trust my own code to get it right
    int glh = getHeight(g->getLeft());
    int grh = getHeight(g->getRight());
    int gbal = grh - glh;

    if (leftP) {
        // g->updateBalance(-1);
        g->setBalance(gbal);

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
        // g->updateBalance(1);
        g->setBalance(gbal);

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
        rotateRight(g);
    }else {
        rotateRight(g);
        p->setBalance(0);
        g->setBalance(0);
        // n->setBalance(0);

        return;
    }


    if (n->getBalance() == -1) {
        p->setBalance(0);
        g->setBalance(1);
        n->setBalance(0);
    }else if (n->getBalance() == 0) {
        p->setBalance(0);
        g->setBalance(0);
        n->setBalance(0);
    }else if (n->getBalance() == 1) {
        p->setBalance(-1);
        g->setBalance(0);
        n->setBalance(0);
    }

}

template <class Key, class Value>
void AVLTree<Key, Value>::finishRightInsert(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n, AVLNode<Key, Value>* g) {
    bool zigzag = isZigZag(g, p, n);
    if (zigzag) {
        rotateRight(p);
        rotateLeft(g);
    }else {
        rotateLeft(g);
        p->setBalance(0);
        g->setBalance(0);
        // n->setBalance(0);
        return;
    }


    if (n->getBalance() == 1) {
        p->setBalance(0);
        g->setBalance(-1);
        n->setBalance(0);
    }else if (n->getBalance() == 0) {
        p->setBalance(0);
        g->setBalance(0);
        n->setBalance(0);
    }else if (n->getBalance() == -1) {
        p->setBalance(1);
        g->setBalance(0);
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
    int pbal = p->getBalance();

    return (pbal < 0 && g->getRight() != nullptr && g->getRight()->getKey() == p->getKey()) ||
            (pbal > 0 && g->getLeft() != nullptr && g->getLeft()->getKey() == p->getKey());
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



template <class Key, class Value>
void AVLTree<Key, Value>::print_placeholders(std::ios::fmtflags origCoutState,
    std::map<Key, uint8_t> valuePlaceholders) const {
    std::cout << "Tree Placeholders:------------------" << std::endl;
    for(typename std::map<Key, uint8_t>::iterator placeholdersIter = valuePlaceholders.begin(); placeholdersIter != valuePlaceholders.end(); ++placeholdersIter) {
        std::cout << '[' << std::setfill('0') << std::setw(2) << ((uint16_t)placeholdersIter->second) << "] -> ";

        // print element with original cout flags
        std::cout.flags(origCoutState);
        std::cout << '(' << placeholdersIter->first << ", ";

        typename BinarySearchTree<Key, Value>::iterator elementIter = this->find(placeholdersIter->first);
        if(elementIter == this->end())
        {
            std::cout << "<error: lookup failed>";
        }
        else
        {
            std::cout << elementIter->second;
        }

        std::cout << ')';

        if (elementIter != this->end()) {
            Node<Key, Value>* cur = elementIter.getCurrent();
            std::cout << " " << (int)dynamic_cast<AVLNode<Key, Value>*>(cur)->getBalance();
        }


        std::cout << std::endl;


    }
}

template <class Key, class Value>
int AVLTree<Key, Value>::calcBalance(const Key& node) {
    typename BinarySearchTree<Key, Value>::iterator h = this->find(node);

    if (h == this->end()) {
        return -1;
    }

    int hl = getHeight(dynamic_cast<AVLNode<Key, Value>*>(h.getCurrent()->getLeft()));
    int hr = getHeight(dynamic_cast<AVLNode<Key, Value>*>(h.getCurrent()->getRight()));
    int bal = hl - hr;
    return bal;

}

#endif
