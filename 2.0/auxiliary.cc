#include "auxiliary.h"

extern unordered_map<string, int> Accounts;
extern vector<Node*> Database;
extern Node *current_account;
extern vector<Edge> Edges;
extern char PostFix[];

void create_recommend(string id) {
	vector<string> result;
	string truncate_id = id.substr(0, id.size()-1);

	//suggest length-1 of truncate_id
	if (truncate_id != ""){
		unordered_map<string, int>::iterator it = Accounts.find(truncate_id);
		if (it == Accounts.end() || it->second == -1)
			result.push_back(truncate_id);
	}
	//the same length as id && length +1
/*	unordered_map<string, int>::iterator it2 = Accounts.find(id);
	while (it2->first.substr(0, it2->first.size()-1) == id.substr(0, id.size()-1)) {
		it2--;
	}
	++it2;

	while (result.size() < 10 && it2->first.size() <= id.size()+1) {
		for (int i = 0; i < 62; i++) {
			if (PostFix[i] != it2->first[id.size()-1]){
				result.push_back(truncate_id + PostFix[i]);
			}
		}
		it2++;
	}
*/
	for (int i = 0; i < 62; i++) {
		if (Accounts.find(truncate_id + PostFix[i]) == Accounts.end() || Accounts.find(truncate_id+PostFix[i])->second == -1) 
		{
			result.push_back(truncate_id + PostFix[i]);
		
		} else if (truncate_id + PostFix[i] == id) {
			for (int j = 0; j < 62; j++) {
				if (Accounts.find(id + PostFix[j]) == Accounts.end() || Accounts.find(id+PostFix[j])->second == -1)
					result.push_back(id + PostFix[j]);
			}
		}
	}

			


	for (int i = 0; i < min(int(result.size()), 10); i++) {	//theratically min should be 10
		printf("%s",result[i].c_str());
    if(i != min(9, int(result.size())-1))
      printf(",");
	}

}

void merge_id(int id1, int id2)		//2 to 1
{   
	Database[id1]->balance += Database[id2]->balance;
    
	// Edge changing 
    for (int i = 0; i < Database[id2]->edges.size(); ++i) {
    //for (int index : Database[id2]->edges.size()) {
    	int index = Database[id2]->edges[i];
    	if (Edges[index].starting == id2)
   		{
   			Edges[index].starting = id1;
   			
   			if (Edges[index].ending != id1)
   				Database[id1]->edges.push_back(index);
   			else
   				Edges[index].change = 1;		//print FROM first
    	} 
    	else if (Edges[index].ending == id2)
    	{
    		Edges[index].ending = id1;
   			
    		if (Edges[index].starting != id1)
    			Database[id1]->edges.push_back(index);
   			else
   				Edges[index].change = -1;		//print TO first
    	}

    }

    Database[id2]->isDeleted = true;
    Accounts[Database[id2]->id] = -1;
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


void transfer_recommend(string id) {
	unordered_map<string, int>::iterator it = Accounts.begin();
	string result[10] = {"", "", "", "", "", "", "", "", "", ""};
	
	while (it != Accounts.end())
	{
		if (it->second == -1) {
			++it;
			continue;
		}		

		for (int i = 0; i < 10; i++) {
			if (result[i] == "") {
				result[i] = it->first;
				break;
			} else if (score(result[i], id) > score(it->first, id)) {	//what to do with i=9
				if (i == 9) {
					result[i] = it->first;
					break;
				} else {
					for (int j = 8; j >= i; j--)
						result[j+1] = result[j];
					result[i] = it->first;
					break;
				}
			} else if (score(result[i], id) == score(it->first, id)) {
				if (strcmp(result[i].c_str(), it->first.c_str()) >= 0) {
					if (i == 9) {
						result[i] = it->first;
						break;
					} else {
						for (int j = 8; j >= i; j--)
							result[j+1] = result[j];
						result[i] = it->first;
						break;
					}
				}
			}
		}

		++it;
	}

	for (int i = 0; i < 10; i++) {
		if (result[i] == "")
			break;
		else if (i == 0)
			printf("%s", result[i].c_str());
		else printf(",%s", result[i].c_str());
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
