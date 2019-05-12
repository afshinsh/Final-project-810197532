#include <iostream>
#include <vector>
#include <string>

using namespace std;

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
protected:
  string email;
  string username;
  string password;
  int ID;
  string age;
  bool publish;
};

customer::customer(string _email, string _username, string _password
, string _age, int _ID_counter, bool _publisher) : email(_email), 
username(_username), password(_password), age(_age), ID_counter(_ID_counter),
publish(publisher) {}

class publisher : public customer
{
public:
  publisher(string _email, string _username, string _password
, string _age, int _ID_counter, bool _publisher);
  void regist_new_film(film* new_film) ;
private:
  vector<film*> my_films;
};

publish::publisher(string _email, string _username, string _password
, string _age, int _ID_counter, bool _publisher) : customer(_email, 
_username, _password, _age, _ID_counter, _publisher) {}

void publisher::regist_new_film(film* new_film)
{
  my_films.push_back(new_film);
}

class film
{
public:
  film(string _name, string _year, string _price, string _summary
, string _length, int _ID_counter_film);
private:
  string name;
  string year;
  string summary;
  string price;
  string length;
}

film::film(string _name, string _year, string _price, string _summary
, string _length, int _ID_counter_film)
 : name(), year(), price(), summary(), length(), ID_counter_film() {}

class interface
{
public:
  interface();
  void process_customers();
  void process_publisher();
  void process_begin_of_command();
  void skip_space();
  void set_first_part();
  void set_second_part();
  void process_command();
  void process_GET_command();
  void process_POST_command();
  void POST_film();
  void POST_signup();
  void process_DELETE_command();
  void process_PUT_command();
  void set_info(string &info);
  void initialize_user(string &email, string &username
  , string &password, string &age, string &publisher);
  void find_user(string username, string password);
  void POST_signup();
  void POST_login();
  void reset();
  void regist_film();
  void set_command(string _command) ;
  string achive_part()
private:
  vector<film*> films;
  vactor<customer*> users;
  vector<customer*> customers;
  vector<customer*> publishers;
  string command;
  int command_chars_counter = 0;
  int ID_counter = 1;
  int ID_counter_film = 1;
  customer* current_user;
  string first_part = "" 
  string second_part = "";
  string part;
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

void interface::reset()
{
  first_part = "";
  second_part = "";
  command_chars_counter = 0;
}

void interface::set_info(string &info)
{
  part = achive_part();
  info = part;
}

void interface::initialize_user(string &email, string &username
, string &password, string &age, string &publisher)
{
  if(publisher == "" || publisher == "false")
  {
    current_user = new customer(email, username, password, age, ID_counter, false);
    ID_counter++;
    users.push_back(current_user);
    customers.push_back(current_user);
  }
  else if(publisher == "true")
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
    throw NotFound();
  }
  previous_user = NULL;
}
void interface::POST_login()
{
  if(second_part == "login" && achive_part() == "?")
  {
    string username, password;
    while(true)
    {
      part = achive_part();
      if(part == "username")
        username = achive_part();
      else if(part == "password")
        password = achive_part();
      else if(part == "")
        break;
      else 
        throw BadRequest();
    }
    find_user(username, password);
    cout<<"OK"<<endl;
  }
}

void interface::POST_signup()
{
  if(second_part == "signup" && achive_part() == "?")
  {
    string email, username, password, age, publisher = "";
    while(true)
    {
      part = achive_part();
      if(part == "")
        break;
      else if(part == "email")
        set_info(email);
      else if(part == "username")
        set_info(username);
      else if(part == "password")
        set_info(password);
      else if(part == "age")//check age
        set_info(age);
      else if(part == "publisher")
        set_info(publisher);
      else 
      throw BadRequest();
    }
    initialize_user(email, username, password, age, publisher);
    cout<<"OK"<<endl;
  }
}

void interface::initialize_film(string name, string year, string length
, string price, string film)
{
  film* new_film = new film(name, year, price, summary, length, ID_counter_film);
  ID_counter_film++;
  films.push_back(new_film);
  current_user->regist_new_film(film);
}

void interface::regist_film()
{
  string name, year, price, summary, length;
  while(true)
  {
    part = achive_part();
    if(part == "")
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
    else 
      throw BadRequest();
  }
  initialize_film(name, year, length, price, film);
}

void interface::POST_film()
{
  if(second_part == "films" && achive_part() == "?")
  {
    if(!current_user->get_publisher())
      throw PermissionDenied();
    regist_film()
    cout<<"OK"<<endl;
  }
}

void interface::process_POST_command()
{
  POST_signup();
  POST_login();
  POST_film();
  //...check_NOT_found
}

void interface::set_first_part()
{
  skip_space();
  int begin_of_word = command_chars_counter;
  while(command[command_chars_counter] != ' ' command[command_chars_counter] != '\0')
    command_chars_counter++;
  first_part = command.substr(begin_of_word, 
  command_chars_counter - begin_of_word);
}

void interface::process_command()
{
  if(first_part == "GET")
    process_GET_command();
  else if(first_part == "POST")
    process_POST_command();
  else if(first_part == "PUT")
    process_PUT_command();
  else if(first_part == "DELETE")
    process_DELETE_command();
  else
    throw BadRequest();
}

void interface::skip_space()
{
  while(command[command_chars_counter] == ' ')
    command_chars_counter++;
}

void interface::set_second_part()
{
  skip_space();
  int begin_of_word = command_chars_counter;
  while(command[command_chars_counter] != ' ')
    command_chars_counter++;
  second_part = command.substr(begin_of_word, 
  command_chars_counter - begin_of_word);
}

void interface::process_begin_of_command()
{
  set_first_part();
  set_second_part();
}

void interface::process_customers()
{

}


string interface::achive_part()
{
  skip_space();
  int begin_of_word = command_chars_counter;
  while(command[command_chars_counter] != ' ')
    command_chars_counter++;
  string str = command.substr(begin_of_word, 
  command_chars_counter - begin_of_word);
  return str;
}

void interface::process_publisher()
{
  process_begin_of_command();
  process_command();
}

int main()
{
  string command;
  interface my_interface();
  while(getline(cin, command))
  {
    try {
      interface.set_command(command);
      my_interface.process_customers(command);
      my_interface.process_publisher(command);
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