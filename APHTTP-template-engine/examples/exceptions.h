#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <iostream>
#include <string>

using namespace std;

class exceptions
{
public:
  virtual string what() = 0;
};

class BadRequest : public exceptions {
public:
  string what()
  {
    return "Bad Request";
  }
};

class PermissionDenied : public exceptions {
public:
  string what()
  {
    return "Permission Denied";
  }
};

class NotFound : public exceptions {
public:
  string what()
  {
    return "Not Found";
  }
};

#endif
