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
def home():
    return render_template('index.html')

@app.route('/Explore/', methods=['GET'])
def explore():
    num_stocks = int(request.args.get('num_stocks', 1))
    return render_template('explore.html', num_stocks=num_stocks)

@app.route('/graph', methods=['GET'])
def graph():
    stock_symbol = request.args.get('search')

    if stock_symbol is None:
        return "No stock symbol provided", 400

    stock_symbol_nse = stock_symbol + ".NS"

    ten_years_ago = datetime.now() - timedelta(days=365 * 10)
    data = yf.download(stock_symbol_nse, start=ten_years_ago.strftime('%Y-%m-%d'), end=datetime.now().strftime('%Y-%m-%d'))

    data.index = pd.to_datetime(data.index)

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

    fig_json2 = json.dumps(line_plot, cls=PlotlyJSONEncoder)
    return render_template('graph.html', plot=fig_json2)

@app.route('/select_stocks', methods=['GET'])
def select_stocks():
    num_stocks = int(request.args.get('num_stocks', 1))
    return render_template('select_stocks.html', num_stocks=num_stocks)

@app.route('/compare_graph', methods=['GET'])
def compare_graph():
    num_stocks = int(request.args.get('num_stocks', 1))

    stock_symbols = [request.args.get(f'stock{i+1}') for i in range(4)]
    stock_symbols = [symbol for symbol in stock_symbols if symbol]

    if not stock_symbols:
        return "Please provide at least one stock symbol", 400
    stock_symbols = [symbol + ".NS" for symbol in stock_symbols]
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
