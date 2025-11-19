#ifndef REDBLACKINTERVALTREE_H
#define REDBLACKINTERVALTREE_H

#include <iostream>
#include <climits>
#include <vector>

enum Color { RED, BLACK };

class RedBlackIntervalTree {
private:
    struct Node {
        int low;
        int high;
        int max;
        Color color;
        Node *parent;
        Node *left;
        Node *right;

        Node(int l, int h);
    };

    Node *root;

    //Private functions
    void updateMax(Node *node);

    void rotateLeft(Node *&node);

    void rotateRight(Node *&node);

    void fixInsert(Node *&node);

    void fixDelete(Node *&node);

    Node *minValueNode(Node *&node);

    void transplant(Node *&root, Node *&u, Node *&v);

    void printHelper(Node *root, std::string indent, bool last);

    void deleteTree(Node *node);

    bool doOverlap(int low1, int high1, int low2, int high2);

    Node *overlapSearch(Node *root, int low, int high);

    //void listAvailableIntervalsHelper(Node* node, int StartLooking, int EndLooking, int &currenttime, std::vector<std::pair<int,int> > &availabletimes);
    void listAvailableIntervalsHelper(Node *node, int StartLooking, int EndLooking, int &currenttime, int &counter);

    template<typename Func>
    void forEachIntervalHelper(Node* node, Func func) {
        if (!node) return;
        forEachIntervalHelper(node->left, func);
        func(node->low, node->high);
        forEachIntervalHelper(node->right, func);
    }

public:
    RedBlackIntervalTree();

    ~RedBlackIntervalTree();

    //Public functions
    void insert(int low, int high);

    void remove(int low, int high);

    bool searchOverlap(int low, int high, bool announce);

    void listAvailableIntervals(int StartHere, int EndHere);

    void printTree();

    template<typename Func>
    void forEachInterval(Func func) {
        forEachIntervalHelper(root, func);
    }
};

#endif
