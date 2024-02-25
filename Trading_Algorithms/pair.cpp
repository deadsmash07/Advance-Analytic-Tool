#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>
using namespace std;

class StockData {
private:
    string date;
    double price1;
    double price2;
    double spread;
    double rollingMean;
    double rollingStdDev;
    double z_score;
    string action = "";
    int quantity = 0;

public:
    // Constructor
    StockData(const string& date, double price1, double price2) 
        : date(date), price1(price1), price2(price2){}

    string getDate() const {
        return date;
    }
    int getQuantity() const {
        return quantity;
    }
    void setQuantity(int quantity) {
        this->quantity = quantity;
    }
    string getAction() const {
        return action;
    }
    void buy() {
        this->action = "BUY";
    }
    void sell() {
        this->action = "SELL";
    }

    double getPrice1() const {
        return price1;
    }

    double getPrice2() const {
        return price2;
    }

    double getSpread() const {
        return spread;
    }
    void setspread(double spread) {
        this->spread = spread;
    }
    double getRollingMean() const {
        return rollingMean;
    }
    void setRollingMean(double rollingMean) {
        this->rollingMean = rollingMean;
    }
    double getRollingStdDev() const {
        return rollingStdDev;
    }
    void setRollingStdDev(double rollingStdDev) {
        this->rollingStdDev = rollingStdDev;
    }
    double getz_score() const {
        return z_score;
    }
    void setz_score(double z_score) {
        this->z_score = z_score;
    }

    static vector<StockData> readFromFile(const string& filename) {
        vector<StockData> data;
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Unable to open file " << filename << endl;
            return data; // Return empty vector
        }

        string line;
        vector<string> lines; // Store lines in a vector

        // Read all lines into the vector
        while (getline(file, line)) {
            lines.push_back(line);
        }

        // Skip the first line
        bool firstLineSkipped = false;

        // Read lines in reverse order
        for (auto it = lines.rbegin(); it != lines.rend(); ++it) {
            // if (!firstLineSkipped) {
            //     firstLineSkipped = true;
            //     continue; // Skip the first line
            // }
            stringstream ss(*it);
            string date, price1Str, price2Str;
            if (getline(ss, date, ',') && getline(ss, price1Str, ',') && getline(ss, price2Str, ',')) {
                try {
                    double price1 = stod(price1Str);
                    double price2 = stod(price2Str);
                    data.emplace_back(date, price1, price2);
                } catch (const invalid_argument& e) {
                    // cerr << "Invalid argument: " << e.what() << endl;
                } catch (const out_of_range& e) {
                    cerr << "Out of range: " << e.what() << endl;
                }
            } else {
                cerr << "Error: Invalid format in line: " << *it << endl;
            }
        }

        file.close();
        return data;
    }
};

vector<StockData> getLastNElements(const std::vector<StockData>& data, int n) {
    // Check if n is greater than or equal to the size of the vector
    if (n >= data.size()) {
        return data;  // Return the entire vector
    } else {
        // Get an iterator pointing to the start of the last n elements
        auto it = std::prev(data.end(), n);
        // Create a new vector containing the last n elements
        return std::vector<StockData>(it, data.end());
    }
}

int main(int argc, char* argv[]) {
    int x;
    int n;
    double threshold;
    //print the number of arguments

    if(argc!=9){
        cout<<"Invalid number of arguments"<<endl;
        return 0;
    }
    else{
        x=stoi(argv[4]);
        n=stoi(argv[5]);
        threshold=stoi(argv[6]);
    }
    int extra_days=n-1;
    vector<StockData> Data = StockData::readFromFile("testingdata.csv");
    vector<StockData> Extra = StockData::readFromFile("extra.csv");
    vector<StockData> Dataf=getLastNElements(Extra,extra_days);
    Data.insert(Data.begin(), Dataf.begin(), Dataf.end());
    for(int i=0;i<Data.size();i++){
        Data[i].setspread(Data[i].getPrice1() - Data[i].getPrice2());
    }



    for (int i = extra_days; i < Data.size(); i++) {
        double sum = 0;
        double sumSquaredDiff = 0; // Sum of squared differences for calculating standard deviation
        for (int j = i - n+1; j < i+1; j++) {
            sum += Data[j].getSpread();
        }
        double mean = sum / n; // Calculate the rolling mean
        Data[i].setRollingMean(mean);
        for (int j = i - n+1; j < i+1; j++) {
            sumSquaredDiff += pow(Data[j].getSpread() - mean, 2); // Sum of squared differences
        }
        double variance = sumSquaredDiff / n; // Calculate the rolling variance
        double stdDev = sqrt(variance); // Calculate the rolling standard deviation
        Data[i].setRollingStdDev(stdDev); // Set the rolling standard deviation
        Data[i].setz_score((Data[i].getSpread() - mean) / stdDev); // Calculate the z-score
    }
    // for(int i=0;i<Data.size();i++){
    //     cout<<Data[i].getDate()<<"    Z: "<<Data[i].getz_score()<<" Spread   "<<Data[i].getSpread()<<"   Mean:   "<<Data[i].getRollingMean()<<"   Dev  "<<Data[i].getRollingStdDev()<<endl;
    // }
    int holding = 0;
    for (int i=n; i<Data.size(); i++) {
        if (Data[i].getz_score() > threshold && holding > -x) {
            Data[i].sell();
            holding--;
            Data[i].setQuantity(holding);

        } else if (Data[i].getz_score() < -threshold && holding < x) {
            Data[i].buy();
            holding++;
            Data[i].setQuantity(holding);
        }
    }

    // for (const auto& entry : Data) {
    //     cout << "Date: " << entry.getDate() << ", Spread: " << entry.getSpread() << ", Rolling Mean: " << entry.getRollingMean() << ", Rolling StdDev: " << entry.getRollingStdDev() << ", Z-Score: " << entry.getz_score() << ", Action: " << entry.getAction() << ", Quantity: " << entry.getQuantity() << endl;
    // }

    ofstream file1("order_statistics_2.csv");
    ofstream file2("order_statistics_1.csv");
    file1 << "Date,Order_dir,Quantity,Price" << endl;
    file2 << "Date,Order_dir,Quantity,Price" << endl;
    for (const auto& entry : Data) {
        if(entry.getAction()=="BUY"){
            file1 << entry.getDate() << "," << entry.getAction()<<","<<"1"<<","<<entry.getPrice1() << endl;
            file2 << entry.getDate() << "," <<"SELL"<<","<<"1"<<","<<entry.getPrice2() << endl;
        }
        if(entry.getAction()=="SELL"){
            file1 << entry.getDate() << "," << entry.getAction()<<","<<"1"<<","<<entry.getPrice1() << endl;
            file2 << entry.getDate() << "," << "BUY"<<","<<"1"<<","<<entry.getPrice2() << endl;
        }
    }
    file1.close();
    ofstream file4("daily_cashflow.csv");
    file4 << "Date,Cashflow" << endl;
    double pnl=0;
    int faltu=0;
    for (const auto& entry : Data) {
        if (faltu<extra_days){
            faltu++;
            continue;
        }
        if (entry.getAction()=="BUY"){
            pnl-=entry.getSpread();
        }
        else if(entry.getAction()=="SELL"){
            pnl+=entry.getSpread();
        }
        file4 << std::fixed << std::setprecision(2) << entry.getDate() << "," << pnl << endl;    }
    pnl+=(Data[Data.size()-1].getSpread()*holding);
    ofstream file3("final_pnl.txt");
    file3 << pnl;
    file3.close();
    file4.close();    
    return 0;
}