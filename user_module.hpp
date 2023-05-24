#pragma once

#include <cpprest/http_listener.h>

using namespace web;
using namespace web::http;
//
using namespace web::http::experimental::listener;

class UserModule
{
public:
          UserModule();

          void registerRoutes(http_listener &listener);

private:
          void handleCreate(const http_request &request);
          void handleGet(const http_request &request);
          void handleUpdate(const http_request &request);
          void handleDelete(const http_request &request);
};
