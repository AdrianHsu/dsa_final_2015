#ifndef MAIN
#define MAIN

#include <string>
#include <map>
#include <vector>

using namespace std;

#include "function.h"
#include "auxiliary.h"


class Node {
public:
	Node() : id(""), password_md5(""), isDeleted(false), balance(0){};
	Node(string id, string password) : 
		id(id), 
		password_md5(password), 
		isDeleted(false), 
		balance(0) {};

	string id;
	string password_md5;
	bool isDeleted;
	int balance;

	vector<int> edges;
};

class Edge {
public:
	Edge() : starting(-1), ending(-1), money(0), change(0) {};
	Edge(int start, int end, int m) : starting(start), ending(end), money(m), change(0) {};
	
	int starting;
	int ending;
	int money;
	int change;	//1:starting	-1:ending
	
};



#endif