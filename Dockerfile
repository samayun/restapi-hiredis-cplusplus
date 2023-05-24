# Base image with necessary build tools and Hiredis dependencies
FROM gcc:latest

# Install Hiredis library
RUN apt-get update && apt-get install -y libhiredis-dev libcpprest-dev libssl-dev

# Set the working directory
WORKDIR /app

# Copy the source code into the container
COPY . .

# Build the C++ application
# RUN g++ -o app main.cpp -lhiredis

# RUN cat main.cpp && ./app

# Set the entrypoint
# CMD ["./app"]

RUN g++ -o app main.cpp user_module.cpp -lcrypto -lcpprest -lhiredis -pthread -lssl -lcrypto

RUN cat main.cpp && ./app

CMD ["./app"]
