### CPP rest sdk Hiredis Restful Api by C++

### Pre requirement

- OS - Linux preferd
- Docker
- Docker Compose

### Before run cpp file install docker & docker-compose must

- [Installation Docker & Docker Compose](https://www.theserverside.com/blog/Coffee-Talk-Java-News-Stories-and-Opinions/How-to-install-Docker-and-docker-compose-on-Ubuntu)

### Installation and Commands

```bash
// Build container and run redis on local pc
sudo make build
// run the c++ program
bash run.sh

```

![image](https://github.com/samayun/restapi-hiredis-cplusplus/assets/31636535/66a6b2b0-deab-4cf0-bd25-60222336c773)

![image](https://github.com/samayun/restapi-hiredis-cplusplus/assets/31636535/6a36e1f4-d85c-4929-bc68-a9d9ade1cc97)

## CRUD using Terminal (Curl)

![image](https://github.com/samayun/restapi-hiredis-cplusplus/assets/31636535/2de725d4-b488-4317-a263-5090552067fd)

## Database Dashboard

![image](https://github.com/samayun/restapi-hiredis-cplusplus/assets/31636535/206cda8d-3afc-4226-b4b6-01d9435dc87f)

### CREATE USER

```sh
 curl -X POST -H "Content-Type: application/json" -d '{"id":"1", "name":"John", "age":"30"}' http://localhost:8080/users

```

### GET USER

```sh
curl http://localhost:8080/users/1

```

### UPDATE

```sh
curl -X PUT -H "Content-Type: application/json" -d '{"name":"John Doe", "age":"35"}' http://localhost:8080/users/1
```

### UPDATE

Delete a user (DELETE):

```sh
curl -X DELETE http://localhost:8080/users/1`
```

````

                   ## #BASIC CRUD

```cpp

#include <iostream>
#include <cpprest/json.h>
#include <hiredis/hiredis.h>
#include <cpprest/http_listener.h>

                   using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

void handle_request(const http_request &request)
{
     // Set the response content type as JSON
     request.reply(status_codes::OK, "Hello, World!");
}

// Global Redis context
redisContext *redis;

// Function to print the Redis reply
void printReply(redisReply \*reply)
{
     if (reply == nullptr)
     {
          printf("Failed to execute Redis command\n");
          return;
     }

     switch (reply->type)
     {
     case REDIS_REPLY_STATUS:
     case REDIS_REPLY_STRING:
          printf("%s\n", reply->str);
          break;
     case REDIS_REPLY_INTEGER:
          printf("%lld\n", reply->integer);
          break;
     case REDIS_REPLY_ARRAY:
          for (size_t i = 0; i < reply->elements; i++)
          {
               printReply(reply->element[i]);
          }
          break;
     default:
          printf("Unknown Redis reply type: %d\n", reply->type);
     }
}

// Function to handle authentication
bool authenticate(const http_request &request)
{
     // Perform authentication logic here
     // You can check for authentication headers, tokens, or any other mechanism

     // For demonstration purposes, allow all requests
     return true;
}

// Function to handle POST requests and create a new user
void handlePost(const http_request &request)
{
     if (!authenticate(request))
     {
          request.reply(status_codes::Unauthorized, "PLEASE LOGIN FIRST");
          return;
     }

     // Read JSON body from the request
     json::value requestBody = request.extract_json().get();

     // Extract user properties from JSON
     std::string id = requestBody["id"].as_string();
     std::string name = requestBody["name"].as_string();
     std::string age = requestBody["age"].as_string();

     // Execute Redis command to create the user
     (redisReply *)redisCommand(redis, "HSET user:%s name %s age %s", id.c_str(), name.c_str(), age.c_str());

     redisReply *reply = (redisReply *)redisCommand(redis, "HGETALL user:%s", id.c_str());
     printReply(reply);

     // Construct JSON response
     json::value response;
     response["id"] = json::value::string(id);
     for (size_t i = 0; i < reply->elements; i += 2)
     {
          std::string key = reply->element[i]->str;
          std::string value = reply->element[i + 1]->str;
          response[key] = json::value::string(value);
     }
     freeReplyObject(reply);
     // Send response
     request.reply(status_codes::OK, response);
}

// Function to handle GET requests and retrieve user information
void handleGet(const http_request &request)
{
     if (!authenticate(request))
     {
          request.reply(status_codes::Unauthorized, "PLEASE LOGIN FIRST");
          return;
     }

     // Extract user ID from the request URI
     std::string id = request.request_uri().to_string();
     std::cout << id << std::endl;
     id = id.substr(id.find_last_of('/') + 1);

     // Execute Redis command to get user information
     redisReply *reply = (redisReply *)redisCommand(redis, "HGETALL user:%s", id.c_str());
     printReply(reply);

     // Construct JSON response
     json::value response;
     response["id"] = json::value::string(id);

     for (size_t i = 0; i < reply->elements; i += 2)
     {
          std::string key = reply->element[i]->str;
          std::string value = reply->element[i + 1]->str;
          response[key] = json::value::string(value);
     }
     freeReplyObject(reply);
     // Send response
     request.reply(status_codes::OK, response);
}

// Function to handle PUT requests and update user information
void handlePut(const http_request &request)
{
     if (!authenticate(request))
     {
          request.reply(status_codes::Unauthorized, "PLEASE LOGIN FIRST");
          return;
     }

     // Extract user ID from the request URI
     std::string id = request.request_uri().to_string();
     id = id.substr(id.find_last_of('/') + 1);

     // Read JSON body from the request
     json::value requestBody = request.extract_json().get();

     // Extract user properties from JSON
     std::string name = requestBody["name"].as_string();
     std::string age = requestBody["age"].as_string();

     // Execute Redis command to update the user
     redisReply *reply = (redisReply *)redisCommand(redis, "HSET user:%s name %s age %s", id.c_str(), name.c_str(), age.c_str());
     printReply(reply);

     // Construct JSON response
     json::value response;
     response["id"] = json::value::string(id);
     for (size_t i = 0; i < reply->elements; i += 2)
     {
          std::string key = reply->element[i]->str;
          std::string value = reply->element[i + 1]->str;
          response[key] = json::value::string(value);
     }
     freeReplyObject(reply);

     // Send response
     request.reply(status_codes::OK, response);
}
// Function to handle DELETE requests and delete a user

void handleDelete(const http_request &request)
{
     if (!authenticate(request))
     {
          request.reply(status_codes::Unauthorized, "PLEASE LOGIN FIRST");
          return;
     }

     // Extract user ID from the request URI
     std::string id = request.request_uri().to_string();
     id = id.substr(id.find_last_of('/') + 1);

     // Execute Redis command to delete the user
     redisReply *reply = (redisReply *)redisCommand(redis, "DEL user:%s", id.c_str());
     printReply(reply);
     freeReplyObject(reply);

     // Send response
     request.reply(status_codes::OK, "User deleted successfully");
}

int main()
{
     const int REDIS_PORT = 6363;

     // Connect to Redis server
     redis = redisConnect("localhost", REDIS_PORT);
     if (redis == nullptr || redis->err)
     {
          printf("Failed to connect to Redis: %s\n", redis->errstr);
          exit(1);
     }

     // Create HTTP listener and map request handlers
     http_listener listener("http://localhost:8080");
     listener.support(methods::POST, handlePost);
     listener.support(methods::GET, handleGet);
     listener.support(methods::PUT, handlePut);
     listener.support(methods::DEL, handleDelete);

     try
     {
          // Start the HTTP listener
          listener.open().wait();

          printf("Listening on http://localhost:8080\n");

          // Keep the program running until interrupted
          std::cout << "Press Enter to exit." << std::endl;
          std::cin.ignore();
     }
     catch (const std::exception &ex)
     {
          printf("Error: %s\n", ex.what());
     }

     // Disconnect from Redis server
     redisFree(redis);

     // Close the HTTP listener
     listener.close().wait();

     return 0;
}

````
