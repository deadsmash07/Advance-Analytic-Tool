from flask import Flask, render_template, request
import yfinance as yf
from datetime import datetime, timedelta
import pandas as pd
import plotly.graph_objects as go
import plotly.io as pio
from plotly.utils import PlotlyJSONEncoder
import json

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
    ten_years_ago = datetime.now() - timedelta(days=365 * 10)
    data = yf.download(stock_symbol, start=ten_years_ago.strftime('%Y-%m-%d'), end=datetime.now().strftime('%Y-%m-%d'))

    # Convert the DataFrame index (which are Timestamps) to datetime
    data.index = pd.to_datetime(data.index)

    # Create a new plot with a title and axis labels
    fig = go.Figure(data=[go.Candlestick(x=data.index,
                                         open=data['Open'],
                                         high=data['High'],
                                         low=data['Low'],
                                         close=data['Close'])])

    # Add range slider
    fig.update_layout(
        title="Stock prices for " + stock_symbol,
        xaxis_title='Date',
        yaxis_title='Price',
        xaxis=dict(
            rangeselector=dict(
                buttons=list([
                    dict(count=1,
                         label="daily",
                         step="day",
                         stepmode="backward"),
                    dict(count=1,
                         label="1m",
                         step="month",
                         stepmode="backward"),
                    dict(count=6,
                         label="6m",
                         step="month",
                         stepmode="backward"),
                    dict(count=10,
                         label="TD",
                         step="year",
                         stepmode="todate"),
                    dict(count=1,
                         label="1y",
                         step="year",
                         stepmode="backward"),
                    dict(step="all")
                ])
            ),
            rangeslider=dict(
                visible=True
            ),
            type="date"
        )
    )

    # Convert the figure to JSON
    fig_json = json.dumps(fig, cls=PlotlyJSONEncoder)

    # Render the template and pass the JSON of the plot
    return render_template('graph.html', plot=fig_json)

if __name__ == "__main__":
    app.run(debug=True)