#ifndef COMMENT_H
#define COMMENT_H
#define SPACE ' '
#define QUERY "?"
#define EMPTEY_STRING ""
#define NONE '\0'
#define MAX_POINT 10
#define MIN_POINT 0
#define DOT '.'

#include <iostream>
#include <string>
#include <vector>
#include "customer.h"

class customer;

using namespace std;

class comment
{
public:
  comment(int _film_id, string _content, int _ID, customer* _owner)
  : film_id(_film_id), content(_content), ID(_ID), owner(_owner) {}
  int get_ID() { return ID; }
  void set_reply(string _content);
  string get_content() { return content; }
  void show_replies(int id);
  customer* get_owner() { return owner; }
private:
  int film_id;
  string content;
  vector<string> replies;
  customer* owner; 
  int ID;
};

#endif