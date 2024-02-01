import schedule
import time
from data import get_top_gainers_losers

def job():
    get_top_gainers_losers()

# Schedule the job to run once a day
schedule.every().day.at("00:00").do(job)

while True:
    # Run pending scheduled jobs
    schedule.run_pending()
    time.sleep(1)