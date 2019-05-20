#include "customer.h"

customer::customer(string _email, string _username, string _password
, string _age, int _ID_counter, bool _publisher) : email(_email), 
username(_username), password(_password), age(_age), ID(_ID_counter),
publish(_publisher) {}

void customer::get_copy_bought_film(vector<film*> &search_result)
{
  for(int i = 0; i < bought_films.size(); i++)
    search_result.push_back(bought_films[i]);
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
