#include <iostream>

// #include "sanitized_checks.h"

#include <random>

#include "bst.h"
#include "avlbst.h"

#include <set>

template<typename Key, typename Value>
bool verifyBST(BinarySearchTree<Key, Value> & tree, std::set<Key> const & keySet)
{
	bool validResult = isValidTree(tree);

	if(!validResult)
	{
		std::cout << ">> Binary tree structure is broken!" << std::endl;
		return validResult;
	}

	// bool keysResult = checkKeys(tree, keySet);
	//
	// if(!keysResult)
	// {
	// 	std::cout << ">> Key mismatch error!" << std::endl;
	// 	std::cout << ">> Tree was:" << std::endl;
	// 	tree.print();
	// 	return keysResult;
	// }

	return true;
}

// similar to above, but only checks validity, not keys.
template<typename Key, typename Value>
bool verifyBST(BinarySearchTree<Key, Value> & tree)
{
	bool validResult = isValidTree(tree);

	if(!validResult)
	{
		std::cout << ">> Binary tree structure is broken!" << std::endl;
		std::cout << ">> Tree was:" << std::endl;
		tree.print();
		return validResult;
	}

	return true;

}


template<typename Key, typename Value>
bool isValidTree(BinarySearchTree<Key, Value> & tree)
{
	// empty trees are always valid
	if(tree.root() == nullptr)
	{
		return true;
	}
	else
	{
		return checkValidTraversal(tree.root());
	}
}

// author credit: Chris Hailey
template<typename Key, typename Value>
bool checkValidTraversal(Node<Key, Value>* current)
{
		if(current->getLeft()!=nullptr){
			if(!(current->getLeft()->getKey() < current->getKey())){
				std::cout  << "The left child of "<<current->getKey()<<" is "<<current->getLeft()->getKey()<<" which is larger than its parent and not allowed for a left child in a BST" << std::endl;
				return false;
			}
			if(current->getLeft()->getParent()!=current){
				std::cout  <<"The left child of "<<current->getKey()<<" does not have its parent set correctly" << std::endl;
				return false;
			}
			Node<Key, Value>* predecessor = current->getLeft();
			while(predecessor->getRight()!=nullptr){
				predecessor = predecessor->getRight();
			}
			if(!(predecessor->getKey()< current->getKey())){
				std::cout << "The predecessor of "<<current->getKey()<<" is "<< predecessor->getKey()<<" which is larger than what it is predecessing" << std::endl;
				return false;
			}

			bool traversalResult = checkValidTraversal(current->getLeft());

			if(!traversalResult){
				return traversalResult;
			}
		}
		if(current->getRight()!=nullptr)
		{
			if(!(current->getKey() < current->getRight()->getKey()))
			{
                          std::cout << "The right child of "<<current->getKey()<<" is "<<current->getRight()->getKey()<<" which is less than its parent and not allowed for a right child" << std::endl;
				return false;
			}
			if(current->getRight()->getParent()!=current)
			{
                          std::cout << "the right child of " << current->getKey() << " does not have its parent set correctly" << std::endl;
				return false;
			}

			bool traversalResult = checkValidTraversal(current->getRight());

			if(!traversalResult)
			{
				return traversalResult;
			}
		}

		return true;
}

template<typename Key, typename Value>
std::pair<int, bool> verifyAVLBalanceRecursive(AVLTree<Key, Value> & tree, Node<Key, Value>* currNode)
{
	if (currNode == nullptr)
	{
		return std::make_pair(0, true);
	}

	std::pair<int, bool> balanceResultsLeft = verifyAVLBalanceRecursive(tree, currNode->getLeft());
	if(!balanceResultsLeft.second)
	{
		return std::make_pair(0, balanceResultsLeft.second);
	}

	std::pair<int, bool> balanceResultsRight = verifyAVLBalanceRecursive(tree, currNode->getRight());
	if(!balanceResultsRight.second)
	{
		return std::make_pair(0, balanceResultsRight.second);
	}

	if(std::abs(balanceResultsLeft.first - balanceResultsRight.first) < 2)
	{
		// this subtree is in balance

		int thisNodeHeight = std::max(balanceResultsLeft.first, balanceResultsRight.first) + 1;

		return std::make_pair(thisNodeHeight, true);
	}
	else
	{
		std::cout << "AVL balance error: subtree rooted at " << currNode->getKey() << " is out of balance!  Left child has height "
				   << balanceResultsLeft.first << ", and right child has height " << balanceResultsRight.first << "." << std::endl;
		// this subtree is out of balance
		return std::make_pair(0, false);

	}
}

template<typename Key, typename Value>
bool verifyAVL(AVLTree<Key, Value> & tree, std::set<Key> & keySet)
{
	// first verify it as a BST
	bool bstResult = verifyBST(tree, keySet);

	if(!bstResult)
	{
		return bstResult;
	}

	bool balanceResult = checkAVLBalance(tree);
	if(!balanceResult)
	{
		std::cout << "Balance error!" << std::endl;
		std::cout << "Tree was: " << std::endl;
		tree.print();
	}

	return balanceResult;
}

template<typename Key, typename Value>
bool checkAVLBalance(AVLTree<Key, Value> & tree)
{
	return verifyAVLBalanceRecursive(tree, tree.root()).second;
}






using namespace std;




int main(int argc, char *argv[]) {
	// Binary Search Tree tests
	BinarySearchTree<char,int> bt;
	bt.insert(std::make_pair('a',1));
	bt.insert(std::make_pair('b',2));
	bt.insert(std::make_pair('v', 3));
	bt.insert(std::make_pair('z', 4));
	bt.insert(std::make_pair('c', 5));
	
	cout << "Binary Search Tree contents:" << endl;
	for(BinarySearchTree<char,int>::iterator it = bt.begin(); it != bt.end(); ++it) {
		cout << it->first << " " << it->second << endl;
	}
	if(bt.find('b') != bt.end()) {
		cout << "Found b" << endl;
	}
	else {
		cout << "Did not find b" << endl;
	}
	cout << "Erasing b" << endl;
	bt.remove('b');
	
	cout << "Binary Search Tree contents:" << endl;
	for(const auto & it : bt) {
		cout << it.first << " " << it.second << endl;
	}
	
	BinarySearchTree<std::string, std::string> testTree;
	
	// AVL Tree Tests
	AVLTree<char,int> at;
	at.insert(std::make_pair('a',1));
	at.insert(std::make_pair('b',2));
	at.insert(std::make_pair('v', 3));
	at.insert(std::make_pair('z', 4));
	at.insert(std::make_pair('c', 5));
	
	cout << "\nAVLTree contents:" << endl;
	for(AVLTree<char,int>::iterator it = at.begin(); it != at.end(); ++it) {
		cout << it->first << " " << it->second << endl;
	}
	if(at.find('b') != at.end()) {
		cout << "Found b" << endl;
	}
	else {
		cout << "Did not find b" << endl;
	}
	at.print();
	at.insert(std::make_pair('z'+1, 6));
	at.print();

	// AVLTree<int, int> biggerTest{};

	// std::mt19937 randEngine{random_device()()};

	// uniform_int_distribution<int> u = uniform_int_distribution<>(-500, 500);

	// int erroring_inputs_0[] = {586566661, 49403963, 326459042, 2055484338, 1860781471, 121131049, 415238228, 1936441487, 2004640315, 1458405564, 1122247569,
	// 						1424242243, 1286670669, 1950741489};

	// int erroring_inputs_1[] = {95, -27, -52, -18, -24, 109, -58, -49, 127, 9, -84, 141, -38, -12, 48, -89, 61, 87, 11, -35, -87, 130, 72, 76, -113, 110, -54, 63, -126, -125};

	// int erroring_inputs_2[] = {132, 109, -130, -42, -98, 52, -58, 50, -73, -118, -125, 91, 66, -144, 37, -133, 106, -18, -16, 112, 26, -129, 110, 42, 62, 88, -83, 63, -145, -43};

	// int erroring_inputs[] = {-93, 27, 1, -109, 62, 37, -14, 135, -47, -74, 133, -115, 78, -78, 147, -5, 72, -94, 10, 28, 68, -64, 73, -62, 66, 65, -40, 7, -102, 55};



  //   std::set<int> aaa{};

	// const int interesting_value = -102;

	// const int inspect_vals[] = {-115, -94, -64};

  //   for (const int i : erroring_inputs) {

  //       // int insert = u(randEngine);
  //   	// int insert = erroring_inputs[i];
  //   	if (i == interesting_value) {
  //   		biggerTest.print();
  //   	}
  //   	int insert = i;
  //       std::cout << "inserting: " << insert << " at " << insert << std::endl;
  //       biggerTest.insert(insert, insert);

  //       aaa.insert(insert);

  //   	for (const int& c : inspect_vals) {
  //   		if (i == c) {
  //   			biggerTest.print();
  //   		}
  //   	}

  //       if (!verifyAVL<int, int>(biggerTest, aaa)) {
  //       	AVLNode<int, int>* nn3 = dynamic_cast<AVLNode<int, int>*>(biggerTest.find(-93).getCurrent());
  //       	std::cout << nn3->height() << std::endl;
  //       	std::cout << biggerTest.calcBalance(-93) << std::endl;
  //           break;
  //       }
  //   }


    // biggerTest.print();





    cout << "Erasing b" << endl;
    at.remove('b');

    return 0;
}
/*
			  04
		02			05
	01		03			06
	 -118



 */