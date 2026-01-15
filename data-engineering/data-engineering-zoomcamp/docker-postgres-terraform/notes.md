## Material
https://github.com/DataTalksClub/data-engineering-zoomcamp/tree/main/01-docker-terraform/docker-sql


- Slides
  https://docs.google.com/presentation/d/19pXcInDwBnlvKWCukP5sDoCAb69SPqgIoxJ_0Bikr00/edit?slide=id.p#slide=id.p

## Notes
Docker provides a isolated environment from the system where it runs on.

Running `docker run hello-world` will confirm that Docker is installed.
When running the `docker run` commadn, if the image doesn't exists locally, it will be pulled remotely

Adding the flag `-it` to the command `run` will run the container in interactive mode. eg:
`docker run -it ubuntu`

When running this OS, inside Docker, then, it will be isolated from my main OS.

Everytime we run a Docker container we create a container from an image, and our changes inside of it will be lost. It doesn't preserve its state.

Applyting `-slim` to the tag (i.e the string after the `:`) will download a smaller version of the image
`docker run -it python 3:13.11-slim`

To see `bash` instead of the REPL version of Python qe can override the entry point by running:
`docker run -it --entrypoint=bash python:latest`


as said containers are stateless, so if I create a file, `echo leavemehere > fsociety.dat` it won't be maintained once I restart/re-run the container

by running `docker ps -a` we can see all the Exited containers and we can recover the state, so containers are not entirely stateless. When I created the `.dat` file inside the container, it was saved somewhere, as a snapshot of the container itself.


By running `docker ps -aq` I can see the IDs of the exited containers, and by running 
```bash
docker rm `docker ps -aq`
```

`docker ps -a` should show nothing after


### Volumes
Let's say we have three files on our machine:
```bash
mkdir example
cd example
touch file{1..3}.txt
echo "Hello, World" > file1.txt
```
and we want to list the files and their content using a previously created script`list_files.py`
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
On the right we have the path of the host machine and on the left the location inside the container


## Data Pipelines
A data pipeline is an automated system that collects raw data from various sources, transforms it (cleans, filters, aggregates) for consistency and usability, and then moves it to a destination like a data warehouse or lake for analysis, reporting, or machine learning

For this Workshop we will use the [NYC Taxis data](https://www.nyc.gov/site/tlc/about/tlc-trip-record-data.page)

The file `pipeline.py` will be our pipleine to ingest data and output it.

### Virtual Environment
If there are missing depndencies we could use Virtual Environments to avoid installing the missing dependencies on our host machine
A virtual environment, it is an isolated Python environment that keeps dependencies for the project separate from other projects and the host system

`uv` will manage the packages for us
`pip install uv`

To initialize a Python project:
`uv init --python=3.13`

To compare the Python versions, i.e the one in the virtual environment, and the one on the host 
```bash
uv run which python  # Python in the virtual environment
uv run python -V

which python        # Python on the host
python -V
```
After creating a project a `.toml` file will be created containing the project details

`uv add pandas` will add the dependency to the project
`uv run python pipeline.py 12` will run our project
