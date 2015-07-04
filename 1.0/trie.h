#ifndef TRIE
#define TRIE

#include <list>

#include "account.h"

using namespace std;

class Node {
 public:
  Node() : _ch(0), _prev(nullptr) {};
  Node(char ch, Node *prev) : _ch(ch), _prev(prev) {};

  char _ch;
  Node *_prev;

  list<Node*> children;
  list<Account*> id_generation;

  Node* find_child(char ch);
  Node* insert(char ch);
};

#endif