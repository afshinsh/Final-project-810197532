#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <iostream>

using namespace std;

class exceptions
{
public:
  virtual void what() = 0;
};

class BadRequest : public exceptions {
public:
  void what()
  {
    cout<<"Bad Request"<<endl;
  }
};

class PermissionDenied : public exceptions {
public:
  void what()
  {
    cout<<"Permission Denied"<<endl;
  }
};

class NotFound : public exceptions {
public:
  void what()
  {
    cout<<"Not Found"<<endl;
  }
};

#endif
