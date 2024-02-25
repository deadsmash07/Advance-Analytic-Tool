# subtask1.py
import argparse
import numpy as np
import time
import matplotlib.pyplot as plt
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
parser.add_argument("train_start_date", type=str, help="the start date for training")
parser.add_argument("train_end_date", type=str, help="the end date for training")
parser.add_argument("start_date", type=str, help="the start date for testing")
parser.add_argument("end_date", type=str, help="the end date for testing")
args = parser.parse_args()

# Use the provided symbol
SYMBOL = args.symbol

# Get NSE calendar
nyse = mcal.get_calendar('NSE')

# Convert string dates to datetime objects


train_start_date = parse_date(args.train_start_date) - timedelta(days=1)
train_end_date = parse_date(args.train_end_date)
start_date = parse_date(args.start_date)- timedelta(days=1)
end_date = parse_date(args.end_date)

# Fetch the data for the given stock symbol for the given dates.
try:
    df1 = stock_df(symbol=SYMBOL, from_date=train_start_date, to_date=train_end_date, series="EQ")
    df2 = stock_df(symbol=SYMBOL, from_date=start_date, to_date=end_date, series="EQ")

    # Check if expected columns are present
    actual_columns = df1.columns

    # Update your column processing logic based on actual columns
    # For example, if you were using 'CH_TRADE_HIGH_PRICE' previously, use 'HIGH' now

    # Processing data (example)
    df1 = df1[['DATE', 'OPEN', 'HIGH', 'LOW', 'CLOSE','VWAP', 'NO OF TRADES']]
    df2 = df2[['DATE', 'OPEN', 'HIGH', 'LOW', 'CLOSE','VWAP', 'NO OF TRADES']]

    # Convert the 'DATE' column to the format DD/MM/YYYY
    df1['DATE'] = pd.to_datetime(df1['DATE']).dt.strftime('%d/%m/%Y')
    df2['DATE'] = pd.to_datetime(df2['DATE']).dt.strftime('%d/%m/%Y')

    # Write the DataFrames to CSV files
    df1.to_csv(f"{SYMBOL}_training_data.csv", index=False)
    df2.to_csv(f"{SYMBOL}_testing_data.csv", index=False)

except Exception as e:
    print(f"An error occurred while fetching stock data: {e}")
    
    
    
