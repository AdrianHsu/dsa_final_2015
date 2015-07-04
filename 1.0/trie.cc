#include "trie.h"

Node* Node::find_child(char ch) {
  list<Node*>::iterator it;

  for (it = children.begin(); it != children.end(); ++it) {
    if ((*it)->_ch == ch) {
      return (*it);
    }
  }

  return nullptr;
}

Node* Node::insert(char ch) {
  list<Node*>::iterator it;

  Node *buf = new Node(ch, this);
  for (it = children.begin(); it != children.end(); ++it) {
    if ((*it)->_ch > ch) {
      children.insert(it, buf);

      return buf;
    }
  }
  children.push_back(buf);

  return buf;
}