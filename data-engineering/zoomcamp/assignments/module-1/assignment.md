# Module 1 Homework: Docker & SQL
### Question 1. Understanding Docker images

Run the command `docker run -it --entrypoint=bash python:3.13`
What's the version of pip in the image?

- [ ] 25.3
- [ ] 24.3.1
- [ ] 24.2.1
- [ ] 23.3.1

### Question 2. Understanding Docker networking and docker-compose
- Given the following `docker-compose.yaml`, what is the hostname and port that pgadmin should use to connect to the postgres database?

In Docker Compose, containers can communicate with each other using the service name as the hostname within the internal Docker network.
From Docker's documentation:
> By default, any service can reach any other service at that service's name. 

Host: db
Port: The PostgreSQL port inside the container is 5432.
- [ ] postgres:5433
- [ ] localhost:5432
- [ ] db:5433
- [ ] postgres:5432
- [x] db:5432
