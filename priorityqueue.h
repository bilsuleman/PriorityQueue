// priorityqueue.h
/*
  Project 5 Starter Code - Priority Queue using custom BST
  Bilal Suleman
  Written in C++
  
  Description:
  This is the class implementation for a priority queue using a custom Binary
  Search Tree modified to handle duplicate values with a linked list chaining
  the duplicate values. It is a complete and templated class allowing the use of
  any data type to be stored in the priority queue and allows for traversal of
  the queue mimicking C++ iterators using begin() and next(). It also allows
  enqueueing and dequeueing values as well as peeking for the next value.
  
*/
#pragma once

#include <iostream>
#include <sstream>
#include <set>

using namespace std;

template<typename T>
class priorityqueue {
private:
    struct NODE {
        int priority = 0;  // used to build BST
        T value = T();  // stored data for the p-queue
        bool dup = false;  // marked true when there are duplicate priorities
        NODE* parent;  // links back to parent
        NODE* link;  // links to linked list of NODES with duplicate priorities
        NODE* left;  // links to left child
        NODE* right;  // links to right child
    };
    NODE* root;  // pointer to root node of the BST
    int size;  // # of elements in the pqueue
    NODE* curr;  // pointer to next item in pqueue (see begin and next)
    
    // Helper function to build a string representaion of priority queue
    // recursively. Passes the string into an output stream;
    void buildString(NODE* node, ostream& output) {
    	if (node == nullptr) {
    		return;
    	}
    	buildString(node->left, output);
    	output << node->priority << " value: " << node->value << "\n";
    	if (node->dup == true) {
    		buildString(node->link, output);
    	}
    	buildString(node->right, output);
    }
    
    // Helper function to delte a entire priority queue recursively. Is
    // called by both the destructor and clear functions. Utilizes post order
    // traversal to delete nodes with a special exeception if a duplicate is
    // encountered then the linked list of that node is traversed and deleted
    // recursively.
    void deleteTree(NODE* n) {
    	if (n == nullptr) {
    		return;
    	}
    	
    	deleteTree(n->left);
    	deleteTree(n->right);
    	if (n->dup == true) {
    		deleteTree(n->link);
    	}
    	delete n;
    	n = nullptr;
    	size--;
    }
    
    // Helper function used by the equals or assignment operator to copy
    // the contents of one queue into another empty queue. The function
    // recursively copies the contents of one queue into another by utilizing
    // enqueue to add a node with the same value and priority as the other
    // queue into a new queue creating a deep copy.
    void copyTree(NODE* root) {
    	if (root == nullptr) {
    		return;
    	} else {
	    	enqueue(root->value, root->priority);
	    	copyTree(root->left);
	    	copyTree(root->right);
			copyTree(root->link);
    	} 
    }
    
    // Helper function to remove a node from the queue, called by dequeue.
    // Uses post order traversal to remove traditional nodes with the exeception
    // of the duplicate nodes being checked for and traversed first in order to
    // re link the childern of the linked node to the parents of the duplicate
    // and preserve the tree structure. Essentially the duplicate is replaced
    // by the next node in its linked list.
    void deleteNode(NODE* n) {
    	if (n->dup && n->link) {
    		deleteDupNode(n);
    	} else if (!(n->left) && !(n->right)) {
    		deleteLeafNode(n);
    	} else {
    		deleteOneChild(n);
    	}
    	delete n;
    	n = nullptr;
    }
    
    // Helper function for the delete node function to deal with removal of
    // duplicate nodes. Implements the logic described in the comment above.
    void deleteDupNode(NODE *&n) {
    	n->link->right = n->right;
		n->link->left = n->left;
		if (n == root) {
			root = n->link;
		} else if (n->parent->left == n) {
			n->parent->left = n->link;
			n->link->parent = n->parent;
		} else if (n->parent->right == n) {
			n->parent->right = n->link;
			n->link->parent = n->link;
		}
		n->link = nullptr;
    }
    
    // Helper function to delete a leaf node from a tree. Simpley unlnks the
    // node from the tree and frees the memory associated with it.
    void deleteLeafNode(NODE *&n) {
    	if (n != root) {
			if (n->parent->left == n) {
    			n->parent->left = nullptr;
    		} else {
    			n->parent->right = nullptr;
    		}
    	} else {
    			root = nullptr;
    	}
    }
    
    // Helper function to deal with the deletion of a node with a single child.
    // Due to the nature of our priority queue there is never a situation where
    // a node with multiple childern needs to be deleted and therefore this is
    // the last case to consider when deleting a node. This is achieved by
    // determining which subtree contains the child for n and then linking that
    // node to the parent of n and linking parent to n's child. Node n can then
    // be removed from the queue with no consequence.
    void deleteOneChild(NODE *&n) {
    	NODE* child = (n->left) ? n->left: n->right;
		if (n != root) {
    		if (n->parent->left == n) {
    			n->parent->left = child;
    			child->parent = n->parent;
    		} else {
    			n->parent->right = child;
    			child = n->parent;
    		}
		} else {
			root = child;
		}
    }
    
    // Helper function to determine if two trees are equivalent. This function
    // recursively compares the value and priority of two nodes and then checks
    // their subtrees for the same. If all nodes are equivalent then both trees
    // must be eequivalent.
	bool compareNodes(NODE* x, NODE* y) const {
    	if (x == nullptr && y == nullptr) {
    		return true;
    	}
    	if (x != nullptr && y != nullptr) {
    		return (x->priority == y->priority &&
    				x->value == y->value &&
    				compareNodes(x->right, y->right) &&
    				compareNodes(x->left, y->left) &&
    				compareNodes(x->link, y->link));
    	}
    	return false;
    }
    
    // Helper function uesed by begin to determine the leftmost node in the
    // queue which is also the minimum node in the queue. Traverses list until
    // no more left childern for n can be found. This function was implemented
    // iteratively.
    NODE * minNode(NODE *n) {
    	n = root;
    	if (n == nullptr) {
    		return n;
    	}
    	while (n->left) {
    		if (n->left) {
    			n = n->left;
    		}
    	}
    	return n;
    }
    
    // Helper function to deal with the enqueueing of a duplicate node. This
    // function is called when a node with the same priority as another needs to
    // be inserted into the queue. A new node is constructed and inserted into
    // the first duplicate node's linked list.
    bool queueDup(T value, int priority, NODE *&n, NODE *&prev) {
    	do {
			if (n->link == nullptr) {
				NODE* dup = new NODE();
    			dup->dup = true;
    			dup->parent = n;
		    	dup->value = value;
		    	dup->priority = priority;
		    	dup->left = nullptr;
		    	dup->right = nullptr;
		    	n->link = dup;
		    	n->dup = true;
		    	size++;
		    	return true;
			} else {
				n = n->link;	
			}
		} while (n->link != nullptr);
		return false;
    }
    
    // Helper function to construct a new node and add to the priority queue.
    // This function is used by the enqueue function to construct and add a 
    // new node into the priority queue at the appriopriate location.
    void addNewNode(T value, int priority, NODE *&n, NODE *&prev) {
    	NODE* node = new NODE();
    	node->parent = prev;
    	node->value = value;
    	node->priority = priority;
    	node->left = nullptr;
    	node->right = nullptr;
    	
    	if (prev == nullptr) {
    		root = node;
    	} else if (priority < prev->priority) {
    		prev->left = node;
    	} else {
    		prev->right = node;
    	}
    }
    
    // Helper function to move curr pointer when encountering the end of the
    // duplicate linked list. Walks curr back to the head of the linked list.
    void traverseEndOfList(T& value, int& priority) {
	 	if (curr->priority == curr->parent->priority && 
   				curr->link == nullptr) {
    		while (curr->priority == curr->parent->priority) {
    			curr = curr->parent;
	   			if (curr->parent == nullptr) {
	   				break;
	   			}
	   		}
    	}
    }
    
    // Helper function to advance curr under normal circumstances. Walks to the
    // next in order node in the list iteratively.
    bool moveCurr(T& value, int& priority) {
    	if (curr->right != nullptr) {
    		curr = curr->right;
    		while (curr->left != nullptr) {
    			curr = curr->left;
    		}
    		return true;
    	} else if (curr->right == nullptr) {
    		while (curr->parent != nullptr) {
    			if (curr->parent->left == curr) {
    				curr = curr->parent;
		    		return true;
    			}
    			curr = curr->parent;
    		}
    		
    	}
    	return false;
    }
    
public:
    //
    // default constructor:
    //
    // Creates an empty priority queue.
    // O(1)
    //
    priorityqueue() {
    	root = nullptr;
    	curr = root;
    	size = 0;
    }
    
    //
    // operator=
    //
    // Clears "this" tree and then makes a copy of the "other" tree.
    // Sets all member variables appropriately.
    // O(n), where n is total number of nodes in custom BST
    //
    priorityqueue& operator=(const priorityqueue& other) {
        this->clear();
        copyTree(other.root);
        return *this;
    }
    
    //
    // clear:
    //
    // Frees the memory associated with the priority queue but is public.
    // O(n), where n is total number of nodes in custom BST
    //
    void clear() {
    	deleteTree(root);
    	root = nullptr;
    	curr = root;
    }
    
    //
    // destructor:
    //
    // Frees the memory associated with the priority queue.
    // O(n), where n is total number of nodes in custom BST
    //
    ~priorityqueue() {
    	deleteTree(root);
    	root = nullptr;
    	curr = root;
    }
    
    //
    // enqueue:
    //
    // Inserts the value into the custom BST in the correct location based on
    // priority.
    // O(logn + m), where n is number of unique nodes in tree and m is number of
    // duplicate priorities
    //
    void enqueue(T value, int priority) {
    	NODE* prev = nullptr;
    	NODE* n = root;
    	
    	while (n != nullptr) {
    		if (priority == n->priority) {
    			if (queueDup(value, priority, n, prev)) {
    				return;
    			}
    		}
    		if (priority < n->priority) {
    			prev = n;
    			n = n->left;
    		} else if (priority > n->priority) {
    			prev = n;
    			n = n->right;
    		}
    	}
    	addNewNode(value, priority, n, prev);
    	
    	size++;
    }
    //
    // dequeue:
    //
    // returns the value of the next element in the priority queue and removes
    // the element from the priority queue.
    // O(logn + m), where n is number of unique nodes in tree and m is number of
    // duplicate priorities
    //
    T dequeue() {
    	if (size <= 0) {
    		return T();
    	}
    	T value;
    	NODE *n;
    	n = minNode(n);
    	if (size > 0) {
    		value = n->value;
    		deleteNode(n);
    		size--;
    	}
    	return value;
    }
    
    //
    // Size:
    //
    // Returns the # of elements in the priority queue, 0 if empty.
    // O(1)
    //
    int Size() {
        return size;
    }
    
    //
    // begin
    //
    // Resets internal state for an inorder traversal.  After the
    // call to begin(), the internal state denotes the first inorder
    // node; this ensure that first call to next() function returns
    // the first inorder node value.
    //
    // O(logn), where n is number of unique nodes in tree
    //
    // Example usage:
    //    pq.begin();
    //    while (tree.next(value, priority)) {
    //      cout << priority << " value: " << value << endl;
    //    }
    //    cout << priority << " value: " << value << endl;
    void begin() {
    	if (size <= 0) {
    		curr = nullptr;
    		return;
    	}
    	curr = minNode(curr);
    }
    
    //
    // next
    //
    // Uses the internal state to return the next inorder priority, and
    // then advances the internal state in anticipation of future
    // calls.  If a value/priority are in fact returned (via the reference
    // parameter), true is also returned.
    //
    // False is returned when the internal state has reached null,
    // meaning no more values/priorities are available.  This is the end of the
    // inorder traversal.
    //
    // O(logn), where n is the number of unique nodes in tree
    //
    // Example usage:
    //    pq.begin();
    //    while (tree.next(value, priority)) {
    //      cout << priority << " value: " << value << endl;
    //    }
    //    cout << priority << " value: " << value << endl;
    //
    bool next(T& value, int &priority) {
    	value = curr->value;
    	priority = curr->priority;
    	
    	if (curr->link != nullptr && curr->dup == true) {
    		curr = curr->link;
    		return true;
    	}
    	
    	if (curr != root) {
    		traverseEndOfList(value, priority);	 
    	}
    	
    	if (moveCurr(value, priority)) {
    		return true;
    	}
    	
    	return false;
    }
    
    //
    // toString:
    //
    // Returns a string of the entire priority queue, in order.  Format:
    // "1 value: Ben
    //  2 value: Jen
    //  2 value: Sven
    //  3 value: Gwen"
    //
    string toString() {
        stringstream ss;
        buildString(root, ss);
        string output = ss.str();
        return output;
    }
    
    //
    // peek:
    //
    // returns the value of the next element in the priority queue but does not
    // remove the item from the priority queue.
    // O(logn + m), where n is number of unique nodes in tree and m is number of
    // duplicate priorities
    //
    T peek() {
    	if (size <= 0) {
    		return T();
    	}
    	T value;
    	NODE *n;
    	n = minNode(n);
    	if (size > 0) {
    		value = n->value;
    	}
    	return value;
    }
    
    //
    // ==operator
    //
    // Returns true if this priority queue as the priority queue passed in as
    // other.  Otherwise returns false.
    // O(n), where n is total number of nodes in custom BST
    //
    bool operator==(const priorityqueue& other) const {
    	return compareNodes(root, other.root) &&
    			size == other.size;
    }
    
    //
    // getRoot - Do not edit/change!
    //
    // Used for testing the BST.
    // return the root node for testing.
    //
    void* getRoot() {
        return root;
    }
};
