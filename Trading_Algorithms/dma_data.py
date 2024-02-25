
import argparse
import time
import os
import pandas as pd
from datetime import datetime
from jugaad_data.nse import stock_df
import pandas_market_calendars as mcal
from datetime import timedelta
# Function to parse date in DD/MM/YYYY format
def parse_date(date_str):
    return datetime.strptime(date_str, "%d/%m/%Y")

# Parse command line arguments
parser = argparse.ArgumentParser()
parser.add_argument("symbol", type=str, help="the stock symbol")
parser.add_argument("start_date", type=str, help="the start date for testing")
parser.add_argument("end_date", type=str, help="the end date for testing")
parser.add_argument("n", type=int, help="num of days")
args = parser.parse_args()

# Use the provided symbol
SYMBOL = args.symbol
# Get NSE calendar
nyse = mcal.get_calendar('NSE')

# Convert string dates to datetime objects

n=args.n


end_date = parse_date(args.end_date)

start_date = parse_date(args.start_date) - timedelta(days=(0))

modified_start_date = start_date - timedelta(days=2*n+5*10)

# Fetch the data for the given stock symbol for the adjusted date range
try:
    df1 = stock_df(symbol=SYMBOL, from_date=start_date, to_date=end_date, series="EQ")

    # Check if expected columns are present
    actual_columns = df1.columns

    # Update your column processing logic based on actual columns
    # For example, if you were using 'CH_TRADE_HIGH_PRICE' previously, use 'HIGH' now

    # Processing data (example)
    df1 = df1[['DATE', 'CLOSE']]

    # Convert the 'DATE' column to the format DD/MM/YYYY
    df1['DATE'] = pd.to_datetime(df1['DATE']).dt.strftime('%d/%m/%Y')

    # Filter out rows that are more than 15 days from the start_date
    # start_index = df1[df1['DATE'] == args.start_date].index[0]
    # df1 = df1.iloc[start_index - 15 : start_index + 1]

    # Write the DataFrames to CSV files
    df1.to_csv(f"{SYMBOL}.csv", index=False)

except Exception as e:
    print(f"An error occurred while fetching stock data: {e}")        
        
# Fetch the data for the given stock symbol for the adjusted date range
try:
    df2 = stock_df(symbol=SYMBOL, from_date=modified_start_date, to_date=start_date, series="EQ")

    # Check if expected columns are present
    actual_columns = df2.columns

    # Update your column processing logic based on actual columns
    # For example, if you were using 'CH_TRADE_HIGH_PRICE' previously, use 'HIGH' now

    # Processing data (example)
    df2 = df2[['DATE', 'CLOSE']]

    # Convert the 'DATE' column to the format DD/MM/YYYY
    df2['DATE'] = pd.to_datetime(df2['DATE']).dt.strftime('%d/%m/%Y')

    # Filter out rows that are more than 15 days from the start_date
    # start_index = df1[df1['DATE'] == args.start_date].index[0]
    # df1 = df1.iloc[start_index - 15 : start_index + 1]

    # Write the DataFrames to CSV files
    df2.to_csv("extra.csv", index=False)

except Exception as e:
    print(f"An error occurred while fetching stock data: {e}")          