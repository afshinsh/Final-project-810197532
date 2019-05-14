#include <iostream>
#include <vector>
#include <string>
#include <map>
#define SPACE ' '
#define QUERY "?"
#define EMPTEY_STRING ""
#define NONE '\0'

using namespace std;

class film;

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
  void score_watched_film(int film_id, double score);
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
  map <int,double> scores;
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
  film(string _name, string _year, string _price
, string _length, string _summary, string _director, int _ID_counter_film);
  void set_name(string _name) { name = _name; }
  void set_summary(string _summary) { summary = _summary; }
  void set_year(string _year) { year = _year; }
  void set_price(string _price) { price = _price; }
  void set_length(string _length) { length = _length; }
  void set_director(string _director) { director = _director; }
  int get_ID() { return ID; }
  int get_price() { return stoi(price); }
  void set_owner(customer* new_custormer);
  double give_avrage_rate();
private:
  string name;
  string year;
  string summary;
  string price;
  string length;
  string director;
  vector<customer*> owners;
  bool paid_money = false;
  int ID;
  //double rate = 0;
};

film::film(string _name, string _year, string _price, string _length
, string _summary , string _director, int _ID_counter_film)
 : name(_name), year(_year), price(_price), length(_length), summary(_summary)
 , director(_director), ID(_ID_counter_film) {}

class interface
{
public:
  interface();
  void process_users();
  void process_begin_of_command();
  void skip_space();
  void set_first_part();
  void set_second_part();
  void process_command();
  void process_GET_command();
  void process_POST_command();
  void check_DELETE_second_part();
  void POST_film();
  void POST_signup();
  void POST_money();
  void process_DELETE_command();
  void process_PUT_command();
  void GET_followers();
  void POST_followers();
  void DELETE_film();
  void charge_account();
  void POST_rate();
  void check_command_for_PUT(string &name, string &year, string &price
  , string &summary, string &length, string &director, string &film_id);
  void initialize_film(string name, string year, string length
  , string price, string summary, string director);
  void check_GET_second_part();
  void set_info(string &info);
  void show_head();
  void initialize_user(string &email, string &username
  , string &password, string &age, string &publisher);
  void find_user(string username, string password);
  void POST_login();
  void reset();
  void regist_film();
  void set_command(string _command) ;
  void check_POST_second_part();
  void check_first_part();
  string achieve_part();
  void check_PUT_second_part();
  void PUT_film();
  void check_repeated_username(string username);
  void POST_buy();
private:
  vector<film*> films;
  vector<customer*> users;
  vector<customer*> customers;
  vector<customer*> publishers;
  string command;
  int command_chars_counter = 0;
  int ID_counter = 1;
  int ID_counter_film = 1;
  customer* current_user = NULL;
  string first_part = EMPTEY_STRING ;
  string second_part = EMPTEY_STRING;
  string part;
  int property = 0;
};

interface::interface()
{
  customer* root = new customer("root", "root", "root", "root", 0, false);
  users.push_back(root);
  customers.push_back(root);
  publishers.push_back(root);
  films.push_back(new film("root", "root", "root", "root", "root", "root", 0));
}

void interface::set_command(string _command)
{
  command = _command;
}

void interface::check_repeated_username(string username)
{
  for(int i = 0; i < users.size(); i++)
  {
    if(username == users[i]->get_username())
      throw BadRequest();
  }
}

void interface::reset()
{
  first_part = EMPTEY_STRING;
  second_part = EMPTEY_STRING;
  command_chars_counter = 0;
}

void interface::set_info(string &info)
{
  part = achieve_part();
  info = part;
}

void film::set_owner(customer* new_custormer)
{
  owners.push_back(new_custormer);
}

void interface::initialize_user(string &email, string &username
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

void interface::find_user(string username, string password)
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
void interface::POST_login()
{
  if(second_part == "login")
  {
    if(achieve_part() != QUERY)
      throw BadRequest();
    string username, password;
    while(true)
    {
      part = achieve_part();
      if(part == EMPTEY_STRING)
        break;
      else if(part == "password")
        password = achieve_part();
      else if(part == "username")
        username = achieve_part();
      else 
        throw BadRequest();
    }
    find_user(username, password);
    cout<<"OK"<<endl;
  }
}

void interface::POST_signup()
{
  if(second_part == "signup" )
  {
    if(achieve_part() != QUERY)
      throw BadRequest();
    string email, username, password, age, publisher = EMPTEY_STRING;
    while(true)
    {
      part = achieve_part();
      if(part == EMPTEY_STRING)
        break;
      else if(part == "email")
        set_info(email);
      else if(part == "username")/////////////check arguments
        set_info(username);
      else if(part == "password")
        set_info(password);
      else if(part == "age")
        set_info(age);
      else if(part == "publisher")
        set_info(publisher);
      else 
        throw BadRequest();
    }
    check_repeated_username(username);
    initialize_user(email, username, password, age, publisher);
    cout<<"OK"<<endl;
  }
}

void interface::initialize_film(string name, string year, string length
, string price, string summary, string director)
{
  film* new_film = new film(name, year, price, length, summary, director, ID_counter_film);
  ID_counter_film++;
  films.push_back(new_film);
  film* my_film = new film(name , year, price, length, summary, director, ID_counter_film);
  current_user->regist_new_film(my_film);
}

void interface::regist_film()
{
  string name, year, price, summary, length, director;
  while(true)
  {
    part = achieve_part();
    if(part == EMPTEY_STRING)
      break;
    else if(part == "name")
      set_info(name);
    else if(part == "year")
      set_info(year);
    else if(part == "length")
      set_info(length);
    else if(part == "price")
      set_info(price);
    else if(part == "summary")
      set_info(summary);
    else if(part == "director")
      set_info(director);
    else 
      throw BadRequest();
  }
  initialize_film(name, year, length, price, summary, director);
}



void interface::POST_film()
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

void interface::check_POST_second_part()
{
  if(second_part != "signup" && second_part != "login" && second_part != "films" &&
  second_part != "money" && second_part != "replies" && second_part != "followers"
  && second_part != "buy" && second_part != "rate" && second_part != "comments")
    throw NotFound();
}

void interface::POST_followers()
{
  if(second_part == "followers")
  {
    if(achieve_part() != QUERY)
      throw BadRequest();
    string user_id;
    while(true)
    {
      part = achieve_part();
      if(part == EMPTEY_STRING)
        break;
      else if(part == "user_id")
        user_id = achieve_part();
      else
        throw BadRequest();
    }///some check
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

void interface::charge_account()
{
  string amount;
  while(true)
  {
    part = achieve_part();
    if(part == EMPTEY_STRING)
      break;
    else if(part == "amount")
      amount = achieve_part();
    else 
      throw BadRequest();
  }
  current_user->increase_money(stoi(amount));
  cout<<"OK"<<endl;
}

void interface::POST_money()
{
  if(second_part == "money")
  {
    if(achieve_part() == QUERY)
      charge_account();
   // else if(achieve_part() == EMPTEY_STRING)
     // catch_money();
    else 
      throw BadRequest();
  }
}

void customer::buy_film(film* new_film)
{
  bought_films.push_back(new_film);
  money -= new_film->get_price();
}

void interface::POST_buy()
{
  if(second_part == "buy")
  {
    if(achieve_part() != QUERY)
      throw BadRequest();
    string film_id;
    while(true)
    {
      part = achieve_part();
      if(part == EMPTEY_STRING)
        break;
      else if(part == "film_id")
        film_id = achieve_part();
      else 
        throw BadRequest();
    }//some check
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

void customer::score_watched_film(int film_id, double score)
{
  for(int i = 0; i < bought_films.size(); i++)
    if(bought_films[i]->get_ID() == film_id)
    {
      scores[film_id] = score;
      return;
    }
  throw BadRequest();
}

void interface::POST_rate()
{
  if(second_part == "rate")
  {
    if(achieve_part() != QUERY)
      throw BadRequest();
    string film_id, score;
    while(true)
    {
      part = achieve_part();
      if(part == EMPTEY_STRING)
        break;
      else if(part == "film_id")
        film_id = achieve_part();
      else if(part == "score")//some check this
        score = achieve_part();
      else 
        throw BadRequest();
    }
    current_user->score_watched_film(stoi(film_id), stod(score));
  }
}

void interface::process_POST_command()
{
  check_POST_second_part();
  POST_signup();
  POST_login();
  POST_film();
  POST_followers();
  POST_money();
  POST_buy();
  POST_rate();
  //...check_NOT_found
}

void interface::set_first_part()
{
  skip_space();
  int begin_of_word = command_chars_counter;
  while(command[command_chars_counter] != SPACE && command[command_chars_counter] != NONE)
    command_chars_counter++;
  first_part = command.substr(begin_of_word, 
  command_chars_counter - begin_of_word);
  check_first_part();
}

void interface::check_first_part()
{
  if(first_part != "GET" && first_part != "POST" && first_part != "PUT" && first_part != "DELETE")
    throw BadRequest();
}

void interface::check_PUT_second_part()
{
  if(second_part != "films")
    throw BadRequest();
}

void interface::check_command_for_PUT(string &name, string &year, string &price
, string &summary, string &length, string &director, string &film_id)
{
  while(true)
  {
    part = achieve_part();
    if(part == EMPTEY_STRING)
      break;
    else if(part == "film_id")
      set_info(film_id);
    else if(part == "name")
      set_info(name);
    else if(part == "year")
      set_info(year);
    else if(part == "length")
      set_info(length);
    else if(part == "price")
      set_info(price);
    else if(part == "summary")
      set_info(summary);
    else if(part == "director")
      set_info(director);
    else 
      throw BadRequest();
  }
}

void interface::PUT_film()
{
  if(second_part == "films")
  {
    if(achieve_part() != QUERY)
      throw BadRequest();
    string name, year, price, summary, length, director, film_id;    
    check_command_for_PUT(name, year, price, summary, length, director, film_id);
    current_user->edit_films(name, year, price, summary, length, director, stoi(film_id));
    cout<<"OK"<<endl;
  }
}

void publisher::edit_films(string name, string year, string price
, string summary, string length, string director, int film_id)
{
  for(int i = 0;i < my_films.size(); i++)
  {
    if(my_films[i]->get_ID() == film_id)
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
  {
    cout<<i + 1<<". ";
    cout<<followers[i]->get_ID()<<" | "<<followers[i]->get_username()
    <<" | "<<followers[i]->get_email()<<endl;
  }
}

void interface::check_DELETE_second_part()
{
  if(second_part != "films")
    throw BadRequest();
}

void interface::process_PUT_command()
{
  check_PUT_second_part();
  PUT_film();
}

void publisher::delete_film(int film_id)
{
  for(int i = 0 ;i < my_films.size(); i++)
  {
    if(my_films[i]->get_ID() == film_id)
    {
      delete my_films[i];
      return;
    }
  }
  throw PermissionDenied();
}

void interface::DELETE_film()
{
  if(achieve_part() != QUERY)
    throw BadRequest();
  string film_id;
  while(true)
  {
    part = achieve_part();
    if(part == EMPTEY_STRING)
      break;
    else if(part == "film_id")
      set_info(film_id);
    else 
      throw BadRequest();
  }
  current_user->delete_film(stoi(film_id));
  cout<<"OK"<<endl;
}

void interface::process_DELETE_command()
{
  check_DELETE_second_part();
  DELETE_film();
}

void interface::show_head()
{
  cout<<"List of Followers"<<endl;
  cout<<"#. User Id | User Username | User Email"<<endl;
}

void interface::GET_followers()
{
  show_head();
  current_user->show_followers();
}

void interface::check_GET_second_part()
{
  if(second_part != "followers" && second_part != "published" && second_part != "films"
  && second_part != "purchased"&& second_part != "notifications")
    throw BadRequest();
}

void interface::process_GET_command()
{
  check_GET_second_part();
  GET_followers();
}

void interface::process_command()
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

void interface::skip_space()
{
  while(command[command_chars_counter] == SPACE)
    command_chars_counter++;
}

void interface::set_second_part()
{
  skip_space();
  int begin_of_word = command_chars_counter;
  while(command[command_chars_counter] != SPACE)
    command_chars_counter++;
  second_part = command.substr(begin_of_word, 
  command_chars_counter - begin_of_word);
}

void interface::process_begin_of_command()
{
  set_first_part();
  set_second_part();
}

void interface::process_users()
{
  process_begin_of_command();
  process_command();
}


string interface::achieve_part()
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
  interface my_interface = interface();
  while(getline(cin, command))
  {
    try {
      my_interface.set_command(command);
      //cout<<command<<endl;
      my_interface.process_users();
    } catch(PermissionDenied ex) {
      ex.what();
    } catch(NotFound ex) {
      ex.what();
    } catch(BadRequest ex) {
      ex.what();
    }
    my_interface.reset();
  }

  }