#include "main.h"

map<string, int> Accounts;
vector<Edge> Edges;
vector<Node*> Database;
Node *current_account;
char PostFix[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

inline void input(char *a, char *b)
{
  scanf("%s%s", a, b);
}

int main(int argc, char const *argv[])
{
	current_account = nullptr;
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
      scanf("%d", &num);
      deposit(num);
    }
    else if (command[0] == 'w') {
      int num = 0;
      scanf("%d", &num);
      withdraw(num);
    }
    else if (command[0] == 't') {
      int num = 0;
      scanf("%s%d", id, &num);

      transfer(string(id), num);
    }
    else if (command[0] == 'f') {
      char wildcard_ID[110];
      scanf("%s", wildcard_ID);

      find_wildcard(string(wildcard_ID));
      //printf("\n");
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