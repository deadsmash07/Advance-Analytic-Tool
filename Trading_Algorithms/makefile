.PHONY: all DMA DMA++ MACD RSI ADX LINEAR_REGRESSION BASIC PAIRS PAIRS_WITH_STOP_LOSS

# Install dependencies for all targets
all: install_dependencies clean

install_dependencies:
	pip install pandas_market_calendars

symbol	?=	"SBIN"
strategy ?= "BASIC"
n ?= 4
x ?= 4
p ?= 1
max_hold_days ?= 28
c1 ?= 2
c2 ?= 0.2
start_date ?= 01/01/2019
end_date ?= 01/01/2020
oversold_threshold ?= 30
overbought_threshold ?= 70
adx_threshold ?= 25
train_start_date ?= 01/01/2018
train_end_date ?= 01/01/2019
symbol1 ?= "SBIN"
symbol2 ?= "ADANIENT"
threshold ?= 2
stop_loss_threshold ?= -5 # garbage value

clean: $(strategy)

BASIC:
	python3 mmt_data.py $(symbol) $(start_date) $(end_date) $(n) 
	g++ mmt.cpp -o mmt
	./mmt $(strategy) $(symbol) $(n) $(x) $(start_date) $(end_date)
	rm -f mmt
	
DMA:
	python3 dma_data.py $(symbol) $(start_date) $(end_date) $(n) 
	g++ dma.cpp -o dma
	./dma $(strategy) $(symbol) $(n) $(x) $(p) $(start_date) $(end_date)
	rm -f dma

DMA++:
	python3 improving_dmas_data.py $(symbol) $(start_date) $(end_date) $(n)
	g++ improving_dmas.cpp -o dmas
	./dmas $(strategy) $(symbol) $(x) $(p) $(n) $(max_hold_days) $(c1) $(c2) $(start_date) $(end_date)
	rm -f dmas
MACD:
	python3 macd_data.py $(symbol) $(start_date) $(end_date) 
	g++ macd.cpp -o macd
	./macd $(strategy) $(symbol) $(x) $(start_date) $(end_date)
	rm -f macd

RSI:
	python3 rsi_data.py $(symbol) $(start_date) $(end_date) $(n)
	g++ rsi.cpp -o rsi
	./rsi $(strategy) $(symbol) $(x) $(n) $(overbought_threshold) $(oversold_threshold) $(start_date) $(end_date)
	rm -f rsi

ADX:
	python3 adx_data.py $(symbol) $(start_date) $(end_date) $(n)
	g++ adx.cpp executer.cpp -o executer
	./executer $(strategy) $(n) $(x) $(adx_threshold)
	rm -f executer

LINEAR_REGRESSION:
	python3 lr_data.py $(symbol) $(train_start_date) $(train_end_date) $(start_date) $(end_date) 
	g++ lr_final.cpp -o test
	./test $(strategy) $(symbol) $(x) $(p) $(train_start_date) $(train_end_date) $(start_date) $(end_date)
	rm -f test


BEST_OF_ALL:


PAIRS:
	python3 pair_data.py $(symbol1) $(symbol2) $(start_date) $(end_date) $(n)
	g++ pair.cpp -o pair
	./pair $(strategy) $(symbol1) $(symbol2) $(x) $(n) $(threshold) $(start_date) $(end_date)
	rm -f pair

PAIRS_WITH_STOP_LOSS:
	python3 pairs_stop_data.py $(symbol1) $(symbol2) $(start_date) $(end_date) $(n)
	g++ pairs_stoploss.cpp -o pair_stoploss
	./pair_stoploss $(strategy) $(symbol1) $(symbol2) $(x) $(n) $(threshold) $(stop_loss_threshold) $(start_date) $(end_date)
	rm -f pair_stoploss



