from datetime import date, datetime
import time
import os
import sys
import matplotlib.pyplot as plt

SYMBOL=input("Enter the symbol of the stock: ")
num_years=int(input("Enter the number of years: "))
from jugaad_data.nse import stock_df
df = stock_df(symbol=SYMBOL, from_date=date(date.today().year-num_years,date.today().month,date.today().day),
            to_date=date.today(), series="EQ")
# df = df[["DATE", "OPEN", "CLOSE", "HIGH", "LOW", "LTP", "VOLUME", "VALUE", "NO OF TRADES"]]
df = df[["DATE", "CLOSE"]] 
time_recorded=[]
size=[]
format=[]

started=time.time()
df.to_csv(f'{SYMBOL}.csv',index=False)
ended=time.time()
time_recorded.append((ended-started))
size.append(os.path.getsize(f'{SYMBOL}.csv'))
format.append("CSV")

started = time.time()
df.to_csv(f'{SYMBOL}.txt', index=False, sep='\t')
ended = time.time()
time_recorded.append((ended-started))
size.append(os.path.getsize(f'{SYMBOL}.txt'))
format.append("text")

started=time.time()
df.to_parquet(f'{SYMBOL}.parquet', index=False)
ended=time.time()
time_recorded.append((ended-started))
size.append(os.path.getsize(f'{SYMBOL}.parquet'))
format.append("Parquet")

started = time.time()
df.to_pickle(f'{SYMBOL}_binary.pkl')
ended = time.time()
time_recorded.append((ended-started))
size.append(os.path.getsize(f'{SYMBOL}_binary.pkl'))
format.append("Binary pickle")


fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8))

ax1.bar(format, time_recorded, color='green')
ax1.set_ylabel("Time")
ax1.set_title(f'Time and Size Subplots for {SYMBOL}')

ax2.bar(format, size, color='yellow')
ax2.set_xlabel("Type")
ax2.set_ylabel("Size")


plt.tight_layout()

plt.savefig(f'{SYMBOL}.png')