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
