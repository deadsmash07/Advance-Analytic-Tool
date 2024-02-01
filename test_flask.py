# Import other necessary libraries
from flask import Flask, render_template, request
import yfinance as yf
from datetime import datetime, timedelta
import pandas as pd
import plotly.graph_objects as go
from plotly.io import to_json
from plotly.subplots import make_subplots
import json
from plotly.utils import PlotlyJSONEncoder

app = Flask(__name__)

def create_plot(data, stock_symbol, plot_type):
    if plot_type == 'area':
        plot = go.Figure(data=go.Scatter(x=data.index, y=data['Close'], fill='tozeroy', fillcolor='rgba(224, 174, 208,0.5)', line_color='rgb(172, 135, 197)', name=stock_symbol))        
        title = "Close prices for " + stock_symbol
    elif plot_type == 'candlestick':
        plot = go.Figure(data=[go.Candlestick(x=data.index,
                                              open=data['Open'],
                                              high=data['High'],
                                              low=data['Low'],
                                              close=data['Close'],
                                              name='Candlestick',
                                              increasing_line_width=1, decreasing_line_width=1,
                                              increasing_line_color='blue', decreasing_line_color='red')])
        title = "Candlestick Chart for " + stock_symbol

    plot.update_layout(
        title=title,
        xaxis_title='Date',
        yaxis_title='Price',
        autosize=True,
        width=1500,
        height=600,        
        hovermode="x",
        paper_bgcolor='rgba(255, 229, 229,0.2)',  # boundary color
        plot_bgcolor='rgba(255,255,255, 0.9)',
        xaxis=dict(
            rangeselector=dict(
                buttons=list([
                    dict(count=7, label="1W", step="day", stepmode="backward"),
                    dict(count=1, label="1M", step="month", stepmode="backward"),
                    dict(count=6, label="6M", step="month", stepmode="backward"),
                    dict(count=1, label="1Y", step="year", stepmode="backward"),
                    dict(count=10, label="10Y", step="year", stepmode="todate"),
                    dict(step="all")
                ])
            ),
            rangeslider=dict(
                visible=True,
                bgcolor='rgb(255, 229, 229,0.2)'
                
            ),
            type="date"
        )
    )
    
    return to_json(plot)


def get_stock_details(stock_symbol):
    # Fetch stock details using yfinance
    stock_info = yf.Ticker(stock_symbol)

    # Initialize variables
    open_price = 'N/A'
    close_price = 'N/A'
    high_price = 'N/A'
    low_price = 'N/A'
    volume = 'N/A'

    try:
        # Get the most recent historical data (1 day)
        history_data = stock_info.history(period='1d')
        print(history_data.head())
        if not history_data.empty:
            # Get the 'Open' and 'Close' prices from the most recent row
            open_price = history_data.iloc[-1]['Open']
            close_price = history_data.iloc[-1]['Close']
            high_price = history_data.iloc[-1]['High']
            low_price = history_data.iloc[-1]['Low']
            volume = history_data.iloc[-1]['Volume']
    except IndexError:
        # Handle IndexError (out-of-bounds) by setting prices to 'N/A'
        pass

    # Get other stock details
    pe_ratio = stock_info.info.get('trailingPE', 'N/A')
    about_stock = stock_info.info.get('longBusinessSummary', 'N/A')

    return {
        'PE_RATIO': pe_ratio,
        'OPEN_PRICE': open_price,
        'CLOSE_PRICE': close_price,
        'ABOUT_STOCK': about_stock,
        'HIGH_PRICE': high_price,
        'LOW_PRICE': low_price,
        'VOLUME': volume,
    }

def create_nifty_line_plot():
    # Fetch NIFTY data from Yahoo Finance
    ten_years_ago = datetime.now() - timedelta(days=365 * 10)
    nifty_data = yf.download('^NSEI', start=ten_years_ago.strftime('%Y-%m-%d'), end=datetime.now().strftime('%Y-%m-%d'))
    # Create a line plot
    fig = go.Figure(data=[go.Scatter(x=nifty_data.index, y=nifty_data['Close'], name='NIFTY', fill='tozeroy', fillcolor='rgba(224, 174, 208,0.5)', line=dict(color='rgba(177,79,255,1)'))])
    # Customize the layout
    fig.update_layout(
        title="NIFTY Index",
        xaxis_title='Date',
        width=1090,
        height=400,
        yaxis_title='Closing Price',        
        paper_bgcolor='rgba(255, 229, 229,0.0)',
        plot_bgcolor='rgba(255, 229, 229,0.0)',
        autosize=True,
        hovermode="x",
        xaxis=dict(
            rangeselector=dict(
                buttons=list([
                    dict(count=7, label="1W", step="day", stepmode="backward"),
                    dict(count=1, label="1M", step="month", stepmode="backward"),
                    dict(count=6, label="6M", step="month", stepmode="backward"),
                    dict(count=1, label="1Y", step="year", stepmode="backward"),
                    dict(count=10, label="10Y", step="year", stepmode="todate"),
                    dict(step="all")
                ])
            ),

        )
    )

    # Convert the figure to JSON
    fig_json = json.dumps(fig, cls=PlotlyJSONEncoder)
    return fig_json

@app.route('/home')
@app.route('/')
def home():
    # Fetch the NIFTY line plot
    # Fetch the NIFTY data
    nifty = yf.Ticker("^NSEI")

    # Calculate the 1 day, 1 week, and 1 year returns
    end = datetime.today()
    start_day = end - timedelta(days=2)
    start_week = end - timedelta(weeks=1)
    start_year = end - timedelta(weeks=52)

    return_day = ((nifty.history(start=start_day, end=end-timedelta(days=1))['Close'][-1] / nifty.history(start=start_day, end=end-timedelta(days=1))['Close'][0]) - 1) * 100
    return_week = ((nifty.history(start=start_week, end=end)['Close'][-1] / nifty.history(start=start_week, end=end)['Close'][0]) - 1) * 100
    return_year = ((nifty.history(start=start_year, end=end)['Close'][-1] / nifty.history(start=start_year, end=end)['Close'][0]) - 1) * 100

    # Read the top_gainers.csv file
    top_gainers = pd.read_csv('top_gainers.csv')
    top_gainers_list = top_gainers.values.tolist()
    print(top_gainers_list)
    nifty_line_plot_json = create_nifty_line_plot()
    return render_template('index.html',  nifty_line_plot=nifty_line_plot_json, return_day=return_day, return_week=return_week, return_year=return_year, top_gainers=top_gainers_list)


@app.route('/Explore/', methods=['GET'])
def explore():
    return render_template('explore.html')

@app.route('/graph', methods=['GET'])
def graph():
    stock_symbol = request.args.get('search')

    if stock_symbol is None:
        return "No stock symbol provided", 400

    stock_symbol_nse = stock_symbol + ".NS"

    ten_years_ago = datetime.now() - timedelta(days=365 * 15)
    data = yf.download(stock_symbol_nse, start=ten_years_ago.strftime('%Y-%m-%d'), end=datetime.now().strftime('%Y-%m-%d'))

    data.index = pd.to_datetime(data.index)

    line_plot_json = create_plot(data, stock_symbol, 'area')
    candlestick_plot_json = create_plot(data, stock_symbol, 'candlestick')

    stock_details = get_stock_details(stock_symbol_nse)

    return render_template('graph.html', line_plot=line_plot_json, candlestick_plot=candlestick_plot_json, stock_symbol=stock_symbol, stock_details=stock_details)


@app.route('/compare_graph', methods=['GET'])
def compare_graph():
    stock_symbols = request.args.getlist('stocks')
    stock_symbols = [symbol + ".NS" for symbol in stock_symbols]

    if not all(stock_symbols):
        return "Please provide valid stock symbols", 400

    data = [yf.download(symbol, start='2013-01-01', end=datetime.now().strftime('%Y-%m-%d')) for symbol in stock_symbols]

    fig = go.Figure()
    colors = ['rgba(255, 0, 0, 0.1)', 'rgba(0, 0, 255, 0.1)', 'rgba(0, 255, 0, 0.1)', 'rgba(255, 0, 255, 0.1)']  # Red, Green, Blue, Purple with 0.5 transparency
    line_colors = ['rgba(255, 0, 0, 1)', 'rgba(0, 0, 255, 1)', 'rgba(0, 255, 0, 1)', 'rgba(255, 0, 255, 1)']  # Darker Red, Green, Blue, Purple with full opacity
    for i in range(len(stock_symbols)):
        fig.add_trace(go.Scatter(x=data[i].index, y=data[i]['Close'], fill='tozeroy', fillcolor=colors[i % len(colors)], line_color=line_colors[i % len(line_colors)], name=stock_symbols[i]))
    fig.update_layout(
        title=f"Closing prices for {', '.join([symbol.replace('.NS', '') for symbol in stock_symbols])}",   
        xaxis_title='Date',
        yaxis_title='Price',
        autosize=False,
        width=1500,
        height=600,
        hovermode="x",
        plot_bgcolor='rgba(255,255,255, 0.9)',
        paper_bgcolor='rgb(255, 229, 229,0.2)',  # boundary color
        margin=dict(l=50, r=50, t=50, b=50),  # boundary size
        yaxis=dict(
            type='log',
            autorange=True,
            tickmode='linear',
            tick0=0.1,
            dtick=0.5
        ),
        xaxis=dict(
            rangeselector=dict(
                buttons=list([
                    dict(count=7, label="1W", step="day", stepmode="backward"),
                    dict(count=1, label="1M", step="month", stepmode="backward"),
                    dict(count=6, label="6M", step="month", stepmode="backward"),
                    dict(count=1, label="1Y", step="year", stepmode="backward"),
                    dict(count=10, label="10Y", step="year", stepmode="todate"),
                    dict(step="all")
                ])
            ),
            rangeslider=dict(
                visible=True,
                bgcolor='rgb(255, 229, 229,0.2)'
            ),
            type="date"
        )
    )

    fig_json = json.dumps(fig, cls=PlotlyJSONEncoder)
    return render_template('compare_graph.html', plot=fig_json)




if __name__ == "__main__":
    app.run(debug=True)
