#include "manager.h"


using namespace std;


int main()
{
  string command;
  manager Manager = manager();
  while(getline(cin, command))
  {
    try {
      Manager.set_command(command);
      Manager.process_users();
    } catch(exceptions &ex) {
      ex.what();
    }
    Manager.reset();
  }
}