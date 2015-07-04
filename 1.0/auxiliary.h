#ifndef AUXILIARY
#define AUXILIARY

#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <stack>
#include <queue>

#include "account.h"
#include "trie.h"

using namespace std;

extern Account* current_account;
extern Node *ROOT;
// extern char PostFix[];

class compare {
	public:
  	bool operator() (pair<string, int> &a, pair<string, int> &b) {
  		if (a.second == b.second)
	    	return strcmp(a.first.c_str(), b.first.c_str()) > 0;

	  	return a.second > b.second;
  	}  
};
typedef priority_queue< pair<string, int>, vector< pair<string, int> >, compare > Suggestion;

Node* find_node(string id);
Account* find_id(string id);
void combine_id(string id, string id_1, string id_2);
void merge_id(string id_1, string id_2);
void create_id(string id, string password_md5);
void delete_id(string id);

void find_traversal(string wildcard_id);
void traverse_find(Node* node, string wildcard_id, vector<string> &result);
int wildcmp(const char *wild, const char *string);

void traverse_find();

void recommend_unused(string id);
void recommend(string id);
void traverse_transfer(Node* node, Suggestion &result, string id);
int score(string u, string v);


#endif