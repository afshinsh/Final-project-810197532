#ifndef PUBLISHER_H
#define PUBLISHER_H

#include "customer.h"

using namespace std;

class publisher : public customer
{
public:
  publisher(string _email, string _username, string _password
, string _age, int _ID_counter, bool _publisher);
  void regist_new_film_with_notif(film* new_film);
  void get_copy_published_films(vector<film*> &search_result);
  void edit_films(string name, string year, string price
, string summary, string length, string director, int film_id);
  void delete_film(int film_id);
  void show_followers();
  void set_followers(customer* new_follower);
  void set_notif_for_follow(customer* follower);
private:
  vector<film*> my_films;
  vector<customer*> followers;
};


#endif