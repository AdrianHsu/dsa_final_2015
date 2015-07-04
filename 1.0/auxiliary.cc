#include "auxiliary.h"

Node* find_node(string id) {
  Node *current = ROOT;

  for (int i = 0; i != id.size(); i++) {
    char now = id[i];

    current = current->find_child(now);
    if (current == nullptr) {
      return nullptr;
    }
  }

  return current;
}

Account* find_id(string id) {
  if (id.back() == '-')
    id.pop_back();
  Node *buf = find_node(id);

  if (buf == nullptr || buf->id_generation.size() == 0) {
    return nullptr;
  }

  if (buf->id_generation.back() != nullptr)
  { 
    if(!buf->id_generation.back()->_deleted)
        return buf->id_generation.back();
  }

  return nullptr;
}


bool compare(pair<string, int> &a, pair<string, int> &b) {
  if (a.second == b.second)
    return a.first < b.first;

  return a.second < b.second;
}


void combine_id(string id, string id_1, string id_2) {
  Account *user = find_id(id);

  if (user == nullptr)
    return ;

  if (user->archive.size() == 0)
    return ;

  int l = 0, r = user->archive.size();
  int mid = (l + r) / 2;

  while (r - l > 1) {
    string now = user->archive[mid]->related_id;
    int buf = strcmp(now.c_str(), id_2.c_str());

    if (buf == 0)
      break;

    if (buf < 0)
      l = mid;
    else
      r = mid;
    mid = (l + r) / 2;
  }

  int pos_2 = -1;
  if (user->archive[mid]->related_id == id_2) {
    pos_2 = mid;
  }

  l = 0, r = user->archive.size();
  mid = (l + r) / 2;

  while (r - l > 1) {
    string now = user->archive[mid]->related_id;
    int buf = strcmp(now.c_str(), id_1.c_str());

    if (buf == 0)
      break;

    if (buf < 0)
      l = mid;
    else
      r = mid;
    mid = (l + r) / 2;
  }

  int pos_1 = -1;
  if (user->archive[mid]->related_id == id_1) {
    pos_1 = mid;
  }

  if (pos_1 != -1 && pos_2 != -1) {
    user->archive[pos_1]->merge_content(user->archive[pos_2]);

    user->archive.erase(user->archive.begin() + pos_2);
  } else if (pos_2 != -1) {
    if (mid == 0) {
      if (user->archive[0]->related_id < id_1)
        mid++;
    } else {
      mid++;
    }

    user->archive.insert(user->archive.begin() + mid, new Content(id_1));

    if (mid <= pos_2)
      pos_2++;
    user->archive[mid]->merge_content(user->archive[pos_2]);

    user->archive.erase(user->archive.begin() + pos_2);
  }

  return ;
}

void merge_id(string id_1, string id_2) { // 2 to 1
  Account *user1 = find_id(id_1);
  Account *user2 = find_id(id_2);

  user1->_balance += user2->_balance;

  vector<Content*> combination;

  int it1 = 0, it2 = 0;
  while(it1 < user1->archive.size() && it2 < user2->archive.size()) {
    string now1 = user1->archive[it1]->related_id;
    string now2 = user2->archive[it2]->related_id;

    int buf = strcmp(now1.c_str(), now2.c_str());

    if (buf == 0) {
      user1->archive[it1]->merge_content(user2->archive[it2]);
      combination.push_back(user1->archive[it1]);

      if (id_1 + "-" != now1 && id_2 + "-" != now1) {
        if (now1.back() == '-') {
          now1.pop_back();
          combine_id(now1, id_1 + "-", id_2 + "-");
        }
      }

      it1++; 
      it2++;

      continue;
    }

    if (buf < 0) {
      combination.push_back(user1->archive[it1]);
      it1++;
    } else {
      if (now2 != id_1 + "-") {
        // Account *user = find_id(user2->archive[it2]->related_id);
        // if(user != nullptr)
        //   user->change_name(id_2 + "-", id_1 + "-", id_1);
        if (now2.back() == '-') {
          now2.pop_back();
          combine_id(now2, id_1 + "-", id_2 + "-");
        }
      }

      combination.push_back(user2->archive[it2]);
      it2++;
    }
  }

  while(it1 < user1->archive.size()) {
    combination.push_back(user1->archive[it1]);
    it1++;
  }
  while(it2 < user2->archive.size()) {
    if (user2->archive[it2]->related_id != id_1 + "-") {
      // Account *user = find_id(user2->archive[it2]->related_id);
      // if(user != nullptr)
      //   user->change_name(id_2 + "-", id_1 + "-", id_1);
      string now = user2->archive[it2]->related_id;
      if (now.back() == '-') {
        now.pop_back();
        combine_id(now, id_1 + "-", id_2 + "-");
      }
    }

    combination.push_back(user2->archive[it2]);
    it2++;
  }

  user1->archive.clear();
  user1->archive = combination;

  combine_id(id_1, id_1 + "-", id_2 + "-");

  delete user2;

  find_node(id_2)->id_generation.pop_back();
  
  return ;
}

// bool create_id(string id, string password_md5) {
//   Node *current = ROOT;

//   for (int i = 0; i != id.size(); i++) {
//     char now = id[i];

//     Node *tmp = current->find_child(now);
//     if (tmp == nullptr) {
//        current = current->insert(now);
//     } else {
//       current = tmp;
//     }
//   }

//   if (current->id_generation.size() == 0) {
//     current->id_generation.push_back(new Account(id, password_md5));

//     return true;
//   }

//   if (current->id_generation.back() != nullptr)
//     if(!current->id_generation.back()->_deleted)
//         return false;

//   current->id_generation.push_back(new Account(id, password_md5));

//   return true;
// }

void create_id(string id, string password_md5) {
  Node *current = ROOT;

  for (int i = 0; i != id.size(); i++) {
    char now = id[i];

    Node *tmp = current->find_child(now);
    if (tmp == nullptr) {
       current = current->insert(now);
    } else {
      current = tmp;
    }
  }

  current->id_generation.push_back(new Account(id, password_md5));

  return ;
}

void delete_id(string id) {
  Node *user_node = find_node(id);
  
  if (user_node == nullptr) {
    return;
  }

  if (user_node->id_generation.back() == nullptr) {
    return;
  }

  user_node->id_generation.back()->_deleted = true;
  
  stringstream ss;
  string postfix;
  ss << user_node->id_generation.size();
  ss >> postfix;
  string new_id = id + "#" + postfix;

  user_node->id_generation.back()->_id = new_id;

  vector<Content*>::iterator it;
  for (it = user_node->id_generation.back()->archive.begin(); it != user_node->id_generation.back()->archive.end(); ++it) {
    // Account *user = find_id((*it)->related_id);
    // if(user != nullptr)
    //   user->change_name(id + "-", new_id, id);
    string now = (*it)->related_id;
    if (now != id + "-") {
      if (now.back() == '-') {
        now.pop_back();
        combine_id(now, new_id, id + "-");
      }
    }
  }

  return ;
}

void find_traversal(string wildcard_id) {

  vector< string > result;
  traverse_find(ROOT, wildcard_id, result);
  for(int i = 0; i < result.size(); i++) {
    printf("%s", result[i].c_str());
    if(i != result.size() - 1)
      printf(",");
  }
  printf("\n");
  return;
}

void traverse_find(Node* node, string wildcard_id, vector<string> &result) {
  if(node == nullptr)
    return;
  list<Node*>::iterator it;
  for (it = node->children.begin(); it != node->children.end(); ++it) {
    if((*it)->id_generation.size() > 0) {
      
      if(wildcmp( wildcard_id.c_str(), (*it)->id_generation.back()->_id.c_str() ) 
         && !(*it)->id_generation.back()->_deleted 
         && (*it)->id_generation.back() != nullptr
         && current_account->_id != (*it)->id_generation.back()->_id)
      {
         result.push_back( (*it)->id_generation.back()->_id );
      }
    }
    traverse_find((*it), wildcard_id, result);
  }
}

int wildcmp(const char *wild, const char *string) {
  // Written by Jack Handy
  // ref: http://www.codeproject.com/Articles/1088/Wildcard-string-compare-globbing
  const char *cp = NULL, *mp = NULL;

  while ((*string) && (*wild != '*')) {
    if ((*wild != *string) && (*wild != '?')) {
      return 0;
    }
    wild++;
    string++;
  }

  while (*string) {
    if (*wild == '*') {
      if (!*++wild) {
        return 1;
      }
      mp = wild;
      cp = string+1;
    } else if ((*wild == *string) || (*wild == '?')) {
      wild++;
      string++;
    } else {
      wild = mp;
      string = cp++;
    }
  }

  while (*wild == '*') {
    wild++;
  }
  return !*wild;
}

void recommend_unused(string id) { //create

  const string input_id = id;
  string recommend_id = id;
  //char last = recommend_id[ recommend_id.size() - 1 ];
  recommend_id.pop_back();
  Node *parent;

  if(recommend_id.size() != 0)
    parent = find_node( recommend_id );
  else
    parent = ROOT;
  vector < string > result;

  char recommend_last_ch = '0';

  if (parent == nullptr) {     //this can be a check to push_back()
    //printf("not defined yet");
    return;
  }

  if (find_id( recommend_id ) == nullptr)
  { 
    if(parent != ROOT)
    result.push_back(recommend_id);
  }

  while (result.size() < 10) {

    bool not_exist = (  parent->find_child(recommend_last_ch) == nullptr ||
                        parent->find_child(recommend_last_ch)->id_generation.size() == 0 ||
                        parent->find_child(recommend_last_ch)->id_generation.back() == nullptr ||
                        parent->find_child(recommend_last_ch)->id_generation.back()->_deleted );
    
    //bool not_exist = (find_id(recommend_id + recommend_last_ch) == nullptr);
    if ( not_exist ) {
      result.push_back(recommend_id + recommend_last_ch );
    }

    if (recommend_id + recommend_last_ch == input_id && input_id.size() < 100)
    {
      Node *self = find_node(input_id);
      char child_last_ch = '0';

      if(self != nullptr) {
        while(result.size() < 10)
        {
          not_exist = ( self->find_child(child_last_ch) == nullptr ||
                        self->find_child(child_last_ch)->id_generation.size() == 0 ||
                        self->find_child(child_last_ch)->id_generation.back() == nullptr ||
                        self->find_child(child_last_ch)->id_generation.back()->_deleted );
          //not_exist = (find_id(input_id + child_last_ch) == nullptr);
          if( not_exist ) {
            result.push_back(input_id + child_last_ch );
          }
          child_last_ch += 1;
          if (child_last_ch == '9' + 1)
            child_last_ch = 'A';
          else if (child_last_ch == 'Z' + 1)
            child_last_ch = 'a';
          else if(child_last_ch == 'z' + 1) {
              //not defined yet
          }
        }
      }
    }
    recommend_last_ch += 1;

    if (recommend_last_ch == '9' + 1)
      recommend_last_ch = 'A';
    else if (recommend_last_ch == 'Z' + 1)
      recommend_last_ch = 'a';
    else if(recommend_last_ch == 'z' + 1) {
      break;
    }     
  }

  for(int i = 0; i < 10; i++)
  {
    printf("%s",result[i].c_str());
    if(i != 9)
      printf(",");
  }
  return;

}


void recommend(string id) {

  //Node* node = find_node(id);
  //if(node == nullptr) //possible
  Suggestion result;
  traverse_transfer(ROOT, result, id);
  int _size = result.size();
  for(int i = 0; i < min(10, _size); i++) {
    printf("%s", result.top().first.c_str());
    if(i != min( 9 ,_size - 1))
      printf(",");
    result.pop();
  }
  return;
}
void traverse_transfer(Node* node, Suggestion &result, string id) {
  if(node == nullptr)
    return;
  list<Node*>::iterator it;
  for (it = node->children.begin(); it != node->children.end(); ++it) {
    if((*it)->id_generation.size() > 0) {
      
      string tmp = (*it)->id_generation.back()->_id;
      if(  !(*it)->id_generation.back()->_deleted 
         && (*it)->id_generation.back() != nullptr
         && id != tmp)
      {
         result.push( make_pair( tmp, score(tmp, id) ) );
      }
    }
    traverse_transfer((*it), result, id);
  }
}
int score(string u, string v) {
  if(u.size() == 0 || v.size() == 0) return 0;
  if(u.size() < v.size())
    swap(u, v);

  int first = (u.size() - v.size()) * (u.size() - v.size() + 1) / 2, second = 0, len = v.size();

  for( int i = 0; i < len; i++ )
    if(u[i] != v[i])
      second += len - i;

  return first + second;
}
