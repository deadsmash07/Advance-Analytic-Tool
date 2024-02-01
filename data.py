import yfinance as yf
import pandas as pd
import datetime

# List of NIFTY50 ticker symbols
nifty50_tickers = [
    'ADANIPORTS.NS', 'ASIANPAINT.NS', 'AXISBANK.NS', 'BAJAJ-AUTO.NS',
    'BAJFINANCE.NS', 'BAJAJFINSV.NS', 'BPCL.NS', 'BHARTIARTL.NS',
    'BRITANNIA.NS', 'CIPLA.NS', 'COALINDIA.NS', 'DIVISLAB.NS',
    'DRREDDY.NS', 'EICHERMOT.NS', 'GRASIM.NS', 'HCLTECH.NS',
    'HDFCBANK.NS', 'HDFCLIFE.NS', 'HEROMOTOCO.NS', 'HINDALCO.NS',
    'HINDUNILVR.NS', 'ICICIBANK.NS', 'ITC.NS',
    'IOC.NS', 'INDUSINDBK.NS', 'INFY.NS', 'JSWSTEEL.NS',
    'KOTAKBANK.NS', 'LT.NS', 'M&M.NS', 'MARUTI.NS',
    'NTPC.NS', 'NESTLEIND.NS', 'ONGC.NS', 'POWERGRID.NS',
    'RELIANCE.NS', 'SBILIFE.NS', 'SHREECEM.NS', 'SBIN.NS',
    'SUNPHARMA.NS', 'TCS.NS', 'TATAMOTORS.NS', 'TATASTEEL.NS',
    'TECHM.NS', 'TITAN.NS', 'UPL.NS', 'ULTRACEMCO.NS',
    'WIPRO.NS', 'ZEEL.NS'
]

def get_top_gainers_losers():

    # Get today's date and yesterday's date
    today = datetime.date.today()
    yesterday = today - datetime.timedelta(days=1)

    # Convert the dates to strings in the format yfinance expects
    start_date = yesterday.strftime('%Y-%m-%d')
    end_date = today.strftime('%Y-%m-%d')

    # Fetch data for the last 3 days
    nifty_data = yf.download(nifty50_tickers, period="3d")

    # Calculate the percentage change for the last two days
    price_changes = nifty_data['Close'].iloc[-2:].pct_change()

    # Convert the price changes to percentage and round to 3 decimal places
    price_changes = (price_changes * 100).round(2)

    # Get the top 10 companies based on the total volume of shares traded
    top_trending = nifty_data['Volume'].sum().sort_values(ascending=False).head(10)

    # Get the closing prices for the top trending stocks
    closing_prices = nifty_data['Close'].iloc[-1][top_trending.index]
    closing_prices = closing_prices.round(2)

    # Add the closing prices as a new column to the top_trending DataFrame
    top_trending = pd.DataFrame({
        'Name': top_trending.index,
        'Percentage_Change': price_changes.iloc[-1][top_trending.index],
   
    })

    # Write the data to CSV files
    top_gainers = price_changes.iloc[-1].sort_values(ascending=False).head(10)
    top_losers = price_changes.iloc[-1].sort_values().head(10)

    # Remove '.NS' from the ticker symbols
    top_gainers.index = top_gainers.index.str.replace('.NS', '', regex=False)
    top_losers.index = top_losers.index.str.replace('.NS', '', regex=False)
    top_trending['Name'] = top_trending['Name'].str.replace('.NS', '', regex=False)

    # Write the data to CSV files
    # Read the CSV file and skip the first row
    top_trending = pd.read_csv('top_trending.csv', skiprows=1)

    # Write the DataFrame back to the CSV file
    top_trending.to_csv('top_trending.csv', index=False)
    top_gainers.to_csv('top_gainers.csv', header=False)
    top_losers.to_csv('top_losers.csv', header=False)

get_top_gainers_losers()
