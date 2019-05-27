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

Response *LoginHandler::show_alert_msg(string msg)
{
  Response *res = new Response;
  res->setHeader("Content-Type", "text/html");
  string body;
  body += "<!DOCTYPE html>";
  body += "<html>";
  body += "<head>";
  body += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  body += "<style>";
  body += ".alert {";
  body += "padding: 20px;";
  body += "background-color: #f44336;";
  body += "color: white;";
  body += "}";
  body += ".closebtn {";
  body += "margin-left: 15px;";
  body += "color: white;";
  body += "font-weight: bold;";
  body += "float: right;";
  body += "font-size: 22px;";
  body += "line-height: 20px;";
  body += "cursor: pointer;";
  body += "transition: 0.3s;";
  body += "}";
  body += ".closebtn:hover {";
  body += "color: black;";
  body += "}";
  body += "</style>";
  body += "</head>";
  body += "<body>";
  body += "<h2>Error!</h2>";
  body += "<div class=\"alert\"";
  body += "<span class=\"closebtn\" onclick =\"this.parentElement.style.display='none';\">&times;</span>";
  body += "<strong>";
  body += msg;
  body += "</strong>";
  body += "</div>";
  body += "<br>";
  body += "<a href=\"/login\">Go to login page</a>";
  body += "<br>";
  body += "<a href=\"/\">Go to first page</a>";
  body += "</body>";
  body += "</html>";
  res->setBody(body);
  return res;
}

bool LoginHandler::check_empety(string u, string p)
{
  if(u == "" || p == "")
    return true;
  return false;
}


Response *LoginHandler::callback(Request *req) {
  string username = req->getBodyParam("username");
  string password = req->getBodyParam("password");
  if(check_empety(username, password))
    return show_alert_msg("you have left one or more fields!");
  customer* current_user ;
  try{
      Manager->find_user(username, password);
      current_user = Manager->get_curr_user();
  } catch(...) {
      return show_alert_msg("user not found!");
  }
  Response *res = Response::redirect("/rand"); ///////////////////
  res->setSessionId(to_string(current_user->get_ID()));
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
