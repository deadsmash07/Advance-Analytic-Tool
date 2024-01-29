# subtask1.py
import argparse
import numpy as np
import time
import os
import plotly.graph_objects as go
import pandas as pd
from datetime import date, timedelta
from jugaad_data.nse import stock_df

# Parse command line arguments
parser = argparse.ArgumentParser()
parser.add_argument("SYMBOL", type=str, help="the stock symbol")
parser.add_argument("num_years", type=int, help="the number of years")
args = parser.parse_args()

# Step 1: Accept a number of years as input.
num_years = args.num_years
# Use the provided symbol
SYMBOL = args.SYMBOL

# Step 2: Fetch the data for the given stock symbol for the last x years.
to_date = date.today()
from_date = to_date - timedelta(days=365*num_years)
df = stock_df(symbol=SYMBOL, from_date=from_date, to_date=to_date, series="EQ")

# Removing unwanted columns
df = df.drop(columns=["VWAP", "52W H", "52W L", "SERIES","SYMBOL","PREV. CLOSE"])

# Step 3: Write the data to different file formats and measure the time taken to write the file.
file_formats = ['csv', 'txt']  # Removed 'hdf5'
benchmark_results = []

for file_format in file_formats:
    start_time = time.time()
    file_name = f"{SYMBOL}.{file_format}"
    if file_format == 'csv':
        df.to_csv(file_name, index=False)
    elif file_format == 'txt':
        df.to_csv(file_name, sep='\t', index=False)
    # elif file_format == 'parquet':
    #     df.to_parquet(file_name, index=False)
    # elif file_format == 'feather':
    #     df.to_feather(file_name)
    # elif file_format == 'stata':
    #     df.to_stata(file_name, write_index=False)
    # elif file_format == 'pickle':
    #     df.to_pickle(file_name)
    # elif file_format == 'json':
    #     df.to_json(file_name, orient='records')
    else:
        print(f"Unsupported file format: {file_format}")
    end_time = time.time()
    file_size = os.path.getsize(file_name)
    benchmark_results.append((file_format, end_time - start_time, file_size))

# Convert benchmark_results to a pandas DataFrame for easier manipulation
df_results = pd.DataFrame(benchmark_results, columns=['Format', 'Time', 'Size'])

# Calculate the mean and standard deviation of the time and size
mean_time = df_results['Time'].mean()
std_time = df_results['Time'].std()
mean_size = df_results['Size'].mean()
std_size = df_results['Size'].std()

# Convert df_results to a list of dictionaries
df_results_list = df_results.to_dict('records')

# Append the calculated values to the list
df_results_list.append({
    'Format': 'Mean',
    'Time': mean_time,
    'Size': mean_size
})

df_results_list.append({
    'Format': 'Std Dev',
    'Time': std_time,
    'Size': std_size
})

# Convert the list back to a DataFrame
df_results = pd.DataFrame(df_results_list)

# Write the DataFrame to a txt file
df_results.to_csv('benchmark_results.txt', sep='\t', index=False)

# Convert time to milliseconds and size to kilobytes
df_results['Time'] = df_results['Time'] * 1000
df_results['Size'] = df_results['Size'] / 1024

# Generate graphs using Plotly
fig = go.Figure()

for format in df_results['Format'].unique():
    if format in ['Mean', 'Std Dev']:  # Skip the mean and standard deviation rows
        continue
    format_data = df_results[df_results['Format'] == format]
    fig.add_trace(go.Scatter(x=format_data['Size'], y=format_data['Time'], mode='markers', name=format))

fig.update_layout(title='File size and time taken to write file by format', xaxis_title='File Size (kilobytes)', yaxis_title='Time (milliseconds)')
fig.write_image('benchmark_results.png')