#ifndef ACCOUNT
#define ACCOUNT

#include <string>
#include <list>
#include <vector>
#include <cstring>

using namespace std;

class Entry {
 public:
  Entry(bool fot, int value, int time_stamp) : _fot(fot), _value(value), _time_stamp(time_stamp){};

  bool _fot; // from = false, to = true
  int _value;
  int _time_stamp;
};
bool cmp(const Entry &a, const Entry &b);

class Content {
 public:
  Content(string id) : related_id(id) {};

  string related_id;

  list<Entry> history;

  void merge_content(Content *cont);
};

class Account {
 public:
  Account(string id, string password_md5) : 
    _balance(0), 
    _id(id),
    _password_md5(password_md5),
    _deleted(false) {};

  int _balance;
  string _id;
  string _password_md5;
  bool _deleted;

  vector<Content*> archive;

  void enroll(string id, bool fot, int value, int time_stamp);
  bool show_history(string id, Content &show);
};

#endif