#include "handlers.hpp"

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
  , string age, string email)
{
  if(user == "" || pass == "" || re_pass == "" || age == "" || email == "")
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
  string publisher = req->getQueryParam("publisher");
  if(check_empety_signup(username, password, re_password, age, email))
    return show_alert_msg("you have left one or more fields!");
  if(check_equality(password, re_password))
    return show_alert_msg("not match for password!");
  try {
    Manager->set_command("POST signup ? email " + email + " username " + username + " password "
    + password + " age " + age  + " publisher " + publisher);
    Manager->process_users();
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

Response *HomeHandler::callback(Request *req) {
  Response *res = new Response;
  res->setHeader("Content-Type", "text/html");
  string body;
  body += "<!DOCTYPE html>";
  body += "<html>";
  body += "<body style=\"text-align: center;\">";
  body += "<h1>NETFLIX</h1>";
  int user_id = stoi(req->getSessionId());
  Manager->set_films(body, user_id);
  body += "</body>";
  body += "</html>";
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

ColorHandler::ColorHandler(string filePath) : TemplateHandler(filePath) {}

map<string, string> ColorHandler::handle(Request *req) {
  map<string, string> context;
  string newName = "I am " + req->getQueryParam("name");
  context["name"] = newName;
  context["color"] = req->getQueryParam("color");
  return context;
}
