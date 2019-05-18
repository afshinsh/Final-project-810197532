#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#define SPACE ' '
#define QUERY "?"
#define EMPTEY_STRING ""
#define NONE '\0'
#define MAX_POINT 10.0
#define MIN_POINT 0.0

using namespace std;

class film;

class comment;

class BadRequest {
public:
  void what()
  {
    cout<<"BadRequest"<<endl;
  }
};

class PermissionDenied {
public:
  void what()
  {
    cout<<"Permission Denied"<<endl;
  }
};

class NotFound {
public:
  void what()
  {
    cout<<"Not Found"<<endl;
  }
};

class customer
{
public:
  customer(string _email, string _username, string _password
, string _age, int _ID_counter, bool _publisher);
  bool get_publisher() { return publish; }
  string get_username() { return username; }
  string get_password() { return password; }
  string get_email() { return email; }
  int get_ID() { return ID; }
  virtual void regist_new_film(film* new_film) { return; }
  virtual void edit_films(string name, string year, string price
, string summary, string length, string director, int film_id) { return; }
  virtual void delete_film(int film_id) { return; }
  virtual void show_followers() { return; }
  void follow_publisher(customer* new_publisher);
  virtual void set_followers(customer* new_follower) { return; }
  void increase_money(int amount) { money += amount; }
  void buy_film(film* new_film);
  bool check_is_not_bought(film* r_film);
  void check_bought_film(film* bought_film);
  void score_watched_film(int film_id, int score);
  double get_rate(int film_id) { return scores[film_id]; }
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
  int money = 0;
};

customer::customer(string _email, string _username, string _password
, string _age, int _ID_counter, bool _publisher) : email(_email), 
username(_username), password(_password), age(_age), ID(_ID_counter),
publish(_publisher) {}

class publisher : public customer
{
public:
  publisher(string _email, string _username, string _password
, string _age, int _ID_counter, bool _publisher);
  void regist_new_film(film* new_film);
  void edit_films(string name, string year, string price
, string summary, string length, string director, int film_id);
void delete_film(int film_id);
void show_followers();
void set_followers(customer* new_follower);
private:
  vector<film*> my_films;
  vector<customer*> followers;
};

publisher::publisher(string _email, string _username, string _password
, string _age, int _ID_counter, bool _publisher) : customer(_email, 
_username, _password, _age, _ID_counter, _publisher) {}

void publisher::regist_new_film(film* new_film)
{
  my_films.push_back(new_film);
}

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
  string get_summary() { return summary; }
  string get_length() { return length; }
  string get_director() { return director; }
  string get_price_s() { return price; } 
  bool get_deleted() { return deleted; }
  void set_owner(customer* new_custormer);
  customer* get_publisher() { return publisher; }
  double give_avrage_rate();
  void increase_unpaid_money() { unpaid_money++; }
  void reset_unpaid_money() { unpaid_money = 0; }
  void set_comment(int film_id, string content);
  void reply_cm(int comment_id, string content);
  void show_comment();
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
  vector<comment*> comments;
  int ID_counter_comment = 1;
  bool deleted = false;
  int ID;
};

film::film(string _name, string _year, string _price, string _length
, string _summary , string _director, int _ID_counter_film, customer* _publisher)
 : name(_name), year(_year), price(_price), length(_length), summary(_summary)
 , director(_director), ID(_ID_counter_film), publisher(_publisher) {}

class comment
{
public:
  comment(int _film_id, string _content, int _ID) : film_id(_film_id), 
  content(_content), ID(_ID) {}
  int get_ID() { return ID; }
  void set_reply(string _content);
  string get_content() { return content; }
  void show_replies();
private:
  int film_id;
  string content;
  vector<string> replies; 
  int ID;
};

class manager
{
public:
  manager();
  void process_users();
  void process_begin_of_command();
  void skip_space();
  void set_first_part();
  void set_second_part();
  void process_command();
  void process_GET_command();
  void process_POST_command();
  void check_DELETE_second_part();
  void process_command_buy(string &film_id);
  void POST_film();
  void POST_signup();
  void process_command_money(string &amount);
  void POST_money();
  void process_DELETE_command();
  void process_PUT_command();
  void GET_followers();
  void check_inputs_for_rate(string score, string film_id);
  void POST_followers();
  void DELETE_film();
  void set_recommendation(film* f);
  void charge_account();
  void process_commamd_comments(string &film_id, string &content);
  void POST_rate();
  void process_command_delete(string &film_id);
  void process_command_signup(string &email, string &username
, string &password, string &age, string &publisher);
  void check_command_for_PUT(string &name, string &year, string &price
  , string &summary, string &length, string &director, string &film_id);
  void initialize_film(string name, string year, string length
  , string price, string summary, string director);
  void process_command_followers(string &user_id);
  void check_GET_second_part();
  void set_info(string &info);
  void show_head_followers();
  void initialize_user(string &email, string &username
  , string &password, string &age, string &publisher);
  void find_user(string username, string password);
  void POST_login();
  void process_command_rate(string &film_id,string &score);
  void reset();
  void show_details(film* Film);
  void process_command_login(string &username, string &password);
  void POST_replies();
  void show_features(film* Film);
  void show_comments(film* Film);
  void show_recommendation(film* Film);
  void check_is_not_integer(string s);
  void regist_film();
  void set_command(string _command) ;
  void check_POST_second_part();
  void check_first_part();
  string achieve_part();
  void check_PUT_second_part();
  void PUT_film();
  void check_user_for_command();
  void catch_money();
  void pay_money(double percent, int i);
  void check_repeated_username(string username);
  void process_command_replies(string &film_id, string &comment_id
  , string &content);
  void GET_films();
  void POST_buy();
  void POST_comments();
  void check_for_buy(string film_id);
private:
  vector<film*> films;
  vector<customer*> users;
  vector<customer*> customers;
  vector<customer*> publishers;
  vector<film*> recommendation_films;
  string command;
  int command_chars_counter = 0;
  int ID_counter = 1;
  int ID_counter_film = 1;
  customer* current_user = NULL;
  string first_part = EMPTEY_STRING ;
  string second_part = EMPTEY_STRING;
  string sentence_part;
  int property = 0;
};

manager::manager()
{
  customer* root = new customer("root", "root", "root", "root", 0, false);
  users.push_back(root);
  customers.push_back(root);
  publishers.push_back(root);
  film* root_f = new film("root", "root", "root", "root", "root", "root", 0, NULL);
  root_f->set_delete();
  films.push_back(root_f);
}

void manager::set_command(string _command)
{
  command = _command;
}

void manager::check_repeated_username(string username)
{
  for(int i = 0; i < users.size(); i++)
  {
    if(username == users[i]->get_username())
      throw BadRequest();
  }
}

void manager::reset()
{
  first_part = EMPTEY_STRING;
  second_part = EMPTEY_STRING;
  command_chars_counter = 0;
}

void manager::set_info(string &info)
{
  sentence_part = achieve_part();
  info = sentence_part;
}


void manager::check_is_not_integer(string s)
{
  for(int i = 0;i < s.length();i++)
    if(!isdigit(s[i]))
      return true;
  return false;
}

void film::set_owner(customer* new_custormer)
{
  owners.push_back(new_custormer);
}

void manager::initialize_user(string &email, string &username
, string &password, string &age, string &publish)
{
  if(publish == EMPTEY_STRING || publish == "false")
  {
    current_user = new customer(email, username, password, age, ID_counter, false);
    ID_counter++;
    users.push_back(current_user);
    customers.push_back(current_user);
  }
  else if(publish == "true")
  {
    current_user = new publisher(email, username, password, age, ID_counter, true);
    ID_counter++;
    users.push_back(current_user);
    publishers.push_back(current_user);
  }
}

void manager::find_user(string username, string password)
{
  customer* previous_user = current_user;
  current_user = NULL;
  for(int i = 0;i < users.size();i++)
  {
    if(users[i]->get_username() == username 
    && users[i]->get_password() == password)
      current_user = users[i];
  }
  if(current_user == NULL)
  {
    current_user = previous_user;
    throw BadRequest();
  }
  previous_user = NULL;
}

void manger::process_command_login(string &username, string &password)
{
  while(true)
  {
    sentence_part = achieve_part();
    if(sentence_part == EMPTEY_STRING)
      break;
    else if(sentence_part == "password")
      password = achieve_part();
    else if(sentence_part == "username")
      username = achieve_part();
    else 
      throw BadRequest();
  }
}

void manager::POST_login()
{
  if(second_part == "login")
  {
    if(achieve_part() != QUERY)
      throw BadRequest();
    string username, password;
    process_command_login(username, password);
    find_user(username, password);
    cout<<"OK"<<endl;
  }
}

void manager::process_command_signup(string &email, string &username
, string &password, string &age, string &publisher)
{
  while(true)
  {
    sentence_part = achieve_part();
    if(sentence_part == EMPTEY_STRING)
      break;
    else if(sentence_part == "email")
      set_info(email);
    else if(sentence_part == "username")
      set_info(username);
    else if(sentence_part == "password")
      set_info(password);
    else if(sentence_part == "age")
      set_info(age);
    else if(sentence_part == "publisher")
      set_info(publisher);
    else 
      throw BadRequest();
  }
}

void manager::POST_signup()
{
  if(second_part == "signup" )
  {
    if(achieve_part() != QUERY)
      throw BadRequest();
    string email, username, password, age, publisher = EMPTEY_STRING;
    process_command_signup(email, username, password, age, publisher);
    check_repeated_username(username);
    if(check_is_not_integer(age))
      throw BadRequest();
    initialize_user(email, username, password, age, publisher);
    cout<<"OK"<<endl;
  }
}

void manager::initialize_film(string name, string year, string length
, string price, string summary, string director)
{
  if(check_is_not_integer(year) || check_is_not_integer(length) || check_is_not_integer(price))
    throw BadRequest();
  film* new_film = new film(name, year, price, length, summary, 
  director, ID_counter_film, current_user);
  films.push_back(new_film);
  current_user->regist_new_film(new_film);
  ID_counter_film++;
}

void manager::regist_film()
{
  string name, year, price, summary, length, director;
  while(true)
  {
    sentence_part = achieve_part();
    if(sentence_part == EMPTEY_STRING)
      break;
    else if(sentence_part == "name")
      set_info(name);
    else if(sentence_part == "year")
      set_info(year);
    else if(sentence_part == "length")
      set_info(length);
    else if(sentence_part == "price")
      set_info(price);
    else if(sentence_part == "summary")
      set_info(summary);
    else if(sentence_part == "director")
      set_info(director);
    else 
      throw BadRequest();
  }
  initialize_film(name, year, length, price, summary, director);
}



void manager::POST_film()
{
  if(second_part == "films")
  {
    if(achieve_part() != QUERY)
      throw BadRequest();
    if(!current_user->get_publisher() || current_user == NULL)
      throw PermissionDenied();
    regist_film();
    cout<<"OK"<<endl;
  }
}

void manager::check_POST_second_part()
{
  if(second_part != "signup" && second_part != "login" && second_part != "films" &&
  second_part != "money" && second_part != "replies" && second_part != "followers"
  && second_part != "buy" && second_part != "rate" && second_part != "comments")
    throw NotFound();
}

void manger::process_command_followers(string &user_id)
{
  while(true)
  {
    sentence_part = achieve_part();
    if(sentence_part == EMPTEY_STRING)
      break;
    else if(sentence_part == "user_id")
      user_id = achieve_part();
    else
      throw BadRequest();
  }
}

void manager::POST_followers()
{
  if(second_part == "followers")
  {
    if(achieve_part() != QUERY)
      throw BadRequest();
    string user_id;
    process_command_followers(user_id);
    if(check_is_not_integer(user_id))
      throw BadRequest();
    if(user_id >= ID_counter)
      throw NotFound();
    customer* followed_publisher = users[stoi(user_id)];
    current_user->follow_publisher(followed_publisher);
    followed_publisher->set_followers(current_user);
    cout<<"OK"<<endl;
  }
}

void publisher::set_followers(customer* new_follower)
{
  for(int i = 0;i < followers.size(); i++)
  {
    if(new_follower->get_ID() == followers[i]->get_ID())
      return;
  }
  followers.push_back(new_follower);
}

void managr::process_command_money(string &amount)
{
  while(true)
  {
    sentence_part = achieve_part();
    if(sentence_part == EMPTEY_STRING)
      break;
    else if(sentence_part == "amount")
      amount = achieve_part();
    else 
      throw BadRequest();
  }
}
void manager::charge_account()
{
  string amount;
  process_command_money(amount);
  if(check_is_not_integer(amount))
    throw BadRequest();
  current_user->increase_money(stoi(amount));
  cout<<"OK"<<endl;
}

void manager::pay_money(double percent, int i)
{
  current_user->increase_money(percent * films[i]->get_price());
  property -= percent * films[i]->get_price();
  cout<<"OK"<<endl;
}

void manager::catch_money()
{
  if(!current_user->get_publisher())
    throw PermissionDenied();
  for(int i = 0;i < films.size();i++)
    if(films[i]->get_publisher() == current_user)
    {
      if(films[i]->give_avrage_rate() >= 8)
        pay_money(0.95, i);
      else if(films[i]->give_avrage_rate() >= 5 && films[i]->give_avrage_rate() < 8)
        pay_money(0.90, i);
      else if(films[i]->give_avrage_rate() >=0 && films[i]->give_avrage_rate() < 5)
        pay_money(0.80, i);
    }
}

void manager::POST_money()
{
  if(second_part == "money")
  {
    string sentence_part = achieve_part();
    if(sentence_part == QUERY)
      charge_account();
    else if(sentence_part == EMPTEY_STRING)
      catch_money();
    else 
      throw BadRequest();
  }
}

void customer::buy_film(film* new_film)
{
  film * bought_film = new film(new_film->get_name() , new_film->get_year()
  , new_film->get_price_s(), new_film->get_length(), new_film->get_summary(), 
  new_film->get_director(), new_film->get_ID(), new_film->get_publisher());
  bought_films.push_back(bought_film);
  new_film->increase_unpaid_money();
  if(money >= new_film->get_price())
    money -= new_film->get_price();
  else
    throw BadRequest();
}

void manager::check_for_buy(string film_id)
{
  if(check_is_not_integer(film_id))
    throw BadRequest();
  if(stoi(film_id) >= ID_counter_film)
    throw NotFound();
  if(films[stoi(film_id)]->get_deleted())
    throw NotFound();
}

void manager::process_command_buy(string &film_id)
{
  while(true)
  {
    sentence_part = achieve_part();
    if(sentence_part == EMPTEY_STRING)
      break;
    else if(sentence_part == "film_id")
      film_id = achieve_part();
    else 
      throw BadRequest();
  }
} 

void manager::POST_buy()
{
  if(second_part == "buy")
  {
    if(achieve_part() != QUERY)
      throw BadRequest();
    string film_id;
    process_command_buy(film_id);
    check_for_buy(film_id);
    current_user->buy_film(films[stoi(film_id)]);
    films[stoi(film_id)]->set_owner(current_user);
    property +=  films[stoi(film_id)]->get_price();
    cout<<"OK"<<endl;
  }
}

double film::give_avrage_rate()
{
  double sum = 0;
  for(int i = 0;i < owners.size();i++)
    sum += owners[i]->get_rate(ID);
  return sum / owners.size();
}

void customer::score_watched_film(int film_id, int score)
{
  for(int i = 0; i < bought_films.size(); i++)
    if(bought_films[i]->get_ID() == film_id)
    {
      scores[film_id] = score;
      return;
    }
  throw BadRequest();
}

void manager::check_inputs_for_rate(string score, string film_id)
{
  if(check_is_not_integer(score) || check_is_not_integer(film_id))
    throw BadRequest();
  if(stoi(film_id) >= ID_counter_film)
    throw NotFound();
  if(stoi(score) < MIN_POINT || stoi(score) > MAX_POINT)
    throw BadRequest();
}

bool customer::check_is_not_bought(film* r_film)
{
  for(int i = 0;i < bought_films.size();i++)
    if(r_film->get_ID() == bought_films[i]->get_ID())
      return false;
  return true;
}

struct compare_rate
{
  inline bool operator() (film* f1, film* f2)
  {
    return (f1->give_avrage_rate() > f2->give_avrage_rate());
  }
};

void manager::set_recommendation(film* f)
{
  recommendation_films.clear();
  for(int i = 0;i < films.size();i++)
    if(current_user->check_is_not_bought(films[i]) && !films[i]->get_deleted()
     && films[i] != f)
      recommendation_films.push_back(films[i]);
  sort(recommendation_films.begin(), recommendation_films.end()
  , compare_rate()); 
}

void manager::process_command_rate(string &film_id,string &score)
{
  while(true)
  {
    sentence_part = achieve_part();
    if(sentence_part == EMPTEY_STRING)
      break;
    else if(sentence_part == "film_id")
      film_id = achieve_part();
    else if(sentence_part == "score")
      score = achieve_part();
    else 
      throw BadRequest();
  }
}

void manager::POST_rate()
{
  if(second_part == "rate")
  {
    if(achieve_part() != QUERY)
      throw BadRequest();
    string film_id, score;
    process_command_rate(film_id, score);
    check_inputs_for_rate(score, film_id);
    current_user->score_watched_film(stoi(film_id), stoi(score));
    cout<<"OK"<<endl;
  }
}

void film::set_comment(int film_id, string content)
{
  comment* new_comment = new comment(film_id, content, ID_counter_comment);
  comments.push_back(new_comment);
  ID_counter_comment++;
}

void customer::check_bought_film(film* bought_film)
{
  for(int i = 0;i < bought_films.size();i++)
    if(bought_film->get_ID() == bought_films[i]->get_ID())
      return;
  throw BadRequest();
}

void manager::process_commamd_comments(string &film_id, string &content)
{
  while(true)
  {
    sentence_part = achieve_part();
    if(sentence_part == EMPTEY_STRING)
      break;
    else if(sentence_part == "film_id")
      film_id = achieve_part();
    else if(sentence_part == "content")
      content = achieve_part();
    else 
      throw BadRequest();
  }
}

void manager::POST_comments()
{
  if(second_part == "comments")
  {
    if(achieve_part() != QUERY)
      throw BadRequest();
    string film_id, content;
    process_commamd_comments(film_id, content);
    if(check_is_not_integer(film_id))
      throw BadRequest();
    if(stoi(film_id) >= ID_counter_film)
      throw NotFound();
    current_user->check_bought_film(films[stoi(film_id)]);
    films[stoi(film_id)]->set_comment(stoi(film_id), content);
    cout<<"OK"<<endl;
  }
}

void comment::set_reply(string _content)
{
  replies.push_back(_content);
}

void film::reply_cm(int comment_id, string content)
{
  for(int i = 0;i < comments.size();i++)
    if(comments[i]->get_ID() == comment_id)
    {
      comments[i]->set_reply(content);
      return;
    }
      
  throw BadRequest();
}

void manager::process_command_replies(string &film_id, string &comment_id, string &content)
{
  while(true)
  {
    sentence_part = achieve_part();
    if(sentence_part == EMPTEY_STRING)
      break;
    else if(sentence_part == "film_id")
      film_id = achieve_part();
    else if(sentence_part == "comment_id")
      comment_id = achieve_part();
    else if(sentence_part == "content")
      content = achieve_part();
    else 
      throw BadRequest();
  }
}

void manager::POST_replies()
{
  if(second_part == "replies")
  {
    if(achieve_part() != QUERY)
      throw BadRequest();
    string film_id, comment_id, content;
    process_command_replies(film_id, comment_id, content);
    if(check_is_not_integer(film_id) || check_is_not_integer(comment_id))
      throw BadRequest();
    if(stoi(film_id) >= ID_counter_film)
      throw NotFound();
    if(films[stoi(film_id)]->get_publisher() != current_user)
      throw PermissionDenied();
    films[stoi(film_id)]->reply_cm(stoi(comment_id), content);
    cout<<"OK"<<endl;
  }
}

void manager::process_POST_command()
{
  check_POST_second_part();
  POST_signup();
  POST_login();
  POST_film();
  POST_followers();
  POST_money();
  POST_buy();
  POST_rate();
  POST_comments();
  POST_replies();
  //...check_NOT_found
}

void manager::set_first_part()
{
  skip_space();
  int begin_of_word = command_chars_counter;
  while(command[command_chars_counter] != SPACE && 
  command[command_chars_counter] != NONE)
    command_chars_counter++;
  first_part = command.substr(begin_of_word, 
  command_chars_counter - begin_of_word);
  check_first_part();
}

void manager::check_first_part()
{
  if(first_part != "GET" && first_part != "POST" && first_part != "PUT" 
  && first_part != "DELETE")
    throw BadRequest();
}

void manager::check_PUT_second_part()
{
  if(second_part != "films")
    throw BadRequest();
}

void manager::check_command_for_PUT(string &name, string &year, string &price
, string &summary, string &length, string &director, string &film_id)
{
  while(true)
  {
    sentence_part = achieve_part();
    if(sentence_part == EMPTEY_STRING)
      break;
    else if(sentence_part == "film_id")
      set_info(film_id);
    else if(sentence_part == "name")
      set_info(name);
    else if(sentence_part == "year")
      set_info(year);
    else if(sentence_part == "length")
      set_info(length);
    else if(sentence_part == "price")
      set_info(price);
    else if(sentence_part == "summary")
      set_info(summary);
    else if(sentence_part == "director")
      set_info(director);
    else 
      throw BadRequest();
  }
}

void manager::PUT_film()
{
  if(second_part == "films")
  {
    if(achieve_part() != QUERY)
      throw BadRequest();
    if(!current_user->get_publisher())
      throw PermissionDenied()
    string name, year, price, summary, length, director, film_id;    
    check_command_for_PUT(name, year, price, summary, length, director, film_id);
    if(check_is_not_integer(year) || check_is_not_integer(length) || 
    check_is_not_integer(price) || check_is_not_integer(film_id))
      throw BadRequest();
    if(stoi(film_id) >= ID_counter_film)
      throw NotFound();
    current_user->edit_films(name, year, price, summary, length, director, stoi(film_id));
    cout<<"OK"<<endl;
  }
}

void publisher::edit_films(string name, string year, string price
, string summary, string length, string director, int film_id)
{
  for(int i = 0;i < my_films.size(); i++)
  {
    if(my_films[i] != NULL && my_films[i]->get_ID() == film_id)
    {
      if(name != EMPTEY_STRING)
        my_films[i]->set_name(name);
      if(year != EMPTEY_STRING)
        my_films[i]->set_year(year);
      if(price != EMPTEY_STRING)
        my_films[i]->set_price(price);
      if(summary != EMPTEY_STRING)
        my_films[i]->set_summary(summary);
      if(length != EMPTEY_STRING)
        my_films[i]->set_length(length);
      if(director != EMPTEY_STRING)
        my_films[i]->set_director(director);
      return;
    }
  }
  throw PermissionDenied();
}

void customer::follow_publisher(customer* new_publisher)
{
  if(!new_publisher->get_publisher())
    throw BadRequest();
  for(int i = 0;i < followed_publishers.size(); i++)
  {
    if(new_publisher->get_ID() == followed_publishers[i]->get_ID())
      return;
  }
  followed_publishers.push_back(new_publisher);
}

void publisher::show_followers()
{
  for(int i = 0;i < followers.size();i++)
    cout<<i + 1<<". "<<followers[i]->get_ID()<<" | "<<
    followers[i]->get_username()<<" | "<<followers[i]->get_email()<<endl;
}

void manager::check_DELETE_second_part()
{
  if(second_part != "films")
    throw BadRequest();
}

void manager::process_PUT_command()
{
  check_PUT_second_part();
  PUT_film();
}

void publisher::delete_film(int film_id)
{
  for(int i = 0 ;i < my_films.size(); i++)
  {
    if(my_films[i] != NULL && my_films[i]->get_ID() == film_id)
    {
      my_films[i] = NULL;
      return;
    }
  }
  throw PermissionDenied();
}

void manger::process_command_delete(string &film_id)
{
  while(true)
  {
    sentence_part = achieve_part();
    if(sentence_part == EMPTEY_STRING)
      break;
    else if(sentence_part == "film_id")
      set_info(film_id);
    else 
      throw BadRequest();
  }
}

void manager::DELETE_film()
{
  if(achieve_part() != QUERY)
    throw BadRequest();
  string film_id;
  process_command_delete(film_id);
  if(check_is_not_integer(film_id) || stoi(film_id) >= ID_counter_film)
    throw BadRequest();
  if(current_user != films[stoi(film_id)]->get_publisher())
    throw PermissionDenied();
  current_user->delete_film(stoi(film_id));
  films[stoi(film_id)]->set_delete();
  cout<<"OK"<<endl;
}

void manager::process_DELETE_command()
{
  check_DELETE_second_part();
  DELETE_film();
}

void manager::show_head_followers()
{
  cout<<"List of Followers"<<endl;
  cout<<"#. User Id | User Username | User Email"<<endl;
}

void manager::GET_followers()
{
  if(second_part == "followers")
  {
    show_head_followers();
    current_user->show_followers();
  }
}

void manager::check_GET_second_part()
{
  if(second_part != "followers" && second_part != "published" 
  && second_part != "films"&& second_part != "purchased"&& 
  second_part != "notifications")
    throw BadRequest();
}

/*void publisher

void manager::show_head_published()
{
  cout<<"#. Film Id | film Length | Film Price | Rate | 
  Production Year | Film Director"<<endl;
}

void manager::GET_published()
{
  if(second_part == "published")
  {
    show_head_published();
    current_user->show_published_film();
  }
}
*/
void manager::show_features(film* Film)
{
  cout<<"Details of Film "<<Film->get_name()<<endl;
  cout<<"Id = "<<Film->get_ID()<<endl;
  cout<<"Director = "<<Film->get_director()<<endl;
  cout<<"Length = "<<Film->get_length()<<endl;
  cout<<"Year = "<<Film->get_year()<<endl;
  cout<<"Summary = "<<Film->get_summary()<<endl;
  cout<<"Rate = "<<Film->give_avrage_rate()<<endl;
  cout<<"Price = "<<Film->get_price()<<endl;
}

void manager::show_comments(film* Film)
{
  cout<<endl<<endl<<"Comments"<<endl;
  Film->show_comment();
}

void comment::show_replies()
{
  for(int i = 0;i < replies.size();i++)
    cout<<ID<<"."<<i + 1<<". "<<replies[i]<<endl;
}

void film::show_comment()
{
  for(int i = 0;i < comments.size();i++)
  {
    cout<<comments[i]->get_ID()<<". "<<comments[i]->get_content()<<endl;
    comments[i]->show_replies();
  }
}

void manager::show_recommendation(film* Film)
{
  cout<<endl<<endl<<"Recommendation Film"<<endl;
  cout<<"#. Film Id | Film Name | Film Length | Film Director"<<endl;
  for(int i = 0;i < recommendation_films.size() && i < 4;i++)
    cout<<i + 1<<". "<<recommendation_films[i]->get_ID()<<" | "<<
    recommendation_films[i]->get_name()<<" | "<<recommendation_films[i]->get_length()
    <<" | "<<recommendation_films[i]->get_director()<<endl;
}

void manager::show_details(film* Film)
{
  show_features(Film);
  show_comments(Film);
  show_recommendation(Film);
}

void manager::GET_films()
{
  if(second_part == "films" )
  {
    if(achieve_part() != QUERY)
      throw BadRequest();
    string film_id; 
    if(achieve_part() == "film_id")
      film_id = achieve_part();
    else
      throw BadRequest();
    if(check_is_not_integer(film_id) || stoi(film_id) >= ID_counter_film))
      throw BadRequest();
    set_recommendation(films[stoi(film_id)]);
    show_details(films[stoi(film_id)]);
  }
}

void manager::process_GET_command()
{
  check_GET_second_part();
  GET_followers();
  GET_films();
  //GET_published();
}

void manager::process_command()
{
  if(first_part == "POST")
    process_POST_command();
  else if(first_part == "GET")
    process_GET_command();
  else if(first_part == "PUT")
    process_PUT_command();
  else if(first_part == "DELETE")
    process_DELETE_command();
  else
    throw BadRequest();
}

void manager::skip_space()
{
  while(command[command_chars_counter] == SPACE)
    command_chars_counter++;
}

void manager::set_second_part()
{
  skip_space();
  int begin_of_word = command_chars_counter;
  while(command[command_chars_counter] != SPACE &&
   command[command_chars_counter] != NONE)
    command_chars_counter++;
  second_part = command.substr(begin_of_word, 
  command_chars_counter - begin_of_word);
}

void manager::process_begin_of_command()
{
  set_first_part();
  set_second_part();
}

void manager::check_user_for_command()
{
  if(second_part != "signup" && current_user == NULL)
    throw PermissionDenied();
  if(second_part == "login" && current_user == NULL)
    throw PermissionDenied();
}

void manager::process_users()
{
  process_begin_of_command();
  check_user_for_command();
  process_command();
}


string manager::achieve_part()
{
  skip_space();
  int begin_of_word = command_chars_counter;
  while(command[command_chars_counter] != SPACE 
  && command[command_chars_counter] != NONE)
    command_chars_counter++;
  string str = command.substr(begin_of_word, 
  command_chars_counter - begin_of_word);
  return str;
}

int main()
{
  string command;
  manager Manager = manager();
  while(getline(cin, command))
  {
    try {
      Manager.set_command(command);
      Manager.process_users();
    } catch(PermissionDenied ex) {
      ex.what();
    } catch(NotFound ex) {
      ex.what();
    } catch(BadRequest ex) {
      ex.what();
    }
    Manager.reset();
  }
}