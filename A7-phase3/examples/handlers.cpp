#include "handlers.hpp"
#include <fstream>

using namespace std;

Response *RandomNumberHandler::callback(Request *req) {
  Response *res = new Response;
  res->setHeader("Content-Type", "text/html");
  string body;
  body += "<!DOCTYPE html>";
  body += "<html>";
  body += "<body style=\"text-align: center;\">";
  body += "<h1>AP HTTP</h1>";
  body += "<p>";
  body += "a random number in [1, 10] is: ";
  body += to_string(rand() % 10 + 1);
  body += "</p>";
  body += "<p>";
  body += "SeddionId: ";
  body += req->getSessionId();
  body += "</p>";
  body += "</body>";
  body += "</html>";
  res->setBody(body);
  return res;
}

bool SignupHandler::check_empety_signup(string user, string pass,string re_pass
  , string age, string email) {
  if(user == "" || pass == "" || re_pass == "" || age == "" || email == "")
    return true;
  return false;
}

bool NewFilmHandler::check_empety_new_film(string name, string year, string lenght
, string price, string summary, string rate, string director) {
  if(name == "" || year == "" || lenght == "" || price == "" || summary == "" || rate == "" || director == "")
    return true;
  return false;
}

bool LoginHandler::check_empety_login(string user, string pass)
{
  if(user == "" || pass == "")
    return true;
  return false;
}

bool SignupHandler::check_equality(string pass, string re_pass)
{
  if(pass != re_pass)
    return true;
  return false;
}

Response *SignupHandler::callback(Request *req) {
  string username = req->getBodyParam("username");
  string password = req->getBodyParam("password");
  string re_password = req->getBodyParam("re_password");
  string email = req->getBodyParam("email");
  string age = req->getBodyParam("age");
  string publisher = req->getBodyParam("publisher");
  if(check_empety_signup(username, password, re_password, age, email))
    return show_alert_msg("you have left one or more fields!");
  if(check_equality(password, re_password))
    return show_alert_msg("not match for password!");
  try {
    Manager->POST_signup(email, username, password, age, publisher);
    current_user = Manager->get_curr_user();
  } catch(exceptions &ex) {
      return show_alert_msg(ex.what());
  }
  Response *res = Response::redirect("/home"); ///////////////////
  res->setSessionId(to_string(current_user->get_ID()));
  return res;
}

Response *LoginHandler::callback(Request *req) {
  string username = req->getBodyParam("username");
  string password = req->getBodyParam("password");
  if(check_empety_login(username, password))
    return show_alert_msg("you have left one or more fields!");
  try {
      Manager->find_user(username, password);
      current_user = Manager->get_curr_user();
  } catch(exceptions &ex) {
      return show_alert_msg(ex.what());
  }
  Response *res = Response::redirect("/home"); ///////////////////
  res->setSessionId(to_string(current_user->get_ID()));
  return res;
}

Response *NewFilmHandler::callback(Request *req) {
  string name = req->getBodyParam("name");
  string year = req->getBodyParam("year");
  string price = req->getBodyParam("price");
  string summary = req->getBodyParam("summary");
  string lenght = req->getBodyParam("lenght");
  string director = req->getBodyParam("director");
  string rate = req->getBodyParam("rate");
  if(check_empety_new_film(name, year, price, summary, lenght, rate, director))
    return show_alert_msg("you have left one or more fields!");
  try {
      Manager->initialize_film(name, year, lenght, price, summary, director);
      current_user = Manager->get_curr_user();
      current_user->score_watched_film(Manager->get_film_id() - 1, stoi(rate));
  } catch(exceptions &ex) {
      return show_alert_msg(ex.what());
  }
  Response *res = Response::redirect("/home");
  res->setSessionId(to_string(current_user->get_ID()));
  return res;
}

void HomeHandler::set_Body(string &body, int user_id, string filter)
{
  body += "<!DOCTYPE html>";
  body += "<html>";
  body += "<body style=\"text-align: center;\">";
  body += "<h1> NETFLIX </h1>";
  body += "<form action=\"/home\" >";
  body += "Filter by name of director : <input type=\"submit\" name=\"filter\" />";
  body += "</form>";
  body += "<br>";
  if(filter == "submit")
    Manager->set_sort_film(body, user_id);
  else
    Manager->set_films(body, user_id);
  for(int i = 0; i < 4; i++)
    body += "<br>";
  if(current_user->get_publisher())
    body += "<a href = \"/newfilm\">publish new film</a>";
  for(int i = 0; i < 4; i++)
    body += "<br>";
  body += "<a href = \"/profile\">Profile</a>";
  for(int i = 0; i < 4; i++)
    body += "<br>";
  body += "<a href = \"/\">Logout</a>";
  body += "</body>";
  body += "</html>";
}

Response *HomeHandler::callback(Request *req) {
  Response *res = new Response;
  int user_id = stoi(req->getSessionId());
  current_user = Manager->get_curr_user();
  res->setHeader("Content-Type", "text/html");
  string Delete = req->getQueryParam("DLT");
  string filter = req->getQueryParam("filter");
  if(Delete == "true")
  {
    string film_id = req->getQueryParam("ID");
    Manager->POST_delete_film(film_id);
  }
  string body;
  set_Body(body, user_id, filter);
  res->setSessionId(req->getSessionId());
  res->setBody(body);
  return res;
}

Response *UploadHandler::callback(Request *req) {
  string name = req->getBodyParam("file_name");
  string file = req->getBodyParam("file");
  cout << name << " (" << file.size() << "B):\n" << file << endl;
  Response *res = Response::redirect("/");
  return res;
}

vector<string> ProfileHandler::write_films_in_vector(vector<film*> bought_films)
{
  vector<string> films;
  for(int i = 0; i < bought_films.size(); i++)
    films.push_back(bought_films[i]->get_name() + " | " + bought_films[i]->get_year()
    + " | " + bought_films[i]->get_price_s() + " | " + bought_films[i]->get_length()
    + " | " + bought_films[i]->get_director() + " | " +  to_string(bought_films[i]->give_avrage_rate()) );
  return films;
}

map<string,string> DetailHandler::handle(Request* req) {
  map <string,string> context;
  int user_id = stoi(req->getSessionId());
  current_user = Manager->get_curr_user();
  string film_id = req->getQueryParam("ID");
  string rate = req->getQueryParam("rate");
  if(film_id == "" && rate == "")
  {
    context["check"] = "false";
    film_id = Manager->get_s_film_id();
    Manager->POST_buy(film_id);
    Manager->set_info_of_film(context, stoi(film_id));
    Manager->set_recomm(context,stoi(film_id));
  }
  else if(film_id == "" && rate != "")
  {
    context["check"] = "false";
    film_id = Manager->get_s_film_id();
    current_user->score_watched_film(stoi(film_id), stoi(rate));
    Manager->set_info_of_film(context, stoi(film_id));
    Manager->set_recomm(context,stoi(film_id));
  }
  else
  {
    context["check"] = "";
    Manager->set_info_of_film(context, stoi(film_id));
    Manager->set_recomm(context,stoi(film_id));
    Manager->set_film_id(film_id);
  }
  return context;
}

map<string,string> ProfileHandler::handle(Request* req) {
  int user_id = stoi(req->getSessionId());
  current_user = Manager->get_curr_user();
  string amount = req->getQueryParam("amount");
  if(amount != "")
    current_user->increase_money(stoi(amount));
  vector<string> films = write_films_in_vector(current_user->get_bought_films());
  string films_text;
  for(int i = 0; i < films.size(); i++)
  {
    films_text += films[i];
    films_text += "\n";
  }
  map<string,string> context;
  context["films"] = films_text;
  return context;
}

ColorHandler::ColorHandler(string filePath) : TemplateHandler(filePath) {}

map<string, string> ColorHandler::handle(Request *req) {
  map<string, string> context;
  string newName = "I am " + req->getQueryParam("name");
  context["name"] = newName;
  context["color"] = req->getQueryParam("color");
  return context;
}
