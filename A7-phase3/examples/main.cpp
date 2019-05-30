#include "handlers.hpp"
#include "my_server.hpp"
#include <cstdlib> // for rand and srand
#include <ctime>   // for time
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
  try {
    MyServer server(argc > 1 ? atoi(argv[1]) : 5000);
    manager* Manager = new manager();
    server.setNotFoundErrPage("static/404.html");
    server.get("/login", new ShowPage("static/logincss.html"));
    server.post("/login", new LoginHandler(Manager));
    server.get("/signup", new ShowPage("static/signupcss.html"));
    server.post("/signup", new SignupHandler(Manager));
    server.get("/home", new HomeHandler(Manager));
    server.get("/rand", new RandomNumberHandler());
    server.get("/netflix2.png", new ShowImage("static/netflix2.png"));
    server.get("/", new ShowPage("static/first_page.html"));
    // server.get("/colors", new ColorHandler("static/colors.html"));
    server.run();
  } catch (Server::Exception e) {
    cerr << e.getMessage() << endl;
  }
}
