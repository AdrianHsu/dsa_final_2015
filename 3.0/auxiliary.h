#ifndef AUXILIARY
#define AUXILIARY

#include "main.h"

#include <string>
#include <cstring>
#include <vector>

using namespace std;


void create_recommend(string id);
void merge_id(int id1, int id2);
int wildcmp(const char *wild, const char *string);
void transfer_recommend(string id);
int score(string u, string v);

#endif