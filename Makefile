REDIS=cplusplus-cache

######################
###  DEVELOPMENT   ###
######################

build:
	docker-compose -f docker-compose.yaml up --detach

destroy:
	docker-compose -f docker-compose.yaml down --volumes

lint:
	docker-compose -f docker-compose.yaml exec api npm run lint

keygen:
	docker-compose -f docker-compose.yaml exec api npm run keygen


start:
	docker-compose -f docker-compose.yaml up --detach

stop:
	docker-compose -f docker-compose.yaml stop

shell:
	docker-compose -f docker-compose.yaml exec api sh

install:
	npm install
	
logs:
	docker-compose -f docker-compose.yaml logs --follow api

redis-cli:
	docker exec -it ${REDIS} redis-cli

kill:
	sudo killall node
	
kill-docker:
	docker kill $(docker ps -q)
