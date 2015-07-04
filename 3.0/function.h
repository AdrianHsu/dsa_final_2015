#ifndef FUNCTION
#define FUNCTION

#include <map>
#include <vector>
#include <algorithm>

#include "main.h"
#include "md5.h"
#include "auxiliary.h"

using namespace std;


void login(string id, string password);
void create(string id, string password);
void delete_account(string id, string password);
void merge(string id_1, string password_1, string id_2, string password_2);

void deposit(int value);
void withdraw(int value);
void transfer(string id, int value);
void find_wildcard(string wildcard_id);
void search(string id);

#endif