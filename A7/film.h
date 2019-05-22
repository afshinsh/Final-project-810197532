#ifndef FILM_H
#define FILM_H

#include "customer.h"
#include "comment.h"

class customer;

class comment;

using namespace std;

class film
{
public:
  film(string _name, string _year, string _price, string _length,
  string _summary, string _director, int _ID_counter_film, customer* _publisher);
  void set_name(string _name) { name = _name; }
  void set_summary(string _summary) { summary = _summary; }
  void set_year(string _year) { year = _year; }
  void set_price(string _price) { price = _price; }
  void set_length(string _length) { length = _length; }
  void set_director(string _director) { director = _director; }
  void set_delete() { deleted = true; }
  int get_ID() { return ID; }
  int get_price() { return stoi(price); }
  string get_name() { return name; }
  string get_year() { return year; }
  int get_CM_ID_counter() { return ID_counter_comment; }
  string get_summary() { return summary; }
  string get_length() { return length; }
  string get_director() { return director; }
  comment* get_comment(int comment_id) { return comments[comment_id - 1]; }
  string get_price_s() { return price; } 
  bool get_deleted() { return deleted; }
  void set_owner(customer* new_custormer);
  customer* get_publisher() { return publisher; }
  double give_avrage_rate();
  int get_unpaid_money() { return unpaid_money; }
  void increase_unpaid_money() { unpaid_money++; }
  void decrease_unpaid_mpney() { unpaid_money--; }
  void reset_unpaid_money() { unpaid_money = 0; }
  void set_comment(int film_id, string content, customer* owner);
  void reply_cm(int comment_id, string content);
  void show_comment();
  void delete_comment(int comment_id);
  void set_rated() { is_rated = true; }
private:
  string name;
  string year;
  string summary;
  string price;
  string length;
  string director;
  customer* publisher;
  vector<customer*> owners;
  int unpaid_money = 0;
  bool is_rated = false;
  vector<comment*> comments;
  int ID_counter_comment = 1;
  bool deleted = false;
  int ID;
};

#endif