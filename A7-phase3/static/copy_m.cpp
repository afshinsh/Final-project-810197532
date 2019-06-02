#include "manager.h"


manager::manager()
{
  customer* admin = new customer("admin", "admin", "admin", "admin", 1, false);
  admin->set_hash_password(hash_password("admin"));
  users.push_back(admin);
  customers.push_back(admin);
  publishers.push_back(admin);
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
    if(username == users[i]->get_username())
      throw BadRequest();
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

int manager::hash_password(string const &password)
{
    unsigned int hash = 0;
    const unsigned int VALUE = password.length();
    for (auto Letter : password)
    {
        srand(VALUE * Letter);
        hash += 33 + rand() % 92;
    }
    return hash;
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

bool manager::check_is_not_double(string s)
{
  if(s == EMPTEY_STRING)
    return false;
  for(int i = 0;i < s.length();i++)
    if(!isdigit(s[i]) && s[i] != DOT)
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
  current_user->set_hash_password(hash_password(password));
}

void manager::find_user(string username, string password)
{
  customer* previous_user = current_user;
  current_user = NULL;
  for(int i = 0;i < users.size();i++)
  {
    if(users[i]->get_username() == username
    && users[i]->get_hash_password() == hash_password(password))
      current_user = users[i];
  }
  if(current_user == NULL)
  {
    current_user = previous_user;
    throw NotFound();
  }
  previous_user = NULL;
}

void manager::set_recomm(map <string,string> &context, int film_id) {
  set_recommendation(films[film_id]);
  context["recomm"] = "Name  |  Director  |  Year\n";
  for(int i = 0; i < recommendation_films.size() && i < 4; i++)
    context["recomm"] += recommendation_films[i]->get_name() + " | " +
    recommendation_films[i]->get_director() + " | " + recommendation_films[i]->get_year() + "\n";
}

void manager::set_info_of_film(map <string,string> &context, int film_id) {
  context["name"] = films[film_id]->get_name();
  context["year"] = films[film_id]->get_year();
  context["director"] = films[film_id]->get_director();
  context["summary"] = films[film_id]->get_summary();
  context["lenght"] = films[film_id]->get_length();
  context["rate"] = to_string(films[film_id]->give_avrage_rate());
  context["price"] = films[film_id]->get_price_s();
}

void manager::set_films(string &body, int user_id)
{
  body += "<h2>Name    |    Price    |    Year    |    lenght    |    Rate    |    Director</h2>";
  for(int i = 0; i < films.size(); i++)
    if(!films[i]->get_deleted() && (current_user->get_money() >= films[i]->get_price()))
    {
      body += to_string(i) + ".<P>" + films[i]->get_name() + "  |  " + films[i]->get_price_s() + "  |  "  + films[i]->get_year() + "  |  "+ films[i]->get_length() + "  |  "  + to_string(films[i]->give_avrage_rate()) + "  |  " + films[i]->get_director() +  "</p>";
      if(films[i]->get_publisher() == current_user)
        body += "<a href = '/detail?ID=" + to_string(films[i]->get_ID()) + "'>Delete</a> <br> ";
      if(films[i]->get_publisher() != current_user)
        body += "<a href = '/detail?ID=" + to_string(films[i]->get_ID()) + "'>Details and buy</a> <br>";
      body += " <br> ";
    }
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
  sentence_part = achieve_part();
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

void manager::POST_signup(string email, string username, string password, string age, string publisher)
{
  // if(second_part == "signup" )
  // {
    // string email, username, password, age, publisher = EMPTEY_STRING;
    // process_command_signup(email, username, password, age, publisher);
    check_repeated_username(username);
    if(check_is_not_integer(age))
      throw BadRequest();
    if(!check_email(email))
      throw BadRequest();
    initialize_user(email, username, password, age, publisher);
  // }
}

void manager::set_matrix()
{
  int * raw =(int *)malloc(films.size() * sizeof(int));
  for(int i = 0 ; i < films.size(); i++)
    raw[i] = 0;
  for(int i = 0; i < films_graph.size(); i++)
  {
    films_graph[i] = (int*)realloc(films_graph[i], films.size() * sizeof(int));
    films_graph[i][films.size() - 1] = 0;
  }
  films_graph.push_back(raw);
}

void manager::initialize_film(string name, string year, string length
, string price, string summary, string director)
{
  if(check_is_not_integer(year) || check_is_not_integer(length) || check_is_not_integer(price))
    throw BadRequest();
  film* new_film = new film(name, year, price, length, summary,
  director, ID_counter_film, current_user);
  films.push_back(new_film);
  current_user->regist_new_film_with_notif(new_film);
  set_matrix();
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
  && second_part != "buy" && second_part != "rate" && second_part != "comments" &&
  second_part != "put_films" && second_part != "delete_films" &&
  second_part != "delete_comments" && second_part != "logout")
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
    followed_publisher->set_notif_for_follow(current_user);
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

void manager::POST_buy(string film_id)
{
  film* bought_film = films[stoi(film_id)];
  if(!current_user->check_is_not_bought(bought_film))
    return;
  current_user->buy_film(bought_film);
  bought_film->set_owner(current_user);
  current_user->set_graph_films(films_graph, bought_film);
  property +=  bought_film->get_price();
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
    return (f1->get_graph() > f2->get_graph());
  }
};


void manager::set_recommendation(film* f)
{
  recommendation_films.clear();
  int j = 0;
  for(int i = 0;i < films.size();i++)
    if(current_user->check_is_not_bought(films[i]) && !films[i]->get_deleted()
     && films[i] != f)
  {
    recommendation_films.push_back(films[i]);
    recommendation_films[j]->set_purpose_film(f);
    recommendation_films[j]->set_films_graph(films_graph);
    j++;
  }
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

void manager::set_notif_for_rate(film* rated_film, customer* rater)
{
  string msg = "User " + rater->get_username() + " with id "
  + to_string(rater->get_ID()) + " rate your film " + rated_film->get_name()
  + " with id " + to_string(rated_film->get_ID()) + ".";
  rated_film->get_publisher()->add_to_unread_notif(msg);
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
    set_notif_for_rate(current_user->get_bought_film(stoi(film_id)), current_user);
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
    films[stoi(film_id)]->set_comment(stoi(film_id), content, current_user);
    set_notif_for_comment(stoi(film_id));
    cout<<"OK"<<endl;
  }
}

void manager::set_notif_for_comment(int film_id)
{
  customer* owner = films[film_id]->get_publisher();
  string msg = "User " + current_user->get_username() + " with id "
  + to_string(current_user->get_ID()) + " comment on your film " +
  films[film_id]->get_name() + " with id " + to_string(film_id) + ".";
  owner->add_to_unread_notif(msg);
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

void manager::set_notif_for_reply(int film_id, int comment_id)
{
  customer* owner = films[film_id]->get_comment(comment_id)->get_owner();
  string msg = "Publisher " + current_user->get_username() + " with id "
  + to_string(current_user->get_ID()) + " reply to your comment.";
  owner->add_to_unread_notif(msg);
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
    set_notif_for_reply(stoi(film_id), stoi(comment_id));
    cout<<"OK"<<endl;
  }
}

void manager::POST_logout()
{
  if(second_part == "logout")
  {
    if(achieve_part() != EMPTEY_STRING)
      throw BadRequest();
    current_user = NULL;
    cout<<"OK"<<endl;
  }
}

void manager::process_POST_command()
{
  check_POST_second_part();
  // POST_signup();
  POST_login();
  POST_film();
  POST_followers();
  POST_money();
  POST_buy();
  POST_rate();
  POST_comments();
  POST_replies();
  POST_put_film();
  POST_delete_comments();
  POST_delete_film();
  POST_logout();
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

void manager::POST_put_film()
{
  if(second_part == "put_films")
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

void manager::POST_delete_film()
{
  if(second_part == "delete_films")
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

void manager::POST_delete_comments()
{
  if(second_part == "delete_comments")
  {
    if(achieve_part() != QUERY)
      throw BadRequest();
    string film_id, comment_id;
    process_command_delete_comments(film_id, comment_id);
    if(check_is_not_integer(film_id) || check_is_not_integer(comment_id))
      throw BadRequest();
    if(stoi(film_id) >= ID_counter_film || films[stoi(film_id)]->get_deleted())
      throw NotFound();
    if(films[stoi(film_id)]->get_publisher() != current_user)
      throw PermissionDenied();
    films[stoi(film_id)]->delete_comment(stoi(comment_id));
    cout<<"OK"<<endl;
  }
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
  && second_part != "films"&& second_part != "purchased" &&
  second_part != "notifications" && second_part != "money")
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
  cout<<"Rate = "<<setprecision(3)<<Film->give_avrage_rate()<<endl;
  cout<<"Price = "<<Film->get_price_s()<<endl;
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

void manager::check_year(string min_year, string max_year)
{
  if(check_is_not_integer(min_year) || check_is_not_integer(max_year))
    throw BadRequest();
}

void manager::check_min_rate(string min_rate)
{
  if(min_rate == EMPTEY_STRING)
    return;
  if(check_is_not_double(min_rate))
    throw BadRequest();
}

void manager::check_price(string price)
{
  if(price == EMPTEY_STRING)
    return;
  if(check_is_not_integer(price))
    throw BadRequest();
}

void manager::get_copy_films()
{
  for(int i = 0; i < films.size(); i++)
    if(!films[i]->get_deleted())
      search_result.push_back(films[i]);
}

struct compare_ID
{
  inline bool operator() (film* f1, film* f2)
  {
    return (f1->get_ID() < f2->get_ID());
  }
};

void manager::set_result_films(string name, string min_year, string price
, string max_year, string min_rate, string director)
{
  for(auto i = search_result.begin(); i != search_result.end(); i++)
  {
    if(name != EMPTEY_STRING && (*i)->get_name() != name)
      search_result.erase(i);
    else if(min_rate != EMPTEY_STRING && (*i)->give_avrage_rate() < stod(min_rate))
      search_result.erase(i);
    else if(price != EMPTEY_STRING && (*i)->get_price() != stoi(price))
      search_result.erase(i);
    else if(director != EMPTEY_STRING && (*i)->get_director() != director)
      search_result.erase(i);
    else if(min_year != EMPTEY_STRING && stoi((*i)->get_year()) < stoi(min_year))
      search_result.erase(i);
    else if(max_year != EMPTEY_STRING && stoi((*i)->get_year()) > stoi(max_year))
      search_result.erase(i);
  }
}

bool manager::check_email(string email)
{
  int dot_offset, ad_offset;
  for(int i = 0; i < email.length(); i++)
  {
    if(!isdigit(email[i]) && !isalpha(email[i]))
      if(email[i] != DOT && email[i] != AD)
        return false;
    if(email[i] == DOT)
      dot_offset = i;
    if(email[i] == AD)
      ad_offset = i;
  }
  if(dot_offset < ad_offset)
    return false;
  if(dot_offset >= email.length() - 1)
    return false;
  return true;
}

void manager::show_result()
{
  cout<<"#. Film Id | Film Name | Film Length "<<
  "| Film price | Rate | Production Year | Film Director"<<endl;
  for(int i = 0; i < search_result.size(); i++)
    cout<<i + 1<<". "<<search_result[i]->get_ID()<<" | "<<
    search_result[i]->get_name()<<" | "<<search_result[i]->get_length()
    <<" | "<<search_result[i]->get_price()<<" | "<<setprecision(3)<<
    search_result[i]->give_avrage_rate()<<" | "<<search_result[i]->get_year()
    <<" | "<<search_result[i]->get_director()<<endl;
}

void manager::GET_films()
{
  if(second_part == "films" && sentence_part == EMPTEY_STRING)
  {
    search_result.clear();
    get_copy_films();
    string name, min_year, price, max_year, min_rate, director;
    process_command_GET_films(name, min_year, price, max_year, min_rate, director);
    check_min_rate(min_rate);
    check_price(price);
    check_year(min_year,max_year);
    set_result_films(name, min_year, price, max_year, min_rate, director);
    sort(search_result.begin(), search_result.end(), compare_ID());
    show_result();
  }
}

void manager::process_command_GET_purchased(string &name, string &min_year, string &price
, string &max_year, string &director)
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
    else if(sentence_part == "director")
      set_info(director);
    else
      throw BadRequest();
  }
}

void manager::set_result_purchased(string name, string min_year, string price
, string max_year, string director)
{
  for(auto i = search_result.begin(); i != search_result.end(); i++)
  {
    if(name != EMPTEY_STRING && (*i)->get_name() != name)
      search_result.erase(i);
    else if(price != EMPTEY_STRING && (*i)->get_price() != stoi(price))
      search_result.erase(i);
    else if(director != EMPTEY_STRING && (*i)->get_director() != director)
      search_result.erase(i);
    else if(min_year != EMPTEY_STRING && stoi((*i)->get_year()) < stoi(min_year))
      search_result.erase(i);
    else if(max_year != EMPTEY_STRING && stoi((*i)->get_year()) > stoi(max_year))
      search_result.erase(i);
  }
}

void manager::GET_purchased()
{
  if(second_part == "purchased")
  {
    search_result.clear();
    current_user->get_copy_bought_film(search_result, films);
    string name, min_year, price, max_year, director;
    process_command_GET_purchased(name, min_year, price, max_year, director);
    check_price(price);
    check_year(min_year,max_year);
    set_result_purchased(name, min_year, price, max_year, director);
    sort(search_result.begin(), search_result.end(), compare_ID());
    show_result();
  }
}

void manager::GET_published()
{
  if(second_part == "published")
  {
    if(!current_user->get_publisher())
      throw PermissionDenied();
    search_result.clear();
    current_user->get_copy_published_films(search_result);
    string name, min_year, price, max_year, min_rate, director;
    process_command_GET_films(name, min_year, price, max_year, min_rate, director);
    check_min_rate(min_rate);
    check_price(price);
    check_year(min_year,max_year);
    set_result_films(name, min_year, price, max_year, min_rate, director);
    sort(search_result.begin(), search_result.end(), compare_ID());
    show_result();
  }
}

void manager::process_command_GET_notif(string &limit)
{
  while(true)
  {
    sentence_part = achieve_part();
    if(sentence_part == EMPTEY_STRING)
      break;
    else if(sentence_part == "limit")
      limit = achieve_part();
    else
      throw BadRequest();
  }
}

void manager::GET_notifications_read()
{
  if(second_part == "notifications")
  {
    sentence_part = achieve_part();
    if(sentence_part == EMPTEY_STRING)
      return;
    if(sentence_part != "read" || achieve_part() != QUERY)
      throw BadRequest();
    string limit;
    if(check_is_not_integer(limit))
      throw BadRequest();
    process_command_GET_notif(limit);
    current_user->show_read_notif(stoi(limit));
  }
}

void manager::GET_notifications()
{
  if(second_part == "notifications" && sentence_part == EMPTEY_STRING)
  {
    sentence_part = achieve_part();
    if(sentence_part != EMPTEY_STRING)
      return;
    current_user->show_unread_notif();
  }
}

void manager::get_money_for_customer()
{
  if(current_user->get_username() != "admin" || current_user->get_password() != "admin")
    cout<<current_user->get_money()<<endl;
}

void manager::get_property_of_system()
{
  if(current_user->get_username() == "admin" && current_user->get_password() == "admin")
    cout<<property<<endl;
}

void manager::GET_money()
{
  if(second_part == "money")
  {
    if(achieve_part() != EMPTEY_STRING)
      throw BadRequest();
    get_money_for_customer();
    get_property_of_system();
  }
}

void manager::process_GET_command()
{
  check_GET_second_part();
  GET_followers();
  GET_film();
  GET_films();
  GET_purchased();
  GET_published();
  GET_notifications_read();
  GET_notifications();
  GET_money();
}

void manager::process_command()
{
  if(first_part == "POST")
    process_POST_command();
  else if(first_part == "GET")
    process_GET_command();
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

void manager::check_log_status()
{
  if(second_part == "logout" && current_user == NULL)
    throw BadRequest();
  if((second_part == "login" || second_part == "signup") && current_user != NULL)
    throw BadRequest();
  if(current_user == NULL  && second_part != "signup" && second_part != "login"
  && second_part != EMPTEY_STRING)
    throw PermissionDenied();
}

void manager::process_users()
{
  process_begin_of_command();
  check_log_status();
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

