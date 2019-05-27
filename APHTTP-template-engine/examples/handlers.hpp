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
};

class SignupHandler : public RequestHandler {
public:
  SignupHandler(manager* _Manager) : Manager(_Manager) {}
  Response *callback(Request *);
  bool check_empety_signup(string user, string pass,string re_pass
  ,string age,string email);
  bool check_equality(string pass, string re_pass);
  manager* Manager;
};

class UploadHandler : public RequestHandler {
public:
  Response *callback(Request *);
};

class ColorHandler : public TemplateHandler {
public:
  ColorHandler(std::string filePath);
  std::map<std::string, std::string> handle(Request *req);
};

#endif
