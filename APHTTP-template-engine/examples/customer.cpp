#include "customer.h"

customer::customer(string _email, string _username, string _password
, string _age, int _ID_counter, bool _publisher) : email(_email),
username(_username), password(_password), age(_age), ID(_ID_counter),
publish(_publisher) {}

void customer::get_copy_bought_film(vector<film*> &search_result, vector<film*> films)
{
  for(int i = 0; i < films.size(); i++)
    for(int j = 0; j < bought_films.size(); j++)
      if(films[i]->get_ID() == bought_films[j]->get_ID())
        search_result.push_back(films[i]);
}

void customer::set_graph_films(vector<int*> &films_graph, film* bought_film)
{
  for(int i = 0; i < bought_films.size() ; i++)
  {
    films_graph[bought_film->get_ID() - 1][bought_films[i]->get_ID() - 1]++;
    films_graph[bought_films[i]->get_ID() - 1][bought_film->get_ID() - 1]++;
  }
}

double customer::get_rate(int film_id, double &count)
{
  if(scores.find(film_id) != scores.end())
  {
    count++;
    return scores[film_id];
  }
  return 0;
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
    throw PermissionDenied();
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

void customer::show_read_notif(int limit)
{
  int counter = 1;
  cout<<"#. Notification Message"<<endl;
  for(int i = read_notif.size() - 1; i >= read_notif.size() - limit; i--)
  {
    cout<<counter<<". "<<read_notif[i]<<endl;
    counter++;
  }
}

void customer::show_unread_notif()
{
  int counter = 0;
  cout<<"#. Notification Message"<<endl;
  for(int i = unread_notif.size() - 1; i >= 0; i--)
  {
    cout<<counter + 1<<". "<<unread_notif[i]<<endl;
    read_notif.push_back(unread_notif[counter]);
    counter++;
  }
  unread_notif.clear();
}

film* customer::get_bought_film(int film_id)
{
  for(int i = 0; i < bought_films.size(); i++)
    if(bought_films[i]->get_ID() == film_id)
      return bought_films[i];
  return NULL;
}

bool customer::check_is_not_bought(film* r_film)
{
  for(int i = 0;i < bought_films.size();i++)
    if(r_film->get_ID() == bought_films[i]->get_ID())
      return false;
  return true;
}

void customer::check_bought_film(film* bought_film)
{
  for(int i = 0;i < bought_films.size();i++)
    if(bought_film->get_ID() == bought_films[i]->get_ID())
      return;
  throw BadRequest();
}

void customer::follow_publisher(customer* new_publisher)
{
  if(!new_publisher->get_publisher())
    throw BadRequest();
  for(int i = 0;i < followed_publishers.size(); i++)
    if(new_publisher->get_ID() == followed_publishers[i]->get_ID())
      return;
  followed_publishers.push_back(new_publisher);
}
