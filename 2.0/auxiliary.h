#ifndef AUXILIARY
#define AUXILIARY

#include "main.h"

#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <stack>
#include <queue>

using namespace std;


void create_recommend(string id);
void merge_id(int id1, int id2);
int wildcmp(const char *wild, const char *string);
void transfer_recommend(string id);
int score(string u, string v);

#endif