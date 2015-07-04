#include "function.h"

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

  Account *user = find_id(id);
  if (user == nullptr) {
    printf("ID %s not found\n", id.c_str());

    return ;
  } 

  if (user->_password_md5 != md5(password)) {
    printf("wrong password\n");
  } else {
    current_account = user;
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

  Account *user = find_id(id);
  if (user == nullptr) {
    create_id(id, md5(password));
    printf("success\n");
  } else {
    printf("ID %s exists, ", id.c_str());
    recommend_unused(id);
    printf("\n");
  }
  // if (create_id(id, md5(password))) {
  //   printf("success\n");
  // } else {
  //   printf("ID %s exists, ", id.c_str());
  //   recommend_unused(id);
  //   printf("\n");
  // }

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
  Account *user = find_id(id);
  
  if (user == nullptr) {
    printf("ID %s not found\n", id.c_str());

    return ;
  }

  if (user->_password_md5 !=  md5(password)) {
    printf("wrong password\n");
  } else {
    delete_id(id);
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

  Account *user1 = find_id(id_1);
  Account *user2 = find_id(id_2);
  if (user1 == nullptr) {
    printf("ID %s not found\n", id_1.c_str());
  } else if (user2 == nullptr) {
    printf("ID %s not found\n", id_2.c_str());
  } else if (user1->_password_md5 != md5(password_1)) {
    printf("wrong password1\n");
  } else if (user2->_password_md5 != md5(password_2)) {
    printf("wrong password2\n");
  } else {
    merge_id(id_1, id_2);
    printf("success, %s has %d dollars\n", id_1.c_str(), user1->_balance);
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

  current_account->_balance += value;
  printf("success, %d dollars in current account\n", current_account->_balance);

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
  if (current_account->_balance < value) {
    printf("fail, %d dollars only in current account\n", current_account->_balance);
  } else {
    current_account->_balance -= value;
    printf("success, %d dollars left in current account\n", current_account->_balance);
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

  Account *user = find_id(id);
  if (user == nullptr) {
    printf("ID %s not found, ", id.c_str());
    recommend(id);
    printf("\n");

    return ;
  }

  if (current_account->_balance < value) {
    printf("fail, %d dollars only in current account\n", current_account->_balance);
  } else {
    current_account->_balance -= value;
    user->_balance += value;

    user->enroll(current_account->_id, false, value, counter);
    current_account->enroll(id, true, value, counter);
    counter++;

    printf("success, %d dollars left in current account\n", current_account->_balance);
  }

  return ;
}

void find_wildcard(string id) {
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
  if (current_account == nullptr)
    return ;
  find_traversal( id );

  //printf("\n");
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

  Node *new_id = find_node(id);

  if(new_id == nullptr || new_id->id_generation.size() == 0) {
    printf("no record\n");

    return ;
  }

  bool is_found = false;

  Content show(id);
  list<Account*>::iterator it;
  for (it = new_id->id_generation.begin(); it != new_id->id_generation.end(); ++it) {
    string now = (*it)->_id;

    if (current_account->show_history(now, show))
      is_found = true;
  }

  if (new_id->id_generation.back()->_deleted == false) {
    string now = new_id->id_generation.back()->_id + "-";
    if (current_account->show_history(now, show))
      is_found = true;
  }

  if (is_found == false)
    printf("no record\n");

  list<Entry>::iterator ite;
  for (ite = show.history.begin(); ite != show.history.end(); ++ite) {
    if ((*ite)._fot)
      printf("To ");
    else
      printf("From ");

    printf("%s %d\n", id.c_str(), (*ite)._value);
  }

  return ;
}
