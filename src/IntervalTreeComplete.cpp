// C++ Program to Implement Red Black Tree

#include <iostream>
#include <climits>
using namespace std;

// Enumeration for colors of nodes (Red-Black Tree property)
enum Color { RED, BLACK };

class RedBlackIntervalTree {
private:
    // Structure for the node
    struct Node {
        
        int low;
        int high;
        int max;
        
        
        Color color;
        Node* parent;
        Node* left;
        Node* right;

        // Constructor to initialize node
        Node(int l, int h) {
            low = l;
            high = h;
            max = h;
            color = RED;
            parent = nullptr;
            left = nullptr;
            right = nullptr;
        }
    };

    //Root node
    Node* root; 
    
    
    //Update the max of a node based on its left and right children
    void updateMax(Node* node) {
    if (node == nullptr)
        return;

    int leftMax = INT_MIN;
    int rightMax = INT_MIN;

    if (node->left != nullptr)
        leftMax = node->left->max;

    if (node->right != nullptr)
        rightMax = node->right->max;

    node->max = node->high;

    if (leftMax > node->max)
        node->max = leftMax;

    if (rightMax > node->max)
        node->max = rightMax;
}

    

    //Left Rotation (used when we fix violations)
    void rotateLeft(Node*& node)
    {
        Node* child = node->right;
        node->right = child->left;
        if (node->right != nullptr)
            node->right->parent = node;
        child->parent = node->parent;
        if (node->parent == nullptr)
            root = child;
        else if (node == node->parent->left)
            node->parent->left = child;
        else
            node->parent->right = child;
        child->left = node;
        node->parent = child;
        
    updateMax(node);
    updateMax(child);
    }

    //Right Rotation (used when we fix violations)
    void rotateRight(Node*& node)
    {
        Node* child = node->left;
        node->left = child->right;
        if (node->left != nullptr)
            node->left->parent = node;
        child->parent = node->parent;
        if (node->parent == nullptr)
            root = child;
        else if (node == node->parent->left)
            node->parent->left = child;
        else
            node->parent->right = child;
        child->right = node;
        node->parent = child;
        
    updateMax(node);
    updateMax(child);
    }

    //Function that will fix violations that appear when we insert a node
    void fixInsert(Node*& node)
    {
        Node* parent = nullptr;
        Node* grandparent = nullptr;
        while (node != root && node->color == RED
               && node->parent->color == RED) {
            parent = node->parent;
            grandparent = parent->parent;
            if (parent == grandparent->left) {
                Node* uncle = grandparent->right;
                if (uncle != nullptr
                    && uncle->color == RED) {
                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = grandparent;
                }
                else {
                    if (node == parent->right) {
                        rotateLeft(parent);
                        node = parent;
                        parent = node->parent;
                    }
                    rotateRight(grandparent);
                    swap(parent->color, grandparent->color);
                    node = parent;
                }
            }
            else {
                Node* uncle = grandparent->left;
                if (uncle != nullptr
                    && uncle->color == RED) {
                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = grandparent;
                }
                else {
                    if (node == parent->left) {
                        rotateRight(parent);
                        node = parent;
                        parent = node->parent;
                    }
                    rotateLeft(grandparent);
                    swap(parent->color, grandparent->color);
                    node = parent;
                }
            }
        }
        root->color = BLACK;
    }

    // Function that will fix violations that appear when we delete a node
    void fixDelete(Node*& node)
    {
        
        if (node == nullptr)
            return;
            
        while (node != root && node->color == BLACK) {
            if (node == node->parent->left) {
                Node* sibling = node->parent->right;
                if (sibling->color == RED) {
                    sibling->color = BLACK;
                    node->parent->color = RED;
                    rotateLeft(node->parent);
                    sibling = node->parent->right;
                }
                if ((sibling->left == nullptr
                     || sibling->left->color == BLACK)
                    && (sibling->right == nullptr
                        || sibling->right->color
                               == BLACK)) {
                    sibling->color = RED;
                    node = node->parent;
                }
                else {
                    if (sibling->right == nullptr
                        || sibling->right->color == BLACK) {
                        if (sibling->left != nullptr)
                            sibling->left->color = BLACK;
                        sibling->color = RED;
                        rotateRight(sibling);
                        sibling = node->parent->right;
                    }
                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;
                    if (sibling->right != nullptr)
                        sibling->right->color = BLACK;
                    rotateLeft(node->parent);
                    node = root;
                }
            }
            else {
                Node* sibling = node->parent->left;
                if (sibling->color == RED) {
                    sibling->color = BLACK;
                    node->parent->color = RED;
                    rotateRight(node->parent);
                    sibling = node->parent->left;
                }
                if ((sibling->left == nullptr
                     || sibling->left->color == BLACK)
                    && (sibling->right == nullptr
                        || sibling->right->color
                               == BLACK)) {
                    sibling->color = RED;
                    node = node->parent;
                }
                else {
                    if (sibling->left == nullptr
                        || sibling->left->color == BLACK) {
                        if (sibling->right != nullptr)
                            sibling->right->color = BLACK;
                        sibling->color = RED;
                        rotateLeft(sibling);
                        sibling = node->parent->left;
                    }
                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;
                    if (sibling->left != nullptr)
                        sibling->left->color = BLACK;
                    rotateRight(node->parent);
                    node = root;
                }
            }
        }
        node->color = BLACK;
    }

    // Find Node with Minimum Value (used when deleting a node)
    Node* minValueNode(Node*& node)
    {
        Node* current = node;
        while (current->left != nullptr)
            current = current->left;
        return current;
    }

    //Transplant nodes in Red-Black Tree (used when deleting a node)
    void transplant(Node*& root, Node*& u, Node*& v)
    {
        if (u->parent == nullptr)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;
        if (v != nullptr)
            v->parent = u->parent;
    }

    // print the tree - so that we can vizualize it
    void printHelper(Node* root, string indent, bool last)
    {
        if (root != nullptr) {
            cout << indent;
        if (indent == "") {
            cout << "Root----";
            indent += "\t";
        }
        else if (last) {
            cout << "R----";
            indent += "   ";
            }
            else {
            cout << "L----";
            indent += "|  ";
        }
        
            string sColor;
            if (root->color == RED)
                sColor = "RED";
            else
                sColor = "BLACK";
            cout << root->low << ", " << root->high << " (max=" << root->max << ", " << sColor << ")\n";
            printHelper(root->left, indent, false);
            printHelper(root->right, indent, true);
        }
    }

    // Delete node in a tree 
    void deleteTree(Node* node)
    {
        if (node != nullptr) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }
    
    //
    bool doOverlap(int low1, int high1, int low2, int high2) {
    return (low1 <= high2 && low2 <= high1);
    }

Node* overlapSearch(Node* root, int low, int high) {
    if (root == nullptr)
        return nullptr;

    if (doOverlap(root->low, root->high, low, high))
        return root;

    if (root->left != nullptr && root->left->max >= low)
        return overlapSearch(root->left, low, high);

    return overlapSearch(root->right, low, high);
}


public:
    //Initialize the Tree
    RedBlackIntervalTree() {
        root = nullptr;
    }

    // Deleteing the Tree (calls delete node function)
    ~RedBlackIntervalTree() { deleteTree(root); }

    // Insert a value into the tree
    void insert(int low, int high)
    {
        Node* node = new Node(low, high);
        Node* parent = nullptr;
        Node* current = root;
        while (current != nullptr) {
            parent = current;
            if (node->low < current->low)
                current = current->left;
            else
                current = current->right;
        }
        node->parent = parent;
        if (parent == nullptr)
            root = node;
        else if (node->low < parent->low)
            parent->left = node;
        else
            parent->right = node;
            
            
        
        Node* temp = node->parent;
        while (temp != nullptr) {
        updateMax(temp);
        temp = temp->parent;
        
    }
    
     fixInsert(node);
     
    }

    // Removes an interval in the tree
    void remove(int low, int high)
    {
        Node* node = root;
        Node* z = nullptr;
        Node* x = nullptr;
        Node* y = nullptr;
        while (node != nullptr) {
            if (node->low == low && node->high == high) {
                z = node;
                break;
            }

            if (node->low <= low) {
                node = node->right;
            }
            else {
                node = node->left;
            }
        }

        if (z == nullptr) {
            cout << "Interval not found in the tree" << endl;
            return;
        }

        y = z;
        Color yOriginalColor = y->color;
        if (z->left == nullptr) {
            x = z->right;
            transplant(root, z, z->right);
        }
        else if (z->right == nullptr) {
            x = z->left;
            transplant(root, z, z->left);
        }
        else {
            y = minValueNode(z->right);
            yOriginalColor = y->color;
            x = y->right;
            if (y->parent == z) {
                if (x != nullptr)
                    x->parent = y;
            }
            else {
                transplant(root, y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(root, z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        delete z;
if (yOriginalColor == BLACK) {
    if (x != nullptr)
        fixDelete(x);
    else if (y->parent != nullptr)
        fixDelete(y->parent);
}
        
    if (y == nullptr) return;
    Node* temp = y->parent;
    while (temp != nullptr) {
        updateMax(temp);
        temp = temp->parent;
    }

    }

    // Print the tree
    void printTree()
    {
        if (root == nullptr)
            cout << "Tree is empty." << endl;
        else {
            cout << "Red-Black Tree:" << endl;
            printHelper(root, "", true);
        }
    }
    
    void searchOverlap(int low, int high) {
    Node* res = overlapSearch(root, low, high);
    if (res != nullptr)
        cout << "Overlaps with [" << res->low << ", " << res->high << "]\n";
    else
        cout << "No overlapping interval found.\n";
}

    
    
};