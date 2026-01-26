import pandas as pd
import click
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

chunk_size = 100_000

@click.command()
@click.option('--user', default='root', help='PostgreSQL user')
@click.option('--password', default='root', help='PostgreSQL password')
@click.option('--host', default='localhost', help='PostgreSQL host')
@click.option('--port', default=9868, type=int, help='PostgreSQL port')
@click.option('--db', default='ny_taxi', help='PostgreSQL database name')
@click.option('--table', default='yellow_taxi_data', help='Target table name')
@click.option('--year', type=int, default=2021, help='Year of the data to ingest')
@click.option('--month', type=int, default=1, help='Month of the data to ingest (1-12)')

def ingest_data(user, password, host, port, db, table, year, month):
    """Ingests NYC Yellow Taxi data into a PostgreSQL database.

    The function reads a compressed CSV file containing New York taxi ride data
    for a given year and month, and loads it into a PostgreSQL table
    using chunked inserts to reduce memory usage.

    Args:
    user (str): PostgreSQL username.
    password (str): PostgreSQL password.
    host (str): PostgreSQL database host.
    port (int): PostgreSQL database port.
    db (str): PostgreSQL database name.
    table (str): Destination table name.
    year (int): Year of data to import.
    month (int): Month of data to import (1–12).

    Raises:
    FileNotFoundError: If the CSV file does not exist.
    sqlalchemy.exc.SQLAlchemyError: In case of connection or database write errors.
    """


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
                name=f'{table}_{year}_{month:02d}', con=engine, if_exists="replace", index=False
            )
            first = False
            print("Table created")
        df_chunk.to_sql(
            name=f'{table}_{year}_{month:02d}',
            con=engine,
            if_exists="append",
            index=False,
            chunksize=chunk_size,
            method="multi",
        )
        print("Inserted:", len(df_chunk))


if __name__ == "__main__":
    ingest_data()
