#include "manager.h"

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


bool manager::check_is_not_integer(string s)
{
  if(s == EMPTEY_STRING)
    return false;
  for(int i = 0;i < s.length();i++)
    if(!isdigit(s[i]))
      return true;
  return false;
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

void manager::process_command_login(string &username, string &password)
{
  int parametr_counter = 2;
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
    parametr_counter--;
  }
  if(parametr_counter > 0)
    throw BadRequest();
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
  int parametr_counter = 5;
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
    parametr_counter--;
  }
  if(parametr_counter > 1)
    throw BadRequest();
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
  int parametr_counter = 6;
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
    parametr_counter--;
  }
  if(parametr_counter > 0)
    throw BadRequest();
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

void manager::process_command_followers(string &user_id)
{
  int parametr_counter = 1;
  while(true)
  {
    sentence_part = achieve_part();
    if(sentence_part == EMPTEY_STRING)
      break;
    else if(sentence_part == "user_id")
      user_id = achieve_part();
    else
      throw BadRequest();
    parametr_counter--;
  }
  if(parametr_counter > 0)
    throw BadRequest();
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
    if(stoi(user_id) >= ID_counter)
      throw NotFound();
    customer* followed_publisher = users[stoi(user_id)];
    current_user->follow_publisher(followed_publisher);
    followed_publisher->set_followers(current_user);
    cout<<"OK"<<endl;
  }
}


void manager::process_command_money(string &amount)
{
  int parametr_counter = 1;
  while(true)
  {
    sentence_part = achieve_part();
    if(sentence_part == EMPTEY_STRING)
      break;
    else if(sentence_part == "amount")
      amount = achieve_part();
    else 
      throw BadRequest();
    parametr_counter--;
  }
  if(parametr_counter > 0)
    throw BadRequest();
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
  while(films[i]->get_unpaid_money() > 0)
  {
    current_user->increase_money(percent * films[i]->get_price());
    property -= percent * films[i]->get_price();
    films[i]->decrease_unpaid_mpney();
  }  
  films[i]->reset_unpaid_money();
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
  cout<<"OK"<<endl;
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
  int parametr_counter = 1;
  while(true)
  {
    sentence_part = achieve_part();
    if(sentence_part == EMPTEY_STRING)
      break;
    else if(sentence_part == "film_id")
      film_id = achieve_part();
    else 
      throw BadRequest();
    parametr_counter--;
  }
  if(parametr_counter > 0)
    throw BadRequest();
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


void manager::check_inputs_for_rate(string score, string film_id)
{
  if(check_is_not_integer(score) || check_is_not_integer(film_id))
    throw BadRequest();
  if(stoi(film_id) >= ID_counter_film)
    throw NotFound();
  if(stoi(score) < MIN_POINT || stoi(score) > MAX_POINT)
    throw BadRequest();
  if(current_user->check_is_not_bought(films[stoi(film_id)]))
    throw PermissionDenied();
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
  int parametr_counter = 2;
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
    parametr_counter--;
  }
  if(parametr_counter > 0)
    throw BadRequest();
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


void manager::process_command_comments(string &film_id, string &content)
{
  int parametr_counter = 2;
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
    parametr_counter--;
  }
  if(parametr_counter > 0)
    throw BadRequest();
}

void manager::POST_comments()
{
  if(second_part == "comments")
  {
    if(achieve_part() != QUERY)
      throw BadRequest();
    string film_id, content;
    process_command_comments(film_id, content);
    if(check_is_not_integer(film_id))
      throw BadRequest();
    if(stoi(film_id) >= ID_counter_film)
      throw NotFound();
    current_user->check_bought_film(films[stoi(film_id)]);
    films[stoi(film_id)]->set_comment(stoi(film_id), content);
    cout<<"OK"<<endl;
  }
}




void manager::process_command_replies(string &film_id, string &comment_id, string &content)
{
  int parametr_counter = 3;
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
    parametr_counter--;
  }
  if(parametr_counter > 0)
    throw BadRequest();
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
    if(stoi(film_id) >= ID_counter_film )
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
  //...check_NOT_found  Comments
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
  && first_part != "DELETE" && first_part != EMPTEY_STRING)
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
      throw PermissionDenied();
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

void manager::check_DELETE_second_part()
{
  if(second_part != "films" && second_part != "comments")
    throw BadRequest();
}

void manager::process_PUT_command()
{
  check_PUT_second_part();
  PUT_film();
}


void manager::process_command_delete(string &film_id)
{
  int parametr_counter = 1;
  while(true)
  {
    sentence_part = achieve_part();
    if(sentence_part == EMPTEY_STRING)
      break;
    else if(sentence_part == "film_id")
      set_info(film_id);
    else 
      throw BadRequest();
    parametr_counter--;
  }
  if(parametr_counter > 0)
    throw BadRequest();
}

void manager::DELETE_film()
{
  if(second_part == "films")
  {
    if(achieve_part() != QUERY)
      throw BadRequest();
    string film_id;
    process_command_delete(film_id);
    if(check_is_not_integer(film_id))
      throw BadRequest();
    if(stoi(film_id) >= ID_counter_film)
      throw NotFound();
    if(current_user != films[stoi(film_id)]->get_publisher())
      throw PermissionDenied();
    current_user->delete_film(stoi(film_id));
    films[stoi(film_id)]->set_delete();
    cout<<"OK"<<endl;
  }
}



void manager::process_command_delete_comments(string &film_id,string &comment_id)
{
  int parametr_counter = 2;
  while(true)
  {
    sentence_part = achieve_part();
    if(sentence_part == EMPTEY_STRING)
      break;
    else if(sentence_part == "film_id")
      film_id = achieve_part();
    else if(sentence_part == "comment_id")
      comment_id = achieve_part();
    else 
      throw BadRequest();
    parametr_counter--;
  }
  if(parametr_counter > 0)
    throw BadRequest();
}

void manager::DELETE_comments()
{
  if(second_part == "comments")
  {
    if(achieve_part() != QUERY)
      throw BadRequest();
    string film_id, comment_id;
    process_command_delete_comments(film_id, comment_id);
    if(check_is_not_integer(film_id) || check_is_not_integer(comment_id))
      throw BadRequest();
    if(stoi(film_id) >= ID_counter_film)
      throw NotFound();
    if(films[stoi(film_id)]->get_publisher() != current_user)
      throw PermissionDenied();
    films[stoi(film_id)]->delete_comment(stoi(comment_id));
    cout<<"OK"<<endl;
  }
}

void manager::process_DELETE_command()
{
  check_DELETE_second_part();
  DELETE_film();
  DELETE_comments();
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
  cout<<endl<<"Comments"<<endl;
  Film->show_comment();
}





void manager::show_recommendation(film* Film)
{
  cout<<endl<<"Recommendation Film"<<endl;
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

void manager::GET_film()
{
  if(second_part == "films" )
  {
    sentence_part = achieve_part();
    if(sentence_part == EMPTEY_STRING)
      return;
    if(sentence_part != QUERY)
      throw BadRequest();
    string film_id; 
    if(achieve_part() == "film_id")
      film_id = achieve_part();
    else
      throw BadRequest();
    if(check_is_not_integer(film_id))
      throw BadRequest();
    if(stoi(film_id) >= ID_counter_film || films[stoi(film_id)]->get_deleted())
      throw NotFound();
    set_recommendation(films[stoi(film_id)]);
    show_details(films[stoi(film_id)]);
  }
}

void manager::process_command_GET_films(string &name, string &min_year, string &price
, string &max_year, string &min_rate, string &director)
{
  while(true)
  {
    sentence_part = achieve_part();
    if(sentence_part == EMPTEY_STRING)
      break;
    else if(sentence_part == "min_year")
      set_info(min_year);
    else if(sentence_part == "name")
      set_info(name);
    else if(sentence_part == "max_year")
      set_info(max_year);
    else if(sentence_part == "price")
      set_info(price);
    else if(sentence_part == "min_rate")
      set_info(min_rate);
    else if(sentence_part == "director")
      set_info(director);
    else 
      throw BadRequest();
  }
}

void manager::

void manager::GET_films()
{
  if(second_part == "films" && sentence_part == EMPTEY_STRING)
  {
    string name, min_year, price, max_year, min_rate, director;
    process_command_GET_films(name, min_year, price, max_year, min_rate, director);
    check_name(name);
    check_min_rate(min_rate);
    check_price(price);
    check_year(min_year,max_year);
    check_director(director);
    show_result();
  }
}

void manager::process_GET_command()
{
  check_GET_second_part();
  GET_followers();
  GET_film();
  GET_films();
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
  else if(first_part == EMPTEY_STRING)
    return;
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
  if(current_user == NULL && second_part != "signup" && first_part != EMPTEY_STRING)///change
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