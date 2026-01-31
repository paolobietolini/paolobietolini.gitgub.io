---
layout: default
title: "Workflow Orchestration with Kestra"
permalink: /data-engineering/zoomcamp/workflow-orchestration-kestra
---

## Table of Contents

- [Introduction to Workflow Orchestration](#introduction-to-workflow-orchestration)
  - [What is Workflow Orchestration?](#what-is-workflow-orchestration)
  - [What is Kestra?](#what-is-kestra)
- [Getting Started with Kestra](#getting-started-with-kestra)
  - [Installing Kestra with Docker Compose](#installing-kestra-with-docker-compose)
  - [Kestra Concepts](#kestra-concepts)
  - [Orchestrating Python Code](#orchestrating-python-code)
- [Building ETL Data Pipelines](#building-etl-data-pipelines)
  - [Getting Started Pipeline](#getting-started-pipeline)
  - [Loading Taxi Data to Postgres](#loading-taxi-data-to-postgres)
  - [Scheduling and Backfills](#scheduling-and-backfills)
- [ELT Pipelines with Google Cloud Platform](#elt-pipelines-with-google-cloud-platform)
  - [ETL vs ELT](#etl-vs-elt)
  - [Setting Up GCP](#setting-up-gcp)
  - [Loading Taxi Data to BigQuery](#loading-taxi-data-to-bigquery)
  - [Scheduling GCP Workflows](#scheduling-gcp-workflows)
- [Using AI for Data Engineering](#using-ai-for-data-engineering)
  - [Why AI for Workflows?](#why-ai-for-workflows)
  - [Context Engineering](#context-engineering)
  - [Kestra AI Copilot](#kestra-ai-copilot)
  - [Retrieval Augmented Generation (RAG)](#retrieval-augmented-generation-rag)
- [Deploying to the Cloud](#deploying-to-the-cloud)

---

# Introduction to Workflow Orchestration

## What is Workflow Orchestration?

Think of a music orchestra: multiple instruments with different roles, all coordinated by a conductor to play together in harmony. Workflow orchestration applies the same concept to data tools and platforms.

A **workflow orchestrator** coordinates multiple tools and systems, ensuring they work together at the right time. It handles:

- **Running workflows** with predefined steps
- **Monitoring and logging** errors, with automated responses when they occur
- **Scheduling** workflows based on time or events
- **Managing dependencies** between tasks and data flows

In data engineering, we often need to move data from one place to another, sometimes transforming it along the way. An orchestrator manages these steps while providing visibility into the entire process.

**Common orchestration scenarios:**
- Extract data from an API daily at 6 AM
- Transform and load data after new files arrive in cloud storage
- Retry failed tasks with exponential backoff
- Send alerts when pipelines fail

---

## What is Kestra?

[Kestra](https://kestra.io) is an open-source, event-driven orchestration platform designed for building both scheduled and event-driven workflows.

**Key features:**
- **Flow as Code (YAML)** - Define workflows declaratively, no complex programming required
- **1000+ Plugins** - Integrate with databases, cloud providers, APIs, and more
- **Language Agnostic** - Run Python, Shell, R, Node.js, or any language
- **Flexible Triggers** - Schedule-based or event-based execution
- **Visual UI** - Monitor, debug, and manage workflows from a web interface
- **AI Copilot** - Generate workflows using natural language

---

# Getting Started with Kestra

## Installing Kestra with Docker Compose

Kestra runs as a Docker container alongside a Postgres database for storing metadata. We can extend the setup from Module 1 to include Kestra.

### Docker Compose Configuration

The `docker-compose.yml` file sets up four services:
1. **pgdatabase** - Postgres for our taxi data (from Module 1)
2. **pgadmin** - Web UI for managing Postgres
3. **kestra_postgres** - Dedicated Postgres for Kestra's internal state
4. **kestra** - The orchestration server

```yaml
services:
  pgdatabase:
    image: postgres:17
    environment:
      POSTGRES_USER: "root"
      POSTGRES_PASSWORD: "root"
      POSTGRES_DB: "ny_taxi"
    volumes:
      - "ny_taxi_postgres_data:/var/lib/postgresql/data"
    ports:
      - "9868:5432"
    networks:
      - pg-network
    healthcheck:
      test: [ "CMD-SHELL", "pg_isready -d ny_taxi -U root" ]
      interval: 10s
      timeout: 5s
      retries: 5

  pgadmin:
    image: dpage/pgadmin4
    environment:
      PGADMIN_DEFAULT_EMAIL: "admin@admin.com"
      PGADMIN_DEFAULT_PASSWORD: "root"
    volumes:
      - "pgadmin_data:/var/lib/pgadmin"
    ports:
      - "8085:80"
    networks:
      - pg-network
    depends_on:
      - pgdatabase

  kestra_postgres:
    image: postgres:18
    volumes:
      - kestra_postgres_data:/var/lib/postgresql
    environment:
      POSTGRES_DB: kestra
      POSTGRES_USER: kestra
      POSTGRES_PASSWORD: k3str4
    healthcheck:
      test: ["CMD-SHELL", "pg_isready -d $${POSTGRES_DB} -U $${POSTGRES_USER}"]
      interval: 30s
      timeout: 10s
      retries: 10
    networks:
      - pg-network

  kestra:
    image: kestra/kestra:v1.1
    pull_policy: always
    user: "root"
    command: server standalone
    volumes:
      - kestra_data:/app/storage
      - /var/run/docker.sock:/var/run/docker.sock
      - /tmp/kestra-wd:/tmp/kestra-wd
    environment:
      KESTRA_CONFIGURATION: |
        datasources:
          postgres:
            url: jdbc:postgresql://kestra_postgres:5432/kestra
            driverClassName: org.postgresql.Driver
            username: kestra
            password: k3str4
        kestra:
          server:
            basicAuth:
              username: "admin@kestra.io"
              password: Admin1234!
          repository:
            type: postgres
          storage:
            type: local
            local:
              basePath: "/app/storage"
          queue:
            type: postgres
          tasks:
            tmpDir:
              path: /tmp/kestra-wd/tmp
          url: http://localhost:8080/
    ports:
      - "8080:8080"
      - "8082:8081"
    networks:
      - pg-network
    depends_on:
      kestra_postgres:
        condition: service_healthy

volumes:
  ny_taxi_postgres_data:
  pgadmin_data:
  kestra_postgres_data:
  kestra_data:

networks:
  pg-network:
    driver: bridge
```

### Starting Kestra

```bash
cd module-2/kestra
docker compose up -d
```

Access the Kestra UI at [http://localhost:8080](http://localhost:8080)

**Default credentials:**
- Username: `admin@kestra.io`
- Password: `Admin1234!`

To shut down:
```bash
docker compose down
```

**Troubleshooting:**
- If port 8080 is in use (e.g., by pgAdmin), modify the Kestra ports to something like `18080:8080`
- Always pin Kestra to version `v1.1` for reproducibility; avoid `develop` images

---

## Kestra Concepts

Understanding these core concepts is essential for building workflows in Kestra:

### Flow

A **Flow** is a container for tasks and their orchestration logic. Flows are defined in YAML.

```yaml
id: my_first_flow
namespace: zoomcamp
description: A simple hello world flow

tasks:
  - id: hello
    type: io.kestra.plugin.core.log.Log
    message: Hello, World!
```

### Tasks

**Tasks** are the individual steps within a flow. Each task has:
- A unique `id`
- A `type` that determines what it does
- Configuration properties specific to that task type

```yaml
tasks:
  - id: extract_data
    type: io.kestra.plugin.core.http.Download
    uri: https://example.com/data.csv

  - id: log_result
    type: io.kestra.plugin.core.log.Log
    message: "Downloaded file: {{ outputs.extract_data.uri }}"
```

### Inputs

**Inputs** are dynamic values passed to a flow at runtime. They allow flows to be reusable with different parameters.

```yaml
inputs:
  - id: taxi_type
    type: SELECT
    required: true
    values:
      - yellow
      - green
    defaults: yellow

  - id: year
    type: INT
    required: true
    defaults: 2021

  - id: month
    type: INT
    required: true
    defaults: 1
```

Access inputs in tasks using: `{{ inputs.taxi_type }}`

### Outputs

**Outputs** pass data between tasks. Each task can produce outputs that subsequent tasks consume.

```yaml
tasks:
  - id: return_value
    type: io.kestra.plugin.core.debug.Return
    format: "The year is {{ inputs.year }}"

  - id: log_output
    type: io.kestra.plugin.core.log.Log
    message: "{{ outputs.return_value.value }}"
```

### Triggers

**Triggers** automatically start flow executions based on schedules or events.

**Schedule Trigger** - Run at specific times (cron syntax):
```yaml
triggers:
  - id: daily_schedule
    type: io.kestra.plugin.core.trigger.Schedule
    cron: "0 9 * * *"  # Every day at 9 AM UTC
```

**Flow Trigger** - Run when another flow completes:
```yaml
triggers:
  - id: after_extraction
    type: io.kestra.plugin.core.trigger.Flow
    conditions:
      - type: io.kestra.plugin.core.condition.ExecutionFlowCondition
        flowId: extraction_flow
        namespace: zoomcamp
```

### Variables

**Variables** are key-value pairs for reusing values across tasks.

```yaml
variables:
  base_url: "https://github.com/DataTalksClub/nyc-tlc-data/releases/download"
  file_name: "{{ inputs.taxi_type }}_tripdata_{{ inputs.year }}-{{ inputs.month | numberFormat('00') }}.csv.gz"

tasks:
  - id: download
    type: io.kestra.plugin.core.http.Download
    uri: "{{ vars.base_url }}/{{ inputs.taxi_type }}/{{ vars.file_name }}"
```

### Plugin Defaults

**Plugin Defaults** apply default values to all tasks of a specific type.

```yaml
pluginDefaults:
  - type: io.kestra.plugin.core.log.Log
    values:
      level: ERROR
```

### Concurrency

**Concurrency** controls how many executions of a flow can run simultaneously.

```yaml
concurrency:
  limit: 2
  behavior: FAIL  # Options: QUEUE, CANCEL, FAIL
```

### Execution

An **Execution** is a single run of a flow. Each execution has:
- A unique ID
- A state (RUNNING, SUCCESS, FAILED, etc.)
- Logs and outputs from each task
- A Gantt chart showing task timing

---

## Orchestrating Python Code

Kestra can execute Python scripts directly within workflows, allowing you to leverage existing Python code or write custom logic.

### Running Python in Kestra

```yaml
id: python_example
namespace: zoomcamp

tasks:
  - id: run_python
    type: io.kestra.plugin.scripts.python.Commands
    docker:
      image: python:3.11-slim
    beforeCommands:
      - pip install requests
    commands:
      - python main.py
    namespaceFiles:
      enabled: true
```

### Inline Python with Outputs

You can write Python directly in the YAML and capture outputs:

```yaml
tasks:
  - id: fetch_dockerhub_pulls
    type: io.kestra.plugin.scripts.python.Script
    docker:
      image: python:3.11-slim
    beforeCommands:
      - pip install requests
    script: |
      import requests

      response = requests.get("https://hub.docker.com/v2/repositories/kestra/kestra")
      data = response.json()
      pull_count = data.get("pull_count", 0)

      # Output the value to Kestra
      print(f"::set-output name=pulls::{pull_count}")

  - id: log_pulls
    type: io.kestra.plugin.core.log.Log
    message: "Docker pulls: {{ outputs.fetch_dockerhub_pulls.vars.pulls }}"
```

**Key points:**
- `docker.image` specifies the container image for running the script
- `beforeCommands` installs dependencies before the main script runs
- Use `::set-output name=KEY::VALUE` to pass values to Kestra as outputs
- Access outputs via `{{ outputs.task_id.vars.key }}`

---

# Building ETL Data Pipelines

## Getting Started Pipeline

Before working with the taxi dataset, let's understand a simple ETL pipeline pattern:

```
Extract (HTTP API) → Transform (Python) → Query (DuckDB)
```

This flow demonstrates:
1. **Extract** - Download data from a REST API
2. **Transform** - Process data with Python
3. **Query** - Analyze with DuckDB (in-memory SQL)

```yaml
id: getting_started_pipeline
namespace: zoomcamp

tasks:
  - id: extract
    type: io.kestra.plugin.core.http.Download
    uri: https://dummyjson.com/products

  - id: transform
    type: io.kestra.plugin.scripts.python.Script
    docker:
      image: python:3.11-slim
    beforeCommands:
      - pip install pandas
    script: |
      import pandas as pd
      import json

      with open("{{ outputs.extract.uri }}", "r") as f:
          data = json.load(f)

      df = pd.DataFrame(data["products"])
      df.to_csv("products.csv", index=False)
    outputFiles:
      - products.csv

  - id: query
    type: io.kestra.plugin.jdbc.duckdb.Query
    sql: |
      SELECT category, COUNT(*) as count, AVG(price) as avg_price
      FROM read_csv_auto('{{ outputs.transform.outputFiles["products.csv"] }}')
      GROUP BY category
      ORDER BY count DESC
```

---

## Loading Taxi Data to Postgres

Now we build a real ETL pipeline for NYC Yellow and Green Taxi data.

### Pipeline Architecture

```
Select Year/Month → Extract CSV → Create Tables → Load Data → Merge to Final Table
```

### Flow Definition

```yaml
id: postgres_taxi
namespace: zoomcamp

inputs:
  - id: taxi
    type: SELECT
    required: true
    values:
      - yellow
      - green
    defaults: yellow

  - id: year
    type: INT
    required: true
    defaults: 2019

  - id: month
    type: INT
    required: true
    defaults: 1

variables:
  file_name: "{{ inputs.taxi }}_tripdata_{{ inputs.year }}-{{ inputs.month | numberFormat('00') }}.csv.gz"
  staging_table: "{{ inputs.taxi }}_tripdata_{{ inputs.year }}_{{ inputs.month | numberFormat('00') }}"
  final_table: "{{ inputs.taxi }}_tripdata"
  data_url: "https://github.com/DataTalksClub/nyc-tlc-data/releases/download/{{ inputs.taxi }}/{{ vars.file_name }}"

tasks:
  - id: set_label
    type: io.kestra.plugin.core.execution.Labels
    labels:
      taxi: "{{ inputs.taxi }}"
      file: "{{ vars.file_name }}"

  - id: extract
    type: io.kestra.plugin.core.http.Download
    uri: "{{ vars.data_url }}"

  - id: create_final_table
    type: io.kestra.plugin.jdbc.postgresql.Query
    url: jdbc:postgresql://pgdatabase:5432/ny_taxi
    username: root
    password: root
    sql: |
      CREATE TABLE IF NOT EXISTS {{ vars.final_table }} (
        unique_row_id TEXT,
        filename TEXT,
        VendorID INT,
        tpep_pickup_datetime TIMESTAMP,
        tpep_dropoff_datetime TIMESTAMP,
        passenger_count INT,
        trip_distance DOUBLE PRECISION,
        RatecodeID INT,
        store_and_fwd_flag TEXT,
        PULocationID INT,
        DOLocationID INT,
        payment_type INT,
        fare_amount DOUBLE PRECISION,
        extra DOUBLE PRECISION,
        mta_tax DOUBLE PRECISION,
        tip_amount DOUBLE PRECISION,
        tolls_amount DOUBLE PRECISION,
        improvement_surcharge DOUBLE PRECISION,
        total_amount DOUBLE PRECISION,
        congestion_surcharge DOUBLE PRECISION
      );

  - id: create_staging_table
    type: io.kestra.plugin.jdbc.postgresql.Query
    url: jdbc:postgresql://pgdatabase:5432/ny_taxi
    username: root
    password: root
    sql: |
      CREATE TABLE IF NOT EXISTS {{ vars.staging_table }} (
        LIKE {{ vars.final_table }} INCLUDING ALL
      );
      TRUNCATE TABLE {{ vars.staging_table }};

  - id: load_data
    type: io.kestra.plugin.jdbc.postgresql.CopyIn
    url: jdbc:postgresql://pgdatabase:5432/ny_taxi
    username: root
    password: root
    table: "{{ vars.staging_table }}"
    from: "{{ outputs.extract.uri }}"
    format: CSV
    header: true
    delimiter: ","

  - id: merge_to_final
    type: io.kestra.plugin.jdbc.postgresql.Query
    url: jdbc:postgresql://pgdatabase:5432/ny_taxi
    username: root
    password: root
    sql: |
      INSERT INTO {{ vars.final_table }}
      SELECT * FROM {{ vars.staging_table }}
      ON CONFLICT (unique_row_id) DO NOTHING;
```

### Understanding the Flow

1. **Inputs** - User selects taxi type, year, and month
2. **Variables** - Construct file names and URLs dynamically
3. **Extract** - Download CSV from GitHub releases
4. **Create Tables** - Ensure destination tables exist
5. **Load** - Use `CopyIn` for bulk insert (faster than row-by-row)
6. **Merge** - Insert new records, skip duplicates

---

## Scheduling and Backfills

### Adding a Schedule Trigger

```yaml
triggers:
  - id: daily_at_9am
    type: io.kestra.plugin.core.trigger.Schedule
    cron: "0 9 * * *"
    inputs:
      taxi: green
```

### Backfilling Historical Data

Kestra supports **backfilling** - running a scheduled flow for past dates as if the schedule had been active.

**Steps to backfill:**
1. Go to the flow in Kestra UI
2. Click the **Triggers** tab
3. Select the schedule trigger
4. Click **Backfill**
5. Choose the date range (e.g., 2019-01-01 to 2019-12-31)
6. Kestra creates executions for each scheduled interval

This is essential for:
- Loading historical data when setting up a new pipeline
- Reprocessing data after fixing bugs
- Filling gaps from downtime

---

# ELT Pipelines with Google Cloud Platform

## ETL vs ELT

In Module 1 and earlier sections, we used **ETL** (Extract, Transform, Load):
1. Extract from source
2. Transform with Python/Pandas locally
3. Load to Postgres

For large datasets and cloud warehouses, **ELT** (Extract, Load, Transform) is often better:
1. Extract from source
2. Load raw data to cloud storage (data lake)
3. Transform using the warehouse's compute power

**Why ELT for BigQuery?**
- BigQuery handles transformations on petabytes of data
- No local memory constraints
- SQL-based transformations are declarative and auditable
- Separation of storage (GCS) and compute (BigQuery)

```
ETL: Source → Transform (local) → Load (DB)
ELT: Source → Load (GCS) → Transform (BigQuery)
```

---

## Setting Up GCP

### Prerequisites

1. **GCP Project** - Create or use existing project
2. **Service Account** - With permissions for GCS and BigQuery
3. **API Keys** - Download JSON credentials

### Required IAM Roles

- `roles/storage.admin` - Create buckets and upload files
- `roles/bigquery.admin` - Create datasets and tables

### Configuring Kestra with GCP Credentials

Store credentials in Kestra's **KV Store** (key-value storage):

```yaml
id: gcp_setup_kv
namespace: zoomcamp

tasks:
  - id: set_gcp_project
    type: io.kestra.plugin.core.kv.Set
    key: GCP_PROJECT_ID
    value: your-project-id

  - id: set_gcp_location
    type: io.kestra.plugin.core.kv.Set
    key: GCP_LOCATION
    value: EU

  - id: set_bucket_name
    type: io.kestra.plugin.core.kv.Set
    key: GCP_BUCKET_NAME
    value: your-bucket-name

  - id: set_dataset
    type: io.kestra.plugin.core.kv.Set
    key: GCP_DATASET
    value: zoomcamp_dataset
```

### Creating GCS Bucket and BigQuery Dataset

```yaml
id: gcp_create_resources
namespace: zoomcamp

tasks:
  - id: create_bucket
    type: io.kestra.plugin.gcp.gcs.CreateBucket
    projectId: "{{ kv('GCP_PROJECT_ID') }}"
    name: "{{ kv('GCP_BUCKET_NAME') }}"
    location: "{{ kv('GCP_LOCATION') }}"

  - id: create_dataset
    type: io.kestra.plugin.gcp.bigquery.CreateDataset
    projectId: "{{ kv('GCP_PROJECT_ID') }}"
    dataset: "{{ kv('GCP_DATASET') }}"
    location: "{{ kv('GCP_LOCATION') }}"
```

---

## Loading Taxi Data to BigQuery

### ELT Pipeline Architecture

```
Extract CSV → Upload to GCS → Create External Table → Create Staging Table → Merge to Final
```

### Flow Definition

```yaml
id: gcp_taxi
namespace: zoomcamp

inputs:
  - id: taxi
    type: SELECT
    values: [yellow, green]
    defaults: yellow

  - id: year
    type: INT
    defaults: 2019

  - id: month
    type: INT
    defaults: 1

variables:
  file_name: "{{ inputs.taxi }}_tripdata_{{ inputs.year }}-{{ inputs.month | numberFormat('00') }}.csv.gz"
  gcs_path: "gs://{{ kv('GCP_BUCKET_NAME') }}/{{ inputs.taxi }}/{{ vars.file_name }}"
  project_id: "{{ kv('GCP_PROJECT_ID') }}"
  dataset: "{{ kv('GCP_DATASET') }}"
  table_ext: "{{ inputs.taxi }}_tripdata_{{ inputs.year }}_{{ inputs.month | numberFormat('00') }}_ext"
  table_tmp: "{{ inputs.taxi }}_tripdata_{{ inputs.year }}_{{ inputs.month | numberFormat('00') }}"
  table_final: "{{ inputs.taxi }}_tripdata"

tasks:
  - id: extract
    type: io.kestra.plugin.core.http.Download
    uri: "https://github.com/DataTalksClub/nyc-tlc-data/releases/download/{{ inputs.taxi }}/{{ vars.file_name }}"

  - id: upload_to_gcs
    type: io.kestra.plugin.gcp.gcs.Upload
    from: "{{ outputs.extract.uri }}"
    to: "{{ vars.gcs_path }}"

  - id: create_final_table
    type: io.kestra.plugin.gcp.bigquery.Query
    projectId: "{{ vars.project_id }}"
    sql: |
      CREATE TABLE IF NOT EXISTS `{{ vars.project_id }}.{{ vars.dataset }}.{{ vars.table_final }}` (
        unique_row_id STRING,
        filename STRING,
        VendorID INT64,
        pickup_datetime TIMESTAMP,
        dropoff_datetime TIMESTAMP,
        passenger_count INT64,
        trip_distance FLOAT64,
        -- ... additional columns
      )
      PARTITION BY DATE(pickup_datetime);

  - id: create_external_table
    type: io.kestra.plugin.gcp.bigquery.Query
    projectId: "{{ vars.project_id }}"
    sql: |
      CREATE OR REPLACE EXTERNAL TABLE `{{ vars.project_id }}.{{ vars.dataset }}.{{ vars.table_ext }}`
      OPTIONS (
        format = 'CSV',
        uris = ['{{ vars.gcs_path }}'],
        skip_leading_rows = 1
      );

  - id: create_staging_table
    type: io.kestra.plugin.gcp.bigquery.Query
    projectId: "{{ vars.project_id }}"
    sql: |
      CREATE OR REPLACE TABLE `{{ vars.project_id }}.{{ vars.dataset }}.{{ vars.table_tmp }}` AS
      SELECT
        GENERATE_UUID() as unique_row_id,
        '{{ vars.file_name }}' as filename,
        *
      FROM `{{ vars.project_id }}.{{ vars.dataset }}.{{ vars.table_ext }}`;

  - id: merge_to_final
    type: io.kestra.plugin.gcp.bigquery.Query
    projectId: "{{ vars.project_id }}"
    sql: |
      MERGE `{{ vars.project_id }}.{{ vars.dataset }}.{{ vars.table_final }}` T
      USING `{{ vars.project_id }}.{{ vars.dataset }}.{{ vars.table_tmp }}` S
      ON T.unique_row_id = S.unique_row_id
      WHEN NOT MATCHED THEN INSERT ROW;

  - id: cleanup
    type: io.kestra.plugin.gcp.gcs.Delete
    uri: "{{ vars.gcs_path }}"
```

### Key Differences from Postgres Pipeline

| Aspect | Postgres (ETL) | BigQuery (ELT) |
|--------|---------------|----------------|
| Transform location | Local Python | BigQuery SQL |
| Intermediate storage | None | GCS (data lake) |
| Loading mechanism | COPY command | External tables |
| Scalability | Limited by memory | Serverless, petabyte-scale |

---

## Scheduling GCP Workflows

```yaml
triggers:
  - id: yellow_daily
    type: io.kestra.plugin.core.trigger.Schedule
    cron: "0 10 * * *"  # 10 AM UTC
    inputs:
      taxi: yellow

  - id: green_daily
    type: io.kestra.plugin.core.trigger.Schedule
    cron: "0 9 * * *"   # 9 AM UTC
    inputs:
      taxi: green
```

**Backfilling in GCP:**
Since BigQuery handles large datasets efficiently, you can backfill the entire historical dataset (2019-2024) without worrying about local resource constraints.

---

# Using AI for Data Engineering

## Why AI for Workflows?

AI tools can accelerate workflow development by:
- **Generating boilerplate YAML** from natural language descriptions
- **Reducing documentation lookup** - AI knows plugin syntax
- **Catching errors** - Suggest corrections for invalid configurations

However, AI quality depends on **context**. Generic AI assistants often produce outdated or incorrect Kestra syntax because they lack knowledge of recent updates.

---

## Context Engineering

### The Problem with Generic AI

When you ask ChatGPT to create a Kestra flow without context:
1. It uses training data that may be months/years old
2. Plugin names, properties, and syntax may have changed
3. The generated YAML may not work

**Example prompt:**
> Create a Kestra flow that loads NYC taxi data from CSV to BigQuery

**Common issues in the response:**
- Outdated task types (renamed plugins)
- Non-existent properties
- Incorrect YAML structure

### The Solution: Context-Aware AI

Provide the AI with:
- Current documentation
- Working examples
- Plugin specifications

This is what Kestra's AI Copilot does automatically.

---

## Kestra AI Copilot

Kestra's built-in AI Copilot is trained on current Kestra documentation and plugin specifications.

### Setup

Add Gemini API configuration to your Docker Compose:

```yaml
services:
  kestra:
    environment:
      KESTRA_CONFIGURATION: |
        kestra:
          ai:
            type: gemini
            gemini:
              model-name: gemini-2.5-flash
              api-key: ${GEMINI_API_KEY}
```

Get an API key from [Google AI Studio](https://aistudio.google.com/app/apikey).

Restart Kestra:
```bash
export GEMINI_API_KEY="your-api-key"
docker compose up -d
```

### Using AI Copilot

1. Open the Kestra UI
2. Create or edit a flow
3. Click the sparkle icon (✨) in the code editor
4. Describe what you want in natural language

**Example prompt:**
> Create a flow that downloads a CSV file, transforms it with Python to filter rows where amount > 100, and saves to PostgreSQL

The Copilot generates working YAML with correct plugin types and properties.

---

## Retrieval Augmented Generation (RAG)

### What is RAG?

**RAG** (Retrieval Augmented Generation) grounds AI responses in real data:

1. **Retrieve** - Find relevant documents from a knowledge base
2. **Augment** - Add retrieved context to the prompt
3. **Generate** - LLM produces an answer using the context

This eliminates hallucinations by ensuring the AI has access to current, accurate information.

### RAG in Kestra

Kestra provides tasks for building RAG pipelines:

```yaml
id: chat_with_rag
namespace: zoomcamp

tasks:
  - id: ingest_docs
    type: io.kestra.plugin.ai.rag.Ingest
    provider:
      type: gemini
      modelName: gemini-2.5-flash
      apiKey: "{{ secret('GEMINI_API_KEY') }}"
    documents:
      - uri: https://kestra.io/docs/release-notes/1.1
        type: HTML
    vectorStore:
      type: kv
      prefix: kestra_docs

  - id: query
    type: io.kestra.plugin.ai.rag.Query
    provider:
      type: gemini
      modelName: gemini-2.5-flash
      apiKey: "{{ secret('GEMINI_API_KEY') }}"
    vectorStore:
      type: kv
      prefix: kestra_docs
    prompt: "What are the new features in Kestra 1.1?"
```

### How It Works

```
Ask Question → Search Vector Store → Find Similar Documents →
Add to Prompt → LLM Generates Grounded Answer
```

**Without RAG:** AI may hallucinate features that don't exist
**With RAG:** AI response is based on actual documentation

### RAG Best Practices

1. **Keep documents updated** - Re-ingest when sources change
2. **Chunk appropriately** - Split large documents into meaningful sections
3. **Test retrieval** - Verify correct documents are found for queries
4. **Use embeddings wisely** - Choose embedding models suited to your domain

---

# Deploying to the Cloud

For production workloads, deploy Kestra to a cloud environment.

### Options

1. **Google Cloud Run** - Serverless container hosting
2. **Google Kubernetes Engine (GKE)** - Managed Kubernetes
3. **Compute Engine** - Traditional VMs

### GitOps Integration

Sync flows from a Git repository:

```yaml
id: git_sync
namespace: system

triggers:
  - id: on_push
    type: io.kestra.plugin.core.trigger.Webhook

tasks:
  - id: pull_flows
    type: io.kestra.plugin.git.Clone
    url: https://github.com/your-org/kestra-flows.git
    branch: main

  - id: deploy_flows
    type: io.kestra.plugin.core.flow.Import
    directory: "{{ outputs.pull_flows.directory }}/flows"
```

### Security Considerations

- **Never commit credentials** to Git
- Use **Secrets** for sensitive values
- Use **KV Store** for configuration
- Enable **RBAC** for multi-user environments
- Use **HTTPS** in production

---

## Resources

### Documentation
- [Kestra Docs](https://kestra.io/docs)
- [Kestra Blueprints](https://kestra.io/blueprints) - Pre-built examples
- [Kestra Plugins](https://kestra.io/plugins) - 600+ integrations

### Community
- [Kestra GitHub](https://github.com/kestra-io/kestra)
- [Kestra Slack](https://kestra.io/slack)
- [DataTalksClub Slack](https://datatalks.club/slack.html)

### Video Playlist
- [Module 2 YouTube Playlist](https://go.kestra.io/de-zoomcamp/yt-playlist)

---

## Key Takeaways

1. **Workflow orchestration** coordinates multiple tools and data pipelines
2. **Kestra** uses declarative YAML for defining workflows
3. **Core concepts**: Flows, Tasks, Inputs, Outputs, Triggers, Variables
4. **ETL** transforms locally before loading; **ELT** loads first, transforms in the warehouse
5. **Scheduling and backfills** enable automated and historical data processing
6. **AI Copilot** accelerates development with context-aware code generation
7. **RAG** grounds AI responses in current documentation
