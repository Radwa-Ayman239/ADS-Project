#include "../../include/structures/IntervalTreeComplete.h"
#include "../../include/helpers/ResourceIO.h"
using namespace std;

// ===================== Node constructor =====================
RedBlackIntervalTree::Node::Node(int l, int h, const std::string &user) {
  low = l;
  high = h;
  max = h;
  color = RED;
  parent = nullptr;
  left = nullptr;
  right = nullptr;
  bookedBy = user;
}

// ===================== Private helper implementations =====================

void RedBlackIntervalTree::updateMax(Node *node) {
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

void RedBlackIntervalTree::rotateLeft(Node *&node) {
  Node *child = node->right;
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

void RedBlackIntervalTree::rotateRight(Node *&node) {
  Node *child = node->left;
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

void RedBlackIntervalTree::fixInsert(Node *&node) {
  Node *parent = nullptr;
  Node *grandparent = nullptr;
  while (node != root && node->color == RED && node->parent->color == RED) {
    parent = node->parent;
    grandparent = parent->parent;
    if (parent == grandparent->left) {
      Node *uncle = grandparent->right;
      if (uncle != nullptr && uncle->color == RED) {
        grandparent->color = RED;
        parent->color = BLACK;
        uncle->color = BLACK;
        node = grandparent;
      } else {
        if (node == parent->right) {
          rotateLeft(parent);
          node = parent;
          parent = node->parent;
        }
        rotateRight(grandparent);
        std::swap(parent->color, grandparent->color);
        node = parent;
      }
    } else {
      Node *uncle = grandparent->left;
      if (uncle != nullptr && uncle->color == RED) {
        grandparent->color = RED;
        parent->color = BLACK;
        uncle->color = BLACK;
        node = grandparent;
      } else {
        if (node == parent->left) {
          rotateRight(parent);
          node = parent;
          parent = node->parent;
        }
        rotateLeft(grandparent);
        std::swap(parent->color, grandparent->color);
        node = parent;
      }
    }
  }
  root->color = BLACK;
}

void RedBlackIntervalTree::fixDelete(Node *&node) {
  if (node == nullptr)
    return;

  while (node != root && node->color == BLACK) {
    if (node == node->parent->left) {
      Node *sibling = node->parent->right;
      if (sibling->color == RED) {
        sibling->color = BLACK;
        node->parent->color = RED;
        rotateLeft(node->parent);
        sibling = node->parent->right;
      }
      if ((sibling->left == nullptr || sibling->left->color == BLACK) &&
          (sibling->right == nullptr || sibling->right->color == BLACK)) {
        sibling->color = RED;
        node = node->parent;
      } else {
        if (sibling->right == nullptr || sibling->right->color == BLACK) {
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
    } else {
      Node *sibling = node->parent->left;
      if (sibling->color == RED) {
        sibling->color = BLACK;
        node->parent->color = RED;
        rotateRight(node->parent);
        sibling = node->parent->left;
      }
      if ((sibling->left == nullptr || sibling->left->color == BLACK) &&
          (sibling->right == nullptr || sibling->right->color == BLACK)) {
        sibling->color = RED;
        node = node->parent;
      } else {
        if (sibling->left == nullptr || sibling->left->color == BLACK) {
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

RedBlackIntervalTree::Node *RedBlackIntervalTree::minValueNode(Node *&node) {
  Node *current = node;
  while (current->left != nullptr)
    current = current->left;
  return current;
}

void RedBlackIntervalTree::transplant(Node *&root, Node *&u, Node *&v) {
  if (u->parent == nullptr)
    root = v;
  else if (u == u->parent->left)
    u->parent->left = v;
  else
    u->parent->right = v;
  if (v != nullptr)
    v->parent = u->parent;
}

void RedBlackIntervalTree::printHelper(Node *root, std::string indent,
                                       bool last) {
  if (root != nullptr) {
    std::cout << indent;
    if (indent == "") {
      std::cout << "Root----";
      indent += "\t";
    } else if (last) {
      std::cout << "R----";
      indent += "   ";
    } else {
      std::cout << "L----";
      indent += "|  ";
    }

    std::string sColor = (root->color == RED) ? "RED" : "BLACK";
    std::cout << root->low << ", " << root->high << " (max=" << root->max
              << ", " << sColor << ")\n";

    printHelper(root->left, indent, false);
    printHelper(root->right, indent, true);
  }
}

void RedBlackIntervalTree::deleteTree(Node *node) {
  if (node != nullptr) {
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
  }
}

bool RedBlackIntervalTree::doOverlap(int low1, int high1, int low2, int high2) {
  return (low1 < high2 && low2 < high1);
}

RedBlackIntervalTree::Node *
RedBlackIntervalTree::overlapSearch(Node *root, int low, int high) {
  if (root == nullptr)
    return nullptr;

  if (doOverlap(root->low, root->high, low, high))
    return root;

  if (root->left != nullptr && root->left->max >= low)
    return overlapSearch(root->left, low, high);

  return overlapSearch(root->right, low, high);
}

// void RedBlackIntervalTree::listAvailableIntervalsHelper(Node* node, int
// StartLooking, int EndLooking, int &currenttime,
// std::vector<std::pair<int,int> > &availabletimes) {
//     if (node == nullptr) return;

//     if (node->left != nullptr && node->left->max >= StartLooking)
//         listAvailableIntervalsHelper(node->left, StartLooking, EndLooking,
//         currenttime, availabletimes);

//     if (currenttime < node->low) {
//         int startFree = currenttime;
//         int endFree = node->low;
//         if (startFree < endFree)
//         availabletimes.push_back(std::make_pair(startFree, endFree));
//     }

//     if (currenttime < node->high) currenttime = node->high;

//     if (node->right != nullptr && node->right->low < EndLooking)
//         listAvailableIntervalsHelper(node->right, StartLooking, EndLooking,
//         currenttime, availabletimes);
// }

void RedBlackIntervalTree::listAvailableIntervalsHelper(Node *node,
                                                        int StartLooking,
                                                        int EndLooking,
                                                        int &currenttime,
                                                        int &counter) {
  if (node == nullptr)
    return;

  if (node->left != nullptr && node->left->max >= StartLooking)
    listAvailableIntervalsHelper(node->left, StartLooking, EndLooking,
                                 currenttime, counter);

  if (currenttime < node->low) {
    int startFree = currenttime;
    int endFree = node->low;
    if (startFree < endFree)
      cout << counter << ". [ " << formatTimestamp(startFree) << ", "
           << formatTimestamp(endFree) << " ]\n";
    counter++;
  }

  if (currenttime < node->high)
    currenttime = node->high;

  if (node->right != nullptr && node->right->low < EndLooking)
    listAvailableIntervalsHelper(node->right, StartLooking, EndLooking,
                                 currenttime, counter);
}

// ===================== Public methods =====================

RedBlackIntervalTree::RedBlackIntervalTree() { root = nullptr; }

RedBlackIntervalTree::~RedBlackIntervalTree() { deleteTree(root); }

void RedBlackIntervalTree::insert(int low, int high, const std::string &user) {
  Node *node = new Node(low, high, user);
  Node *parent = nullptr;
  Node *current = root;
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

  Node *temp = node->parent;
  while (temp != nullptr) {
    updateMax(temp);
    temp = temp->parent;
  }

  fixInsert(node);
}

void RedBlackIntervalTree::remove(int low, int high) {
  Node *node = root;
  Node *z = nullptr;
  Node *x = nullptr;
  Node *y = nullptr;

  while (node != nullptr) {
    if (node->low == low && node->high == high) {
      z = node;
      break;
    }
    if (node->low <= low)
      node = node->right;
    else
      node = node->left;
  }

  if (z == nullptr) {
    std::cout << "Interval not found\n";
    return;
  }

  y = z;
  Color yOriginalColor = y->color;
  if (z->left == nullptr) {
    x = z->right;
    transplant(root, z, z->right);
  } else if (z->right == nullptr) {
    x = z->left;
    transplant(root, z, z->left);
  } else {
    y = minValueNode(z->right);
    yOriginalColor = y->color;
    x = y->right;
    if (y->parent == z) {
      if (x != nullptr)
        x->parent = y;
    } else {
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

  if (y == nullptr)
    return;
  Node *temp = y->parent;
  while (temp != nullptr) {
    updateMax(temp);
    temp = temp->parent;
  }
}

// bool RedBlackIntervalTree::searchOverlap(int low, int high) {

//     return overlapSearch(root, low, high) != nullptr;
// }

bool RedBlackIntervalTree::searchOverlap(int low, int high, bool announce) {
  RedBlackIntervalTree::Node *overlapNode = overlapSearch(root, low, high);

  if (overlapNode != nullptr) {
    if (announce == true) {
      cout << "\nThis period overlaps with the following already booked "
              "period: [ "
           << overlapNode->low << ", " << overlapNode->high << " ]";
    }

    return true;
  } else {
    return false;
  }
}

// std::vector<std::pair<int,int> >
// RedBlackIntervalTree::listAvailableIntervals(int StartHere, int EndHere) {
//     std::vector<std::pair<int,int> > available;
//     int current = StartHere;
//     listAvailableIntervalsHelper(root, StartHere, EndHere, current,
//     available); if (current <= EndHere)
//     //available.push_back(std::make_pair(current, EndHere));
//     //return available;
//     cout << "[ " << current << ", " << EndHere << " ]";
// }

void RedBlackIntervalTree::listAvailableIntervals(int StartHere, int EndHere) {
  // std::vector<std::pair<int, int> > available;
  int current = StartHere;
  int counter = 1;
  listAvailableIntervalsHelper(root, StartHere, EndHere, current, counter);
  if (current <= EndHere)
    cout << counter << ". [ " << formatTimestamp(current) << ", "
         << formatTimestamp(EndHere) << " ]\n";
}

void RedBlackIntervalTree::printTree() {
  if (root == nullptr)
    std::cout << "Tree is empty.\n";
  else {
    std::cout << "Red-Black Tree:\n";
    printHelper(root, "", true);
  }
}
