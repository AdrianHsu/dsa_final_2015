#include "function.h"
extern map<string, int> Accounts;
extern vector<Node*> Database;
extern Node *current_account;
extern vector<Edge> Edges;

using namespace std;

//function that are not associated with the last-successful-login-ID
void login(string id, string password) {
  /*login to a specific account
  IF id cannot be found
    print "ID [id] not found"
    return
  ELSE IF password is wrong
    print "wrong password"
    return
  ELSE
    current_id = id
    print "success" 
  */

  map<string, int>::iterator it = Accounts.find(id);
  if (it == Accounts.end() || it->second == -1) {
    printf("ID %s not found\n", id.c_str());

    return ;
  } 

  if (Database[it->second]->password_md5 != md5(password)) {
    printf("wrong password\n");
  } else {
    current_account = Database[it->second];
    printf("success\n");
  }

  return ;
}

void create(string id, string password) {
  /*create a specific account with password
  IF id is found
    print "ID [id] exists, [best 10 unused accounts separated by ', ']"
  ELSE
    create a new Element with id = id, password = password
    print "success"
  */

  map<string, int>::iterator it = Accounts.find(id);
  
  if (it == Accounts.end() || it->second == -1) {
    Node *node = new Node(id, md5(password));
    Database.push_back(node);
    Accounts[id] = Database.size()-1;
    
    printf("success\n");
  } else {
    printf("ID %s exists, ", id.c_str());
    create_recommend(id);
    printf("\n");
  }

  return ;
}

void delete_account(string id, string password) {
  /*delete a specific account with password
  IF id cannot be found
    print "ID [id] not found"
    return
  ELSE IF password is wrong
    print "wrong password"
    return
  ELSE
    delete the user
    print "success" 
  */

  map<string, int>::iterator it = Accounts.find(id);
  
  if (it == Accounts.end() || it->second == -1) {
    printf("ID %s not found\n", id.c_str());

    return ;
  }

  if (Database[it->second]->password_md5 !=  md5(password)) {
    printf("wrong password\n");
  } else {
    Database[it->second]->isDeleted = true;
    it->second = -1;
    printf("success\n");
  }
  
  return ;
}

void merge(string id_1, string password_1, string id_2, string password_2) {
  /*merge the second account into the first one; after merging, the second one is deleted
  IF id_1 cannot be found
    print "ID [id_1] not found"
    return
  ELSE IF id_2 cannot be found
    print "ID [id_2] not found"
    return
  ELSE IF password_1 is wrong
    print "wrong password1"
    return
  ELSE IF password_2 is wrong
    print "wrong password2"
    return
  ELSE
    merge the money and transfer history of id_2 to id_1
    print "success, [id_1] has [money of id_1 after operation] dollars" 


  P.S note1: You can assume that [ID1] ̸= [ID2] and [ID2] ̸= [last-successful-login-ID].
  We need to merge two things: the money, and the transfer history (as if all transfer records of [ID2] from/to other IDs are replaced by [ID1]).
  
  P.S note2: If [ID1] and [ID2] have transfer history between them (i.e. there will be two records at the same time stamp), 
  the record of [ID1] is considered “earlier” and the other record is considered “later” during search
  */

  map<string, int>::iterator it1 = Accounts.find(id_1);
  map<string, int>::iterator it2 = Accounts.find(id_2);
  
  if (it1 == Accounts.end() || it1->second == -1) {
    printf("ID %s not found\n", id_1.c_str());
  } else if (it2 == Accounts.end() || it2->second == -1) {
    printf("ID %s not found\n", id_2.c_str());
  } else if (Database[it1->second]->password_md5 != md5(password_1)) {
    printf("wrong password1\n");
  } else if (Database[it2->second]->password_md5 != md5(password_2)) {
    printf("wrong password2\n");
  } else {
    merge_id(it1->second, it2->second);
    printf("success, %s has %d dollars\n", id_1.c_str(), Database[it1->second]->balance);
  }
}

//function that are associated with the last-successful-login-ID, current-id = last-successful-login-ID
void deposit(int value) {
  /*deposit money into [last-successful-login-ID]
  current_id -> money += money
  print "success, [current_id -> money] dollars in current account"
  */

  if (current_account == nullptr) {
    return ;
  }

  current_account->balance += value;
  printf("success, %d dollars in current account\n", current_account->balance);

  return ;
}

void withdraw(int value) {
  /*withdraw money from [last-successful-login-ID]
  IF current_id -> money < money
    print "fail, [current_id -> money] dollars only in current account"
  ELSE
    current_id -> money -= money
    print "success, [current_id -> money] dollars left in current account"
  */

  if (current_account == nullptr) {
    return ;
  }
  if (current_account->balance < value) {
    printf("fail, %d dollars only in current account\n", current_account->balance);
  } else {
    current_account->balance -= value;
    printf("success, %d dollars left in current account\n", current_account->balance);
  }

  return ;
}

void transfer(string id, int value) {
  /*transfer money from [last-successful-login-ID] to a specific account
  IF id not found
    print "ID [id] not found, [best 10 existing accounts separated by ', ']"
    //If there are fewer than 10 existing account IDs, just output them all
  ELSE IF current_id -> money < money
    print "fail, [current_id -> money] dollars only in current account"
  ELSE
    current_id -> money -= money
    id -> money += money
    make two transfer history: current_id to id and id to current_id
    print "success, [current_id -> money] dollars left in current account"

  note: you can assume that [ID] ≠= [last-successful-login-ID]
  */

  if (current_account == nullptr) {
    return ;
  }

  map<string, int>::iterator it1 = Accounts.find(id);
  
  if (it1 == Accounts.end() || it1->second == -1) {
    printf("ID %s not found, ", id.c_str());
    transfer_recommend(id);
    printf("\n");

    return ;
  }

  if (current_account->balance < value) {
    printf("fail, %d dollars only in current account\n", current_account->balance);
  } else {
    current_account->balance -= value;
    Database[it1->second]->balance += value;

    map<string, int>::iterator it2 = Accounts.find(current_account->id);
    Edge trans(it2->second, it1->second, value);
    Edges.push_back(trans);

    Database[it1->second]->edges.push_back(Edges.size()-1);
    Database[it2->second]->edges.push_back(Edges.size()-1);

    printf("success, %d dollars left in current account\n", current_account->balance);
  }

  return ;
}

void find_wildcard(string wildcard_id) {
  /*find all existing account IDs that matches [wildcard ID] but is different
  print all satisfying IDs (separated by ",") in ascending dictionary order

  [wildcard ID] is a ID string that may also contain * or ?, 
  where * stands for 0 or more characters, and ? stands for exactly 1 character. 
  There might be multiple ? or *, but a * will not sit beside ? nor *. 
  That is, there is no ?*, **, nor *?. The following are some examples:
    a*b?: ab0,a0b1,a00b2,abbbbb ...... 
    a??b*: abbb,a01bbbb,a01bacb ...... 
    a*b*: ab,abb,abbbb,acccbccab ...... 
    *a?b*: aab,cccasbbb,twaiblwe ...... 
    a**b: will not be in the test data
  Note that you can simply use strcmp in C for what we mean by “ascending dictionary order.” If strcmp(u, v) < 0, then u should be placed earlier than v.
  */

  vector<string> result;
  map<string, int>::iterator it = Accounts.begin();

  while (it != Accounts.end()) {
    if (it->second == -1 || current_account->id == it->first) {
      ++it;
      continue;
    }

    if (wildcmp(wildcard_id.c_str(), it->first.c_str())) {
      result.push_back(it->first);
    }
    ++it;
  }

  for(int i = 0; i < result.size(); i++) {
    printf("%s", result[i].c_str());
    if(i != result.size() - 1)
      printf(",");
  }
  printf("\n");
}

void search(string id) {
  /*search all transfer history of [last-successful-login-ID] from/to a specific account
  pick out the current_id and find out his transfer history.
  At last, print them out in time order, the earliest being the first. 
  Each line should follow the format: [From/To] [ID] [num]. The following are some examples:
          From Frank 10000
          To Frank 2000
          ......and so on
    */

  map<string, int>::iterator it = Accounts.find(id);
  sort(current_account->edges.begin(),current_account->edges.end());

  
  if(it == Accounts.end()) {
    printf("no record\n");

    return ;
  }

  bool is_found = false;

  for (int i = 0; i < current_account->edges.size(); i++) {

      int starting = Edges[current_account->edges[i]].starting;
      int ending = Edges[current_account->edges[i]].ending;
      int money = Edges[current_account->edges[i]].money;

    if ((Database[starting]->id==id && Database[ending]->id==current_account->id) ||
        (Database[ending]->id==id && Database[starting]->id==current_account->id)) {
      // assert(Database[starting]->id==id && Database[ending]->id==current_account->id ||
      //   Database[ending]->id==id && Database[starting]->id==current_account->id);

      if (starting == ending) {
        if (Edges[current_account->edges[i]].change == 1) {
          printf("From %s %d\n", Database[starting]->id.c_str(), money);
          printf("To %s %d\n", Database[ending]->id.c_str(), money);
          is_found = true;
        } else if (Edges[current_account->edges[i]].change == -1) {
          printf("To %s %d\n", Database[ending]->id.c_str(), money);
          printf("From %s %d\n",Database[starting]->id.c_str(), money);
          is_found = true;
        }
      } else {
        map<string, int>::iterator it2 = Accounts.find(current_account->id);

        if (it2 == Accounts.end() || it2->second == -1)
          return ;

        if (it2->second == starting) {
          printf("To %s %d\n", Database[ending]->id.c_str(), money);
          is_found = true;
        } else if (it2->second == ending) {
          printf("From %s %d\n", Database[starting]->id.c_str(), money);
          is_found = true;
        }

      }
    }
  }


  if (is_found == false)
    printf("no record\n");

  return ;
}
