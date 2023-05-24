#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <hiredis/hiredis.h>

void printReply(redisReply *reply)
{
    if (reply == NULL)
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

int main()
{

    const int REDIS_PORT = 6363;

    // Connect to Redis server
    redisContext *redis = redisConnect("localhost", REDIS_PORT);
    if (redis == NULL || redis->err)
    {
        printf("Failed to connect to Redis: %s\n", redis->errstr);
        exit(1);
    }

    std::string input;

    std::cout << "ENETR NAME: ";
    std::cin >> input;

    std::string setter = "SET hello " + input;

    redisReply *reply = (redisReply *)redisCommand(redis, setter.c_str());
    printReply(reply);
    freeReplyObject(reply);

    reply = (redisReply *)redisCommand(redis, "GET hello");

    printReply(reply);
    freeReplyObject(reply);

    // Create
    reply = (redisReply *)redisCommand(redis, "HSET user:1 name John age 30");
    printReply(reply);
    freeReplyObject(reply);

    // Read
    reply = (redisReply *)redisCommand(redis, "HGET user:1 name");
    printReply(reply);
    freeReplyObject(reply);

    // Update
    reply = (redisReply *)redisCommand(redis, "HSET user:1 age 35");
    printReply(reply);
    freeReplyObject(reply);

    // Read again to verify update
    reply = (redisReply *)redisCommand(redis, "HGET user:1 age");
    printReply(reply);
    freeReplyObject(reply);

    // Delete
    // reply = (redisReply *)redisCommand(redis, "DEL user:1");
    // printReply(reply);
    // freeReplyObject(reply);

    // Disconnect from Redis server
    redisFree(redis);

    return 0;
}
