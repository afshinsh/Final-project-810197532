#ifndef _MY_HANDLERS_
#define _MY_HANDLERS_

#include "../server/server.hpp"
#include <cstdlib> // for rand and srand
#include <ctime>   // for time
#include <iostream>
#include <string>

class RandomNumberHandler : public RequestHandler {
public:
  Response *callback(Request *);
};

class LoginHandler : public RequestHandler {
public:
  LoginHandler(manager* _Manager) : Manager(_Manager) {}
  Response *callback(Request *);
  bool check_empety_login(string user, string pass);
  manager* Manager;
  customer* current_user;
};

class NewFilmHandler : public RequestHandler {
public:
  NewFilmHandler(manager* _Manager) : Manager(_Manager) {}
  Response *callback(Request *);
  bool check_empety_new_film(string name, string year, string lenght
  , string price, string summary, string rate, string director);
  manager* Manager;
  customer* current_user;
};

class HomeHandler : public RequestHandler {
public:
  HomeHandler(manager* _Manager) : Manager(_Manager) {}
  Response *callback(Request *);
  customer* current_user;
  void set_Body(string &body, int user_id, string filter);
  manager* Manager;
};

class SignupHandler : public RequestHandler {
public:
  SignupHandler(manager* _Manager) : Manager(_Manager) {}
  Response *callback(Request *);
  bool check_empety_signup(string user, string pass,string re_pass
  ,string age,string email);
  bool check_equality(string pass, string re_pass);
  customer* current_user;
  manager* Manager;
};

class UploadHandler : public RequestHandler {
public:
  Response *callback(Request *);
};

class DetailHandler : public TemplateHandler {
public:
  DetailHandler(manager* _Manager, string _filePath) : Manager(_Manager), TemplateHandler(_filePath) {}
  map<string, string> handle(Request *req);
  customer* current_user;
  manager* Manager;
};

class ProfileHandler : public TemplateHandler {
public:
  ProfileHandler(manager* _Manager, string _filePath) : Manager(_Manager) , TemplateHandler(_filePath) {}
  map<string,string> handle(Request *req);
  vector<string> write_films_in_vector(vector<film*> bought_films);
  customer* current_user;
  manager* Manager;
};

class ColorHandler : public TemplateHandler {
public:
  ColorHandler(std::string filePath);
  std::map<std::string, std::string> handle(Request *req);
};

#endif
