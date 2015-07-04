#include <iostream>
#include <string>
#include <list>

#include "md5.h"
#include "trie.h"
#include "auxiliary.h"
#include "function.h"
#include "account.h"

using namespace std;

Node *ROOT;
Account *current_account;
int counter;
// char PostFix[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

void input_num(int &num) {
  char ch;
  num = 0;

  do {
    ch = getchar();
    if (ch == EOF)
      return ;
  } while (ch < '0' || ch > '9');

  while (ch >= '0' && ch <= '9') {
    num *= 10;
    num += (ch - '0');
    ch = getchar();    
  }

  return ;
}

void input(char *a, char *b)
{
  scanf("%s%s", a, b);
}

bool input_command(char *s) {
  int i = 0;
  char ch;

  do {
    ch = getchar();
    if (ch == EOF) 
      return false;
  } while (ch < 'a' || ch > 'z');

  while (ch >= 'a' && ch <= 'z') {
    s[i] = ch;
    i++;
    ch = getchar();
  }

  return true;
}

int main() {
  ROOT = new Node();
  current_account = nullptr;
  counter = 0;
  char command[10];

   while (scanf("%s", command) != EOF) {
    char id[110];
    char password[110];
    if (command[0] == 'd' && command[2] == 'l') {
      input(id, password);
      delete_account(string(id), string(password));
    } 
    else if (command[0] == 'l') {
      input(id, password);
      login(string(id), string(password));     
    }
    else if (command[0] == 'c') {
      input(id, password);
      create(string(id), string(password)); 
    }
    else if (command[0] == 'm') {
      char id_2[110];
      char password_2[110];
      input(id, password);
      input(id_2, password_2);
      merge(string(id), string(password), string(id_2), string(password_2)); 
    }
    else if (command[0] == 'd' && command[2] == 'p') {
      int num = 0;
      input_num(num);
      deposit(num);
    }
    else if (command[0] == 'w') {
      int num = 0;
      input_num(num);
      withdraw(num);
    }
    else if (command[0] == 't') {
      int num = 0;
      scanf("%s", id);
      input_num(num);

      transfer(string(id), num);
    }
    else if (command[0] == 'f') {
      char wildcard_ID[110];
      scanf("%s", wildcard_ID);

      find_wildcard(string(wildcard_ID));
      // printf("\n");
    }
    else if (command[0] == 's') {
      scanf("%s", id);
      search(string(id));
    }
    else
      break;
  }

  return 0;
}

