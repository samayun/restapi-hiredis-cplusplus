# HI Redis

# CREATE USER

```sh

curl -X POST -H "Content-Type: application/json" -d '{"id":"1", "name":"John", "age":"30"}' http://localhost:8080/users

```

## GET USER

```sh
curl http://localhost:8080/users/1
```
