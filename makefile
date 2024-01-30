install:
	@echo "Installing dependencies..."
	@pip install -r requirements.txt

run:
	@echo "Running the application..."
	@python3 test_flask.py

all: install run