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

def create_line_plot(data, stock_symbol):
    line_plot = go.Figure(data=[go.Scatter(x=data.index, y=data['Close'], name='', hovertemplate='Date: %{x}<br>Close Price: %{y}')])

    line_plot.update_layout(
        title="Close prices for " + stock_symbol,
        xaxis_title='Date',
        yaxis_title='Price',
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
            rangeslider=dict(
                visible=True
            ),
            type="date"
        )
    )
    
    return to_json(line_plot)

def create_candlestick_plot(data, stock_symbol):
    candlestick_plot = go.Figure(data=[go.Candlestick(x=data.index,
                                                      open=data['Open'],
                                                      high=data['High'],
                                                      low=data['Low'],
                                                      close=data['Close'],
                                                      name='Candlestick',
                                                      increasing_line_width=1, decreasing_line_width=1,
                                                      increasing_line_color='blue', decreasing_line_color='red')])

    candlestick_plot.update_layout(
        title="Candlestick Chart for " + stock_symbol,
        xaxis_title='Date',
        yaxis_title='Price',
        autosize=True,
        hovermode="x",
        showlegend=True,
        xaxis=dict(
            tickformat='%d %B (%a)<br>%Y',
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
                visible=True
            ),
            type="date"
        )
    )

    return to_json(candlestick_plot)

@app.route('/home')
@app.route('/')
def home():
    return render_template('index.html')

@app.route('/Explore/', methods=['GET'])
def explore():
    return render_template('explore.html')

@app.route('/graph', methods=['GET'])
def graph():
    stock_symbol = request.args.get('search')

    if stock_symbol is None:
        return "No stock symbol provided", 400

    stock_symbol_nse = stock_symbol + ".NS"

    ten_years_ago = datetime.now() - timedelta(days=365 * 10)
    data = yf.download(stock_symbol_nse, start=ten_years_ago.strftime('%Y-%m-%d'), end=datetime.now().strftime('%Y-%m-%d'))

    data.index = pd.to_datetime(data.index)

    line_plot_json = create_line_plot(data, stock_symbol)
    candlestick_plot_json = create_candlestick_plot(data, stock_symbol)

    return render_template('graph.html', line_plot=line_plot_json, candlestick_plot=candlestick_plot_json)

@app.route('/compare_graph', methods=['GET'])
def compare_graph():
    stock_symbols = request.args.getlist('stocks')
    stock_symbols = [symbol + ".NS" for symbol in stock_symbols]

    if not all(stock_symbols):
        return "Please provide valid stock symbols", 400

    data = [yf.download(symbol, start='2013-01-01', end=datetime.now().strftime('%Y-%m-%d')) for symbol in stock_symbols]

    fig = go.Figure()
    for i in range(len(stock_symbols)):
        fig.add_trace(go.Scatter(x=data[i].index, y=data[i]['Close'], mode='lines', name=stock_symbols[i]))

    fig.update_layout(
        title=f"Closing prices for {', '.join([symbol.replace('.NS', '') for symbol in stock_symbols])}",   
        xaxis_title='Date',
        yaxis_title='Price',
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
            rangeslider=dict(
                visible=True
            ),
            type="date"
        )
    )

    fig_json = json.dumps(fig, cls=PlotlyJSONEncoder)
    return render_template('compare_graph.html', plot=fig_json)

if __name__ == "__main__":
    app.run(debug=True)
