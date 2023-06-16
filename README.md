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
curl -X DELETE http://localhost:8080/users/1
```
