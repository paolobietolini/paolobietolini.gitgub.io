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
- [x] postgres:5432  > This might work. Containers name can be used for DNS resolution 
- [x] db:5432

### Question 3. Counting short trips
For the trips in November 2025 (lpep_pickup_datetime between '2025-11-01' and '2025-12-01', exclusive of the upper bound), how many trips had a trip_distance of less than or equal to 1 mile?

- [ ] 7,853
- [x] 8,007
- [ ] 8,254
- [ ] 8,421

```sql
SELECT
count(*) as TRIPS
FROM green_trips
WHERE lpep_pickup_datetime >= '2025-11-01' 
  AND lpep_pickup_datetime < '2025-12-01'
  AND trip_distance <= 1
```

### Question 4. Longest trip for each day
Which was the pick up day with the longest trip distance? Only consider trips with trip_distance less than 100 miles (to exclude data errors).

Use the pick up time for your calculations.

- [x] 2025-11-14
- [ ] 2025-11-20
- [ ] 2025-11-23
- [ ] 2025-11-25

```sql
SELECT date(lpep_pickup_datetime), trip_distance
FROM green_trips
WHERE trip_distance < 100.00
ORDER BY trip_distance DESC
LIMIT 1
```

### Question 5. Biggest pickup zone
Which was the pickup zone with the largest total_amount (sum of all trips) on November 18th, 2025?

- [x] East Harlem North
- [ ] East Harlem South
- [ ] Morningside Heights
- [ ] Forest Hills

```sql
SELECT 
    taxi_zones.Zone,
    SUM(green_trips.total_amount) as total_amount
FROM green_trips
JOIN taxi_zones ON green_trips.PULocationID = taxi_zones.LocationID
WHERE DATE(green_trips.lpep_pickup_datetime) = '2025-11-18'
GROUP BY taxi_zones.Zone
ORDER BY total_amount DESC
LIMIT 1
```

### Question 6. Largest tip
For the passengers picked up in the zone named "East Harlem North" in November 2025, which was the drop off zone that had the largest tip?

Note: it's tip , not trip. We need the name of the zone, not the ID.

- [ ] JFK Airport
- [x] Yorkville West
- [ ] East Harlem North
- [ ] LaGuardia Airport

```sql
SELECT 
    dropoff_zones.Zone as dropoff_zone,
    green_trips.tip_amount
FROM green_trips
JOIN taxi_zones AS pickup_zones ON green_trips.PULocationID = pickup_zones.LocationID
JOIN taxi_zones AS dropoff_zones ON green_trips.DOLocationID = dropoff_zones.LocationID
WHERE pickup_zones.Zone = 'East Harlem North' 
    AND green_trips.lpep_pickup_datetime >= '2025-11-01'
    AND green_trips.lpep_pickup_datetime < '2025-12-01'
ORDER BY green_trips.tip_amount DESC
LIMIT 1
```

### Question 7. Terraform Workflow
Which of the following sequences, respectively, describes the workflow for:

- Downloading the provider plugins and setting up backend,
- Generating proposed changes and auto-executing the plan
- Remove all resources managed by terraform

Answers:
- [ ] terraform import, terraform apply -y, terraform destroy
- [ ] teraform init, terraform plan -auto-apply, terraform rm
- [ ] terraform init, terraform run -auto-approve, terraform destroy
- [x] terraform init, terraform apply -auto-approve, terraform destroy
- [ ] terraform import, terraform apply -y, terraform rm