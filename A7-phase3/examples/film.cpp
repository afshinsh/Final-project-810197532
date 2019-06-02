#include "film.h"

film::film(string _name, string _year, string _price, string _length
, string _summary , string _director, int _ID_counter_film, customer* _publisher)
 : name(_name), year(_year), price(_price), length(_length), summary(_summary)
 , director(_director), ID(_ID_counter_film), publisher(_publisher) { set_owner(_publisher); }

 void film::set_owner(customer* new_custormer)
{
  owners.push_back(new_custormer);
}

double film::give_avrage_rate()
{
  double sum = 0;
  double count = 0;
  for(int i = 0;i < owners.size();i++)
      sum += owners[i]->get_rate(ID, count);
  if(count == 0)
    return 0;
  return sum / count;
}

void film::set_comment(int film_id, string content, customer* owner)
{
  comment* new_comment = new comment(film_id, content, ID_counter_comment, owner);
  comments.push_back(new_comment);
  ID_counter_comment++;
}

void film::reply_cm(int comment_id, string content)
{
  for(int i = 0;i < comments.size();i++)
    if(comments[i]->get_ID() == comment_id)
    {
      comments[i]->set_reply(content);
      return;
    }
  throw NotFound();
}

void film::delete_comment(int comment_id)
{
  for(auto i = comments.begin();i != comments.end();i++)
    if((*i)->get_ID() == comment_id)
    {
      delete *i;
      comments.erase(i);
      return;
    }
  throw NotFound();
}

void film::show_comment()
{
  for(int i = 0;i < comments.size();i++)
  {
    cout<<i + 1<<". "<<comments[i]->get_content()<<endl;
    comments[i]->show_replies(i + 1);
  }
}

int film::get_graph()
{
  return films_graph[film_purpose->get_ID() - 1][ID - 1];
}
