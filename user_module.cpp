#include "user_module.hpp"
#include <iostream>
#include <cpprest/json.h>
#include <hiredis/hiredis.h>
#include <cpprest/http_listener.h>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

redisContext *redis;

UserModule::UserModule()
{
          redis = redisConnect("localhost", 6363);
          if (redis == nullptr || redis->err)
          {
                    printf("Failed to connect to Redis: %s\n", redis->errstr);
                    exit(1);
          }
}

void UserModule::registerRoutes(http_listener &listener)
{
          listener.support(methods::POST, std::bind(&UserModule::handleCreate, this, std::placeholders::_1));
          listener.support(methods::GET, std::bind(&UserModule::handleGet, this, std::placeholders::_1));
          listener.support(methods::PUT, std::bind(&UserModule::handleUpdate, this, std::placeholders::_1));
          listener.support(methods::DEL, std::bind(&UserModule::handleDelete, this, std::placeholders::_1));
}

void UserModule::handleCreate(const http_request &request)
{
          // Read JSON body from the request
          json::value requestBody = request.extract_json().get();

          // Extract user properties from JSON
          std::string id = requestBody["id"].as_string();
          std::string name = requestBody["name"].as_string();
          std::string age = requestBody["age"].as_string();

          // Execute Redis command to create the user
          (redisReply *)redisCommand(redis, "HSET user:%s name %s age %s", id.c_str(), name.c_str(), age.c_str());

          redisReply *reply = (redisReply *)redisCommand(redis, "HGETALL user:%s", id.c_str());

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

void UserModule::handleGet(const http_request &request)
{

          // Extract user ID from the request URI
          std::string id = request.request_uri().to_string();
          // std::cout << id << std::endl;
          id = id.substr(id.find_last_of('/') + 1);

          // Execute Redis command to get user information
          redisReply *reply = (redisReply *)redisCommand(redis, "HGETALL user:%s", id.c_str());

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

void UserModule::handleUpdate(const http_request &request)
{
          // Extract user ID from the request URI
          std::string id = request.request_uri().to_string();
          id = id.substr(id.find_last_of('/') + 1);

          // Read JSON body from the request
          json::value requestBody = request.extract_json().get();

          // Extract user properties from JSON
          std::string name = requestBody["name"].as_string();
          std::string age = requestBody["age"].as_string();

          // Execute Redis command to update the user
          (redisReply *)redisCommand(redis, "HSET user:%s name %s age %s", id.c_str(), name.c_str(), age.c_str());

          redisReply *reply = (redisReply *)redisCommand(redis, "HGETALL user:%s", id.c_str());

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

void UserModule::handleDelete(const http_request &request)
{
          // Extract user ID from the request URI
          std::string id = request.request_uri().to_string();
          id = id.substr(id.find_last_of('/') + 1);

          // Execute Redis command to delete the user
          redisReply *reply = (redisReply *)redisCommand(redis, "DEL user:%s", id.c_str());
          freeReplyObject(reply);

          // Send response
          request.reply(status_codes::OK, "User deleted successfully");
}
