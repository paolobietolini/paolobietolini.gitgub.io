FROM python:3.13

COPY --from=ghcr.io/astral-sh/uv:latest /uv /bin/

WORKDIR /code

ENV PATH="/code/.venv/bin:$PATH"

COPY "pyproject.toml" "uv.lock" ".python-version" ./

RUN uv sync --locked

COPY data-ingestion.py .
COPY green_tripdata_2025-11.parquet taxi_zone_lookup.csv ./

ENTRYPOINT ["python", "data-ingestion.py"]