from flask import Flask, render_template, request
import yfinance as yf
from datetime import datetime, timedelta
from bokeh.plotting import figure
from bokeh.embed import components
from bokeh.models import DatetimeTickFormatter
import pandas as pd

app = Flask(__name__)

@app.route('/home')
@app.route('/')
def index():
    return render_template('index.html')

@app.route('/Explore/')
def Explore():
    return render_template('Explore.html')

@app.route('/graph', methods=['GET'])
def graph():
    # Get the stock symbol from the URL parameter
    stock_symbol = request.args.get('search')

    # Check if the stock symbol is None
    if stock_symbol is None:
        return "No stock symbol provided", 400

    # Download the stock data
    ten_years_ago = datetime.now() - timedelta(days=365 * 1)
    data = yf.download(stock_symbol, start=ten_years_ago.strftime('%Y-%m-%d'), end=datetime.now().strftime('%Y-%m-%d'))

    # Convert the DataFrame index (which are Timestamps) to datetime
    data.index = pd.to_datetime(data.index)
    #print head of the data
    print(data.head())

    # Write the data to a CSV file
    data.to_csv('data.csv')

    # Create a new plot with a title and axis labels
    p = figure(title="Stock prices for " + stock_symbol, x_axis_label='Date', y_axis_label='Price', x_axis_type="datetime")

    # Add a line renderer with legend and line thickness
    p.line(data.index, data['Close'], legend_label="Close", line_width=2)

    # Format the x-axis to display dates
    p.xaxis.formatter = DatetimeTickFormatter(days="%m/%d/%Y", months="%m/%d/%Y", years="%m/%d/%Y")

    # Generate the components of the plot
    script, div = components(p)
    print(script, div)  # Print the script and div
    
        

    # Render the template and pass the components of the plot
    return render_template('graph.html', script=script, div=div)

if __name__ == "__main__":
    app.run(debug=True)
