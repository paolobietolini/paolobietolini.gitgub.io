---
layout: page
title: "Docker Basics"
permalink: /data-engineering/zoomcamp/docker-postgres-terraform
---

## Table of Contents

- [Docker Basics](#docker-basics)
  - [Material](#material)
  - [Notes](#notes)
    - [Volumes](#volumes)
  - [Data Pipelines](#data-pipelines)
    - [Virtual Environment](#virtual-environment)
    - [Dockerfile](#dockerfile)
    - [Alternative: Using PATH instead of `uv run`](#alternative-using-path-instead-of-uv-run)
- [Postgres with Docker](#postgres-with-docker)
    - [NY Taxi Dataset and Data Ingestion](#ny-taxi-dataset-and-data-ingestion)
- [Terraform with Docker](#terraform-with-docker)
# Docker Basics

## Material
- [GitHub: DataTalksClub Data Engineering Zoomcamp](https://github.com/DataTalksClub/data-engineering-zoomcamp/tree/main/01-docker-terraform/docker-sql)
- [Slides](https://docs.google.com/presentation/d/19pXcInDwBnlvKWCukP5sDoCAb69SPqgIoxJ_0Bikr00/edit?slide=id.p#slide=id.p)

## Notes

Docker provides an isolated environment from the system where it runs on.

Running `docker run hello-world` will confirm that Docker is installed.
When running the `docker run` command, if the image doesn't exist locally, it will be pulled remotely.

Adding the flag `-it` to the command `run` will run the container in interactive mode. eg:
`docker run -it ubuntu`

When running this OS, inside Docker, then, it will be isolated from my main OS.

Everytime we run a Docker container we create a container from an image, and our changes inside of it will be lost. It doesn't preserve its state.

Applying `-slim` to the tag (i.e the string after the `:`) will download a smaller version of the image:
`docker run -it python:3.13.11-slim`

To see `bash` instead of the REPL version of Python we can override the entry point by running:
`docker run -it --entrypoint=bash python:latest`


As said containers are stateless, so if I create a file, `echo leavemehere > fsociety.dat` it won't be maintained once I restart/re-run the container.

By running `docker ps -a` we can see all the Exited containers and we can recover the state, so containers are not entirely stateless. When I created the `.dat` file inside the container, it was saved somewhere, as a snapshot of the container itself.


By running `docker ps -aq` I can see the IDs of the exited containers, and by running:
```bash
docker rm `docker ps -aq`
```

`docker ps -a` should show nothing after.


### Volumes

Let's say we have three files on our machine:
```bash
mkdir example
cd example
touch file{1..3}.txt
echo "Hello, World" > file1.txt
```
and we want to list the files and their content using a previously created script `list_files.py`
```python
from pathlib import Path

current_dir = Path.cwd()
current_file = Path(__file__).name

print(f"Files in {current_dir}")
for filepath in current_dir.iterdir():
    if filepath.name == current_file:
        continue

    print(f" - {filepath.name}")

    if filepath.is_file():
        content = filepath.read_text(encoding="utf-8")
        print(f"   Content:\n{content}")
```

I can map the content of this folder to a Python container
```bash
docker run -it \
  --rm \
  -v "$(pwd)/example:/app/example" \
# -w /app/example \ Will take the user directly into the container's directory
  --entrypoint=bash \
  python:latest
```

`-v ${pwd}/example:/app/example`
On the left we have the path of the host machine and on the right the location inside the container


## Data Pipelines

A data pipeline is an automated system that collects raw data from various sources, transforms it (cleans, filters, aggregates) for consistency and usability, and then moves it to a destination like a data warehouse or lake for analysis, reporting, or machine learning

For this Workshop we will use the [NYC Taxis data](https://www.nyc.gov/site/tlc/about/tlc-trip-record-data.page)

The file `pipeline.py` will be our pipeline to ingest data and output it.

### Virtual Environment

If there are missing dependencies we could use Virtual Environments to avoid installing the missing dependencies on our host machine.
A virtual environment is an isolated Python environment that keeps dependencies for the project separate from other projects and the host system.

`uv` will manage the packages for us
`pip install uv`

To initialize a Python project:
`uv init --python=3.13`

To compare the Python versions, i.e the one in the virtual environment, and the one on the host:
```bash
uv run which python  # Python in the virtual environment
uv run python -V

which python        # Python on the host
python -V
```
After creating a project a `.toml` file will be created containing the project details

`uv add pandas` will add the dependency to the project
`uv run python pipeline.py 12` will run our project


---
### Dockerfile

To Dockerize the pipeline we will use the `Dockerfile`, a file containing all the instructions to create a Docker image.

```Dockerfile
FROM python:latest

RUN pip install pandas pyarrow

WORKDIR /code
COPY pipeline.py .
```

After finishing editing the Dockerfile we build the image by using
`docker build -t test:pandas .`
and after the build is complete we run it using:
`docker run -it --entrypoint=bash --rm test:pandas`

using the `--rm` flag will flush all the changes to the container and our host filesystem will less likely have leftovers files.

after running the container we are in the `/code` directory, as stated in the directive `WORKDIR /code`, in the Dockerfile


Now we are executing the Python file by manually running it.
We can change this behaviour by adding
`ENTRYPOINT ["python", "pipeline.py"]` to the Dockerfile.
So now when we run `docker run --rm test:pandas 12` we see the output from the Python file
```bash
> docker run --rm test:pandas 12
  arguments ['pipeline.py', '12']
  Running pipeline for month 12
    day  passengers  month
  0    1           3     12
  1    2           4     12
```

We are still not using `uv` in our Docker container, so we can update the image, we can use the following command in the Dockerfile
`COPY --from=docker.io/astral/uv:latest /uv /bin/`,
So we are copy another Docker image in our.
[DockerHub documentation](https://hub.docker.com/r/astral/uv)

The final Dockerfile will look like this
```Dockerfile
FROM python:latest
COPY --from=ghcr.io/astral-sh/uv:latest /uv /bin/

WORKDIR /code
# We are copying the following host files into the WORKDIR
COPY pyproject.toml .python-version uv.lock ./

# It is going to copy the dependencies in the lock file
RUN uv sync --locked

COPY pipeline.py .

ENTRYPOINT ["uv","run","python", "pipeline.py"]
```

Starting the container will output the same data but now we are using `uv`
```bash
> docker run --rm test:pandas 12
    arguments ['pipeline.py', '12']
    Running pipeline for month 12
      day  passengers  month
    0    1           3     12
    1    2           4     12
```

### Alternative: Using PATH instead of `uv run`

Instead of using `ENTRYPOINT ["uv","run","python", "pipeline.py"]`, we can add this line to the Dockerfile:

```Dockerfile
ENV PATH="/code/.venv/bin:$PATH"
```

And then simplify the entrypoint to:

```Dockerfile
ENTRYPOINT ["python", "pipeline.py"]
```

**How this works:**

When `uv sync` runs, it creates a virtual environment at `/code/.venv/` with all the installed packages. Inside `.venv/bin/` there are executable scripts including `python`, `pip`, and any CLI tools from installed packages.

The `PATH` environment variable is a list of directories (separated by `:`) where the shell looks for executables. When you type `python`, the shell searches each directory in `PATH` from left to right until it finds a matching executable.

By prepending `/code/.venv/bin` to `PATH`:
- The shell will find `/code/.venv/bin/python` **before** the system Python (`/usr/local/bin/python`)
- This virtual environment Python already has all the dependencies installed by `uv sync`
- So running `python pipeline.py` automatically uses the correct Python with all packages available

**Why this is cleaner:**
- No need to prefix every command with `uv run`
- The container behaves like a normal Python environment
- Any script or command that calls `python` will use the virtual environment automatically

**Comparison:**

| Approach | ENTRYPOINT | How it works |
|----------|------------|--------------|
| `uv run` | `["uv","run","python", "pipeline.py"]` | `uv` activates the venv for each command |
| `PATH` | `["python", "pipeline.py"]` | Shell finds venv Python first via PATH |

Both approaches achieve the same result - running Python with the correct dependencies. The `PATH` approach is more idiomatic for Docker containers since it makes the environment "just work" without requiring a wrapper command.


# Postgres with Docker
We can run a containerized version of Postgres that doesn't require any installation steps. We only need to provide a few environment variables to it as well as a volume for storing data.


```bash
    $ mkdir ny_taxi_postgres_data
    $ docker run -it --rm \
      -e POSTGRES_USER="root" \
      -e POSTGRES_PASSWORD="root" \
      -e POSTGRES_DB="ny_taxi" \
      -v ny_taxi_postgres_data:/var/lib/postgresql \
      -p 9868:5432 \
      postgres:18
  ```
- The flags using `-e` are Environmental Variables used to configure the application.
So in this case we are creating a password and a user called `root`, and a database called `ny_taxi`.

- `v` refers to a volume, that is the storage location of the container's file. Hence we can preserve our data for the next time the container is run.

- Finally, the `p` flag refers to the ports mapped from the container to the host machine.

Once the container is running, we can log into our database with [pgcli](https://www.pgcli.com/).

```bash
uv add --dev pgcli
```

The `--dev` flag marks this as a development dependency (not needed in production). It will be added to the [dependency-groups] section of pyproject.toml instead of the main dependencies section.
In production we will use a different tool than `pgcli`.


After installing `pgcli` we can connect to the Postgres db using the following command:
`uv run pgcli -h localhost -p 9868 -u root -d ny_taxi`

- uv run executes a command in the context of the virtual environment
- -h is the host. Since we're running locally we can use localhost.
- -p is the port.
- -u is the username.
- -d is the database name.
- The password is not provided; it will be requested after running the command.
- 
### NY Taxi Dataset and Data Ingestion
We will now create a Jupyter Notebook `notebook.ipynb` file which we will use to read a CSV file and export it to Postgres.

To install Jupyter we use:
```bash
uv add --dev jupyter
```

and to create a Jupyter notebook the command is:
```bash
uv run jupyter notebook
```
This will open a browser window on `localhost:8888/tree`.

We are going to use the [NYC TLC Trip Record Data](https://www.nyc.gov/site/tlc/about/tlc-trip-record-data.page).
Specifically, we will use the [Yellow taxi trip records CSV file for January 2021](https://github.com/DataTalksClub/nyc-tlc-data/releases/download/yellow/yellow_tripdata_2021-01.csv.gz).

The data dictionary of the set is available at:
[NYC TLC Trip Record Data Dictionary](https://www.nyc.gov/assets/tlc/downloads/pdf/data_dictionary_trip_records_yellow.pdf)



**Data Types**
We encountered an error:
```python
/tmp/ipykernel_25483/2933316018.py:1: DtypeWarning: Columns (6) have mixed types. Specify dtype option on import or set low_memory=False.
```

An important difference between .csv and .parquet files is the absence of schemas in the former. 

Having a aschema make it easier for Pandas to infer the data types.

To help Pandas knowing the correct types we need to set the following schema:
```python
dtype = {
    "VendorID": "Int64",
    "passenger_count": "Int64",
    "trip_distance": "float64",
    "RatecodeID": "Int64",
    "store_and_fwd_flag": "string",
    "PULocationID": "Int64",
    "DOLocationID": "Int64",
    "payment_type": "Int64",
    "fare_amount": "float64",
    "extra": "float64",
    "mta_tax": "float64",
    "tip_amount": "float64",
    "tolls_amount": "float64",
    "improvement_surcharge": "float64",
    "total_amount": "float64",
    "congestion_surcharge": "float64"
}

parse_dates = [
    "tpep_pickup_datetime",
    "tpep_dropoff_datetime"
]

df = pd.read_csv(
    'yellow_tripdata_2021-01.csv.gz',
    nrows=100,
    dtype=dtype,
    parse_dates=parse_dates
)
```


**SQLAlchemy**
To add this data to our PostgreSQL database we need a library called SQLAlchemy via Pandas.
To add the library into the Jupiter Notebook we run 
`!uv add sqlalchemy psycopg2-binary`.
`psycopg2` is used to connect to Postgres.
Then we create the engine to  connect to the database:

```python
from sqlalchemy import create_engine
engine = create_engine('postgresql://root:root@localhost:9868/ny_taxi')
```

`df.to_sql()` will insert the data into our database

Before that we are going to print the schema that it is going to be created into the database:
```python
DB_TABLE_NAME = 'yellow_taxi_data'
print(pd.io.sql.get_schema(df, name=DB_TABLE_NAME, con=engine))
```
This will print the follwing SQL command
```sql
CREATE TABLE yellow_taxi_data (
	"VendorID" BIGINT, 
	tpep_pickup_datetime TIMESTAMP WITHOUT TIME ZONE, 
	tpep_dropoff_datetime TIMESTAMP WITHOUT TIME ZONE, 
	passenger_count BIGINT, 
	trip_distance FLOAT(53), 
	"RatecodeID" BIGINT, 
	store_and_fwd_flag TEXT, 
	"PULocationID" BIGINT, 
	"DOLocationID" BIGINT, 
	payment_type BIGINT, 
	fare_amount FLOAT(53), 
	extra FLOAT(53), 
	mta_tax FLOAT(53), 
	tip_amount FLOAT(53), 
	tolls_amount FLOAT(53), 
	improvement_surcharge FLOAT(53), 
	total_amount FLOAT(53), 
	congestion_surcharge FLOAT(53)
)
```

Now we can run:
```python
df.head(n=0).to_sql(name=DB_TABLE_NAME, con=engine, if_exists='replace')
```
This will create the table if it won't exist already by runinng the `CREATE TABLE` command.
`head(n=0)` makes sure we only create the table, we don't add any data yet.


By running `pgcli` we will see that in the Postgres database we will have correctly created the table:

```bash
root@localhost:ny_taxi> \dt
+--------+------------------+-------+-------+
| Schema | Name             | Type  | Owner |
|--------+------------------+-------+-------|
| public | yellow_taxi_data | table | root  |
+--------+------------------+-------+-------+
```


**Ingesting Data in Chunks**

We can now start inserting data into our table.
We cannot ingest the data all at once as it will take long time and we will not know the status.
So we split it in chunks of equal size.
For this task we can use an iterator.

Our chunk will be 100000 elements big

```python
df_iter = pd.read_csv(
    filename,
    dtype=dtype,
    parse_dates=parse_dates,
    iterator=True,
    chunksize=100000
)
```

We can use a for-loop (or the `next()` method) to iterate over the list.
```python
first = True
for df_chunk in df_iter:
    if first:
    df_chunk.head(0).to_sql(
      name=DB_TABLE_NAME,
      con=engine,
      if_exists='replace'
    )
    first = False
    print(f'Table {DB_TABLE_NAME} created')
  df_chunk.to_sql(
    name=DB_TABLE_NAME,
    con=engine,
    if_exists='append'
  )
  print("Inserted:", len(df_chunk))
```

Alternative approach using `next()`
```python
first_chunk = next(df_iter)

first_chunk.head(0).to_sql(
    name="yellow_taxi_data",
    con=engine,
    if_exists="replace"
)

print("Table created")

first_chunk.to_sql(
    name="yellow_taxi_data",
    con=engine,
    if_exists="append"
)

print("Inserted first chunk:", len(first_chunk))

for df_chunk in df_iter:
    df_chunk.to_sql(
        name="yellow_taxi_data",
        con=engine,
        if_exists="append"
    )
    print("Inserted chunk:", len(df_chunk))
```

To add a progress bar we can run:
`!uv add tqdm`
adn wrap it around the iterable:
```python
# Iterate over the DataFrame to insert the chunks into the database
first = True

for df_chunk in tqdm(df_iter):
    if first:
        df_chunk.head(0).to_sql(
            name=DB_TABLE_NAME,
            con=engine,
            if_exists="replace",
            index=False
        )
        first = False
        print("Table created")

    df_chunk.to_sql(
        name=DB_TABLE_NAME,
        con=engine,
        if_exists="append",
        index=False,
        chunksize=10_000,
        method="multi"
    )

    print("Inserted:", len(df_chunk))
```


**Creating the data ingestion script**

Now let's convert the notebook to a Python script:
```bash
uv run jupyter nbconvert --to=script notebook.ipynb
mv notebook.py ingest_data.py
```

In questo modo avremo uno script Python pronto per essere dockerizzato:

```python
import pandas as pd
from sqlalchemy import create_engine
from tqdm.auto import tqdm

# user="root",
# password="root",
# host="localhost",
# port="9868",
# db="ny_taxi",
# table="yellow_taxi_data",
# year=2021,
# month=1
chunk_size = 10_000


def ingest_data(user, password, host, port, db, table, year, month):
    file_name = f"yellow_tripdata_{year}-{month:02d}.csv.gz"
    engine = create_engine(f"postgresql://{user}:{password}@{host}:{port}/{db}")
    dtype = {
        "VendorID": "Int64",
        "passenger_count": "Int64",
        "trip_distance": "float64",
        "RatecodeID": "Int64",
        "store_and_fwd_flag": "string",
        "PULocationID": "Int64",
        "DOLocationID": "Int64",
        "payment_type": "Int64",
        "fare_amount": "float64",
        "extra": "float64",
        "mta_tax": "float64",
        "tip_amount": "float64",
        "tolls_amount": "float64",
        "improvement_surcharge": "float64",
        "total_amount": "float64",
        "congestion_surcharge": "float64",
    }
    parse_dates = ["tpep_pickup_datetime", "tpep_dropoff_datetime"]
    df_iter = pd.read_csv(
        file_name,
        dtype=dtype,
        parse_dates=parse_dates,
        iterator=True,
        chunksize=chunk_size,
    )
    first = True
    for df_chunk in tqdm(df_iter):
        if first:
            df_chunk.head(0).to_sql(
                name=table, con=engine, if_exists="replace", index=False
            )
            first = False
            print("Table created")
        df_chunk.to_sql(
            name=table,
            con=engine,
            if_exists="append",
            index=False,
            chunksize=chunk_size,
            method="multi",
        )
        print("Inserted:", len(df_chunk))


if __name__ == "__main__":
    ingest_data(
        user="root",
        password="root",
        host="localhost",
        port="9868",
        db="ny_taxi",
        table="yellow_taxi_data",
        year=2021,
        month=1,
    )
```

Prima pero' dobbiamo fare in modo che i parametri della funzione siano configurabili da CLI, so we are gonna use a library called `click`
```python
@click.command()
@click.option('--user', default='root', help='PostgreSQL user')
@click.option('--password', default='root', help='PostgreSQL password')
@click.option('--host', default='localhost', help='PostgreSQL host')
@click.option('--port', default=9868, type=int, help='PostgreSQL port')
@click.option('--db', default='ny_taxi', help='PostgreSQL database name')
@click.option('--table', default='yellow_taxi_data', help='Target table name')
@click.option('--year', type=int, default=2021, help='Year of the data to ingest')
@click.option('--month', type=int, default=1, help='Month of the data to ingest (1-12)')
```

Now we're ready to dockerize our function, just by changing our Dockerfile

```Dockerfile
# Start with slim Python 3.13 image
FROM python:3.13.10-slim

# Copy uv binary from official uv image (multi-stage build pattern)
COPY --from=ghcr.io/astral-sh/uv:latest /uv /bin/

# Set working directory
WORKDIR /code

# Add virtual environment to PATH so we can use installed packages
ENV PATH="/code/.venv/bin:$PATH"

# Copy dependency files first (better layer caching)
COPY "pyproject.toml" "uv.lock" ".python-version" ./
# Install dependencies from lock file (ensures reproducible builds)
RUN uv sync --locked

# Copy application code
COPY ingest_data.py .

# Set entry point
ENTRYPOINT ["python", "ingest_data.py"]
```

Then we build the Docker image

```bash
cd pipeline
docker build -t taxi_ingest:v001 .
```
and run it 

```bash
docker run -it \
  --network=pg-network \
  taxi_ingest:v001 \
    --user=root \
    --pass=root \
    --host=pgdatabase \ # This is the name of the Postgres container
    --port=9868 \
    --db=ny_taxi \
    --target-table=yellow_taxi_data \
    --month=12
```

- We need to provide the network for Docker to find the Postgres container. It goes before the name of the image.
- Since Postgres is running on a separate container, the host argument will have to point to the container name of Postgres (`pgdatabase`).
- You can drop the table in pgAdmin beforehand if you want, but the script will automatically replace the pre-existing table.

To create a network we use `docker network pg-network`.
Then we will have to update the Postgres container's network to match the one for the ingestion script
```bash
    docker run -it --rm \
    --network=pg-network \ 
    --name=pgdatabase
      -e POSTGRES_USER="root" \
      -e POSTGRES_PASSWORD="root" \
      -e POSTGRES_DB="ny_taxi" \
      -v ny_taxi_postgres_data:/var/lib/postgresql \
      -p 9868:5432 \
      postgres:18
```


To move away from `pgcli` we can use `pgadmin`, a UI used to manage Postgres.
To enable it, run:
```bash
docker run -it \ 
    -e PGADMIN_DEFAULT_EMAIL="admin@admin.com"
    -e PGADMIN_DEFAULT_PASSWORD="root" \
    -v pgadmin_data:/var/lib/pgadmin \
    -p 8085:80
    --networdk=pg-network \
    --name pgadmin \
    dbpage/pgadmin4
```

To run the containers at the same time we can use a Docker compose file, `docker-compose.yml`

```yaml
services:
  pgdatabase:
    image: postgres:18
    environment:
      POSTGRES_USER: "root"
      POSTGRES_PASSWORD: "root"
      POSTGRES_DB: "ny_taxi"
    volumes:
      - "ny_taxi_postgres_data:/var/lib/postgresql"
    ports:
      - "9698:5432"
 

  pgadmin:
    image: dpage/pgadmin4
    environment:
      PGADMIN_DEFAULT_EMAIL: "admin@admin.com"
      PGADMIN_DEFAULT_PASSWORD: "root"
    volumes:
      - "pgadmin_data:/var/lib/pgadmin"
    ports:
      - "8085:80"



volumes:
  ny_taxi_postgres_data:
  pgadmin_data:
```

- We don't have to specify a network because docker compose takes care of it: every single container (or "service", as the file states) will run within the same network and will be able to find each other according to their names (`pgdatabase` and `pgadmin` in this example).
- All other details from the docker run commands (environment variables, volumes and ports) are mentioned accordingly in the file following YAML syntax.

To run the containers using the compose file we use 
```bash
docker compose up
```

To run the ingestion script using Docker Compose we will have to find the name of the virtual network used by the ingestion container.
We can use 
```bash
# check the network link:
docker network ls

# it's pipeline_default (or similar based on directory name)
# now run the script:
docker run -it --rm\
  --network=pipeline_default \
  taxi_ingest:v001 \
    --pg-user=root \
    --pg-pass=root \
    --pg-host=pgdatabase \
    --pg-port=5432 \
    --pg-db=ny_taxi \
    --target-table=yellow_taxi_trips
```


# Terraform with Docker
(WIP)