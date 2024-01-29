# Makefile

all: run

run:
	@pip3 install -r requirements.txt
	@python3 main.py $(SYMBOL) $(num_years)