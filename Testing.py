# this code is for testing purpose only
import yfinance as yf

def get_stock_details(stock_symbol):
    # Fetch stock details using yfinance
    stock_info = yf.Ticker(stock_symbol)

    # Get the P/E ratio from the 'info' attribute
    pe_ratio = stock_info.info.get('trailingPE', 'N/A')

    # Get the summary or long business description
    about_stock = stock_info.info.get('longBusinessSummary', 'N/A')

    return {
        'PE_RATIO': pe_ratio,
        'ABOUT_STOCK': about_stock
    }

# Replace 'AAPL' with the stock symbol of your choice
symbol = 'AAPL'

# Create a Ticker object
stock = yf.Ticker(symbol)

# Retrieve earnings data
earnings_data = stock.earnings

# Display EPS data
print(earnings_data)

# Example usage:
stock_symbol = 'AAPL'  # Replace with the desired stock symbol
details = get_stock_details(stock_symbol)

print(f"P/E Ratio for {stock_symbol}: {details['PE_RATIO']}")
print(f"About {stock_symbol}: {details['ABOUT_STOCK']}")
