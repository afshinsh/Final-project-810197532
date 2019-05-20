#ifndef CUSTOMER_H
#define CUSTOMER_H


#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <exception>
#include <string>
#include <vector>
#include <map>
#include "exceptions.h"
#include "film.h"

using namespace std;

class film;


class customer
{
public:
  customer(string _email, string _username, string _password
, string _age, int _ID_counter, bool _publisher);
  bool get_publisher() { return publish; }
  string get_username() { return username; }
  string get_password() { return password; }
  string get_email() { return email; }
  void get_copy_bought_film(vector<film*> &search_result);
  int get_ID() { return ID; }
  virtual void regist_new_film_with_notif(film* new_film) { return; }
  virtual void edit_films(string name, string year, string price
, string summary, string length, string director, int film_id) { return; }
  virtual void delete_film(int film_id) { return; }
  virtual void show_followers() { return; }
  virtual void get_copy_published_films(vector<film*> &search_result)
  { return; };
  void follow_publisher(customer* new_publisher);
  virtual void set_followers(customer* new_follower) { return; }
  void increase_money(int amount) { money += amount; }
  void buy_film(film* new_film);
  bool check_is_not_bought(film* r_film);
  void check_bought_film(film* bought_film);
  void score_watched_film(int film_id, int score);
  double get_rate(int film_id) { return scores[film_id]; }
  void add_to_unread_notif(string notif) { unread_notif.push_back(notif); }
  void clear_unread_notif() { unread_notif.clear(); }
  void set_notif_for_follow(customer* follower) { return; }
protected:
  string email;
  string username;
  string password;
  int ID;
  string age;
  bool publish;
  vector<customer*> followed_publishers;
  vector<film*> bought_films;
  map <int,int> scores;
  vector<string> unread_notif;
  vector<string> read_notif;
  int money = 0;
};



#endif