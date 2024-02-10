#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

struct StockData {
    string date;
    double open;
    double high;
    double low;
    double prevClose;
    double close;
    double vwap;
    int numofTrades;
};

// Function to read data from CSV file and store it in a vector of StockData
vector<StockData> readCSV(const string& filename) {
    vector<StockData> stockData;
    ifstream file(filename);
    string line;

    // Skip the header line
    getline(file, line);

    // Inside your while loop
    while (getline(file, line)) {
        stringstream ss(line);
        StockData data;
        string field;

        getline(ss, data.date, ',');
        
        getline(ss, field, ',');
        data.open = stod(field);
        
        getline(ss, field, ',');
        data.high = stod(field);
        
        getline(ss, field, ',');
        data.low = stod(field);
        
        getline(ss, field, ',');
        data.prevClose = stod(field);
        
        getline(ss, field, ',');
        data.close = stod(field);
        
        getline(ss, field, ',');
        data.vwap = stod(field);
        
        getline(ss, field, ',');
        data.numofTrades = stoi(field);

        stockData.push_back(data);
    }

    return stockData;
}

int main() {
    // Replace "your_file.csv" with the actual filename
    vector<StockData> stockData = readCSV("SBIN.csv");

    // Now you can work with the extracted data
    for (const auto& data : stockData) {
        cout << "Date: " << data.date << ", Close: " << fixed << setprecision(2) << data.close << endl;
        // Add more fields as needed
    }

    return 0;
}

