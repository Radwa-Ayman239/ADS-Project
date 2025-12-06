#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iostream>

template <typename T> class LinkedList {
private:
  struct Node {
    T data;
    Node *next;
    Node(T val) : data(val), next(nullptr) {}
  };

  Node *head;
  int size_;

public:
  LinkedList() : head(nullptr), size_(0) {}

  ~LinkedList() { clear(); }

  void push_back(const T &value) {
    Node *newNode = new Node(value);
    if (!head) {
      head = newNode;
    } else {
      Node *current = head;
      while (current->next) {
        current = current->next;
      }
      current->next = newNode;
    }
    size_++;
  }

  // Remove the first occurrence of value
  bool remove(const T &value) {
    if (!head)
      return false;

    if (head->data == value) {
      Node *temp = head;
      head = head->next;
      delete temp;
      size_--;
      return true;
    }

    Node *current = head;
    while (current->next) {
      if (current->next->data == value) {
        Node *temp = current->next;
        current->next = current->next->next;
        delete temp;
        size_--;
        return true;
      }
      current = current->next;
    }
    return false;
  }

  void clear() {
    while (head) {
      Node *temp = head;
      head = head->next;
      delete temp;
    }
    size_ = 0;
  }

  int size() const { return size_; }

  bool empty() const { return size_ == 0; }

  template <typename Func> void forEach(Func func) const {
    Node *current = head;
    while (current) {
      func(current->data);
      current = current->next;
    }
  }

  // Check if value exists
  bool contains(const T &value) const {
    Node *current = head;
    while (current) {
      if (current->data == value)
        return true;
      current = current->next;
    }
    return false;
  }
};

#endif // LINKED_LIST_H
