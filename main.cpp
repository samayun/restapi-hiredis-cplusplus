#include "user_module.hpp"
#include <iostream>
#include <cpprest/json.h>
#include <hiredis/hiredis.h>
#include <cpprest/http_listener.h>

using namespace web::http::experimental::listener;

int main()
{
          http_listener listener("http://localhost:8080");
          UserModule userModule;

          userModule.registerRoutes(listener);

          try
          {
                    listener.open().wait();

                    printf("Listening on http://localhost:8080\n");

                    printf("Datbase dashboard on http://localhost:8081\n");

                    std::cout << "Press Enter to exit." << std::endl;
                    std::cin.ignore();
          }
          catch (const std::exception &ex)
          {
                    printf("Error: %s\n", ex.what());
          }

          listener.close().wait();

          return 0;
}
