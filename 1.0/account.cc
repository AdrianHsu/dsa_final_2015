#include "account.h"

bool cmp(const Entry &a, const Entry &b) {
  return a._time_stamp <= b._time_stamp;
}

void Content::merge_content(Content *cont) {
  history.merge(cont->history, cmp);

  return ;
}

void Account::enroll(string id, bool fot, int value, int time_stamp) {
  string hash_id = id + "-";
  if (archive.size() == 0) {
    archive.push_back(new Content(hash_id));
    archive[0]->history.push_back(Entry(fot, value, time_stamp));

    return ;
  }

  int l = 0, r = archive.size();
  int mid = (l + r) / 2;

  while (r - l > 1) {
    string now = archive[mid]->related_id;
    int buf = strcmp(now.c_str(), hash_id.c_str());
    
    if (buf == 0) {
      archive[mid]->history.push_back(Entry(fot, value, time_stamp));

      return ;
    }

    if (buf < 0)
      l = mid;
    else
      r = mid;

    mid = (l + r) / 2;
  }

  if (archive[mid]->related_id == hash_id) {
    archive[mid]->history.push_back(Entry(fot, value, time_stamp));

    return ;
  }

  if (mid == 0) {
    string now = archive[mid]->related_id;
    int buf = strcmp(now.c_str(), hash_id.c_str());

    if (buf < 0)
      mid++;
  } else {
    mid++;
  }
  archive.insert(archive.begin() + mid, new Content(hash_id));
  archive[mid]->history.push_back(Entry(fot, value, time_stamp));

  return ;
}

bool Account::show_history(string id, Content &show) {
  if (archive.size() == 0) {
    return false;
  }

  int l = 0, r = archive.size();
  int mid = (l + r) / 2;

  bool is_found = false;
  while (r - l > 1) {
    string now = archive[mid]->related_id;
    int buf = strcmp(now.c_str(), id.c_str());
    
    if (buf == 0) {
      is_found = true;
      break;
    }

    if (buf < 0)
      l = mid;
    else
      r = mid;

    mid = (l + r) / 2;
  }

  if (archive[mid]->related_id == id)
    is_found = true;

  if (archive.size() == 1 && archive[0]->related_id == id)
    is_found = true;

  if (is_found == false)
    return false;

  Content *buf = new Content(archive[mid]->related_id);
  list<Entry>::iterator it;
  for (it = archive[mid]->history.begin(); it != archive[mid]->history.end(); ++it) {
    buf->history.push_back((*it));
  }
  show.merge_content(buf);

  // list<Entry>::iterator it;
  // for (it = archive[mid]->history.begin(); it != archive[mid]->history.end(); ++it) {

  //   if ((*it)._fot)
  //     printf("To ");
  //   else
  //     printf("From ");

  //   printf("%s %d %d\n", archive[mid]->real_id.c_str(), (*it)._value, (*it)._time_stamp);
  // }

  return true;
}
