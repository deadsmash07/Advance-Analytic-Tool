#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

class StockData {
private:
    string date;
    double price;
    double dma;
    string action="";
    int quantity=0;

public:
    // Constructor
    StockData(const string& date, double price) : date(date), price(price) {}

    // Getter methods
    string getDate() const {
        return date;
    }
    int getQuantity() const{
        return quantity;
    }
    void setQuantity(int quantity){
        this->quantity=quantity;
    }
    int getdma() const {
        return dma;
    }
    string getAction() const {
        return action;
    }
    void setdma(double dma) {
        this->dma = dma;
    }
    void buy(){
        this->action="BUY";
    }
    void sell(){
        this->action="SELL";
    }

    double getPrice() const {
        return price;
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
    // Read lines in reverse order
    for (auto it = lines.rbegin(); it != lines.rend(); ++it) {
        stringstream ss(*it);
        string date;
        string priceStr;
        if (getline(ss, date, ',') && getline(ss, priceStr, ',')) {
            try {
                double price = stod(priceStr);
                data.emplace_back(date, price);
            } catch (const invalid_argument& e) {
                // cerr << "Invalid argument: " << e.what() << endl;
                continue;
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
    int n;
    double x,p;
    string symbol;
    if (argc != 8) {
        cerr << "Usage: " << argv[0] << " <n> <x> <p> <symbol>" << endl;
        return 1;
    }
    try {
        n = stoi(argv[3]);
        x = stod(argv[4]);
        p = stod(argv[5]);
        symbol = argv[2];
    } catch (const invalid_argument& e) {
        cerr << "Invalid argument: " << e.what() << endl;
        return 1;
    } catch (const out_of_range& e) {
        cerr << "Out of range: " << e.what() << endl;
        return 1;
    }
    string filename = symbol + ".csv";
    int extra_days=n;
    vector<StockData> Data = StockData::readFromFile(filename);
    vector<StockData> Extra = StockData::readFromFile("extra.csv");
    vector<StockData> Dataf=getLastNElements(Extra,extra_days);
    Data.insert(Data.begin(), Dataf.begin(), Dataf.end());
    double sum;
    double sumSquaredDiff;
    int holding=0;
    for (int i = n; i < Data.size(); i++){
        sum = 0;
        for (int j = i - n+1; j < i+1; j++) {
            sum += Data[j].getPrice();
        }
        double mean = sum / n;
        Data[i].setdma(mean);
        sumSquaredDiff = 0;
        for (int j = i - n+1; j <1+ i; j++){
            sumSquaredDiff += pow(Data[j].getPrice() - mean, 2);
        }
        double standardDeviation = sqrt(sumSquaredDiff / n);

        if(Data[i].getPrice() > (mean + (p * standardDeviation)) &&  holding < x){
            Data[i].buy();
            holding++;
            Data[i].setQuantity(holding);
        }
        else if(Data[i].getPrice() < (mean - (p * standardDeviation)) && holding > -x){
            Data[i].sell();
            holding--;
            Data[i].setQuantity(holding);
        }
        // cout<<Data[i].getDate()<<","<<Data[i].getPrice()-mean<<","<<p*standardDeviation<<endl;
    }


    ofstream file1("order_statistics.csv");
    file1 << "Date,Order_dir,Quantity,Price" << endl;
    for (const auto& entry : Data) {
        if(entry.getAction()=="BUY" || entry.getAction()=="SELL"){
            file1 << entry.getDate() << "," << entry.getAction()<<","<<"1"<<","<<entry.getPrice() << endl;
        }
    }
    file1.close();
    ofstream file2("daily_cashflow.csv");
    file2 << "Date,Cashflow" << endl;
    double pnl=0;
    for (int i=extra_days;i<Data.size();i++) {
        auto entry=Data[i];
        if (entry.getAction()=="BUY"){
            pnl-=entry.getPrice();
        }
        else if(entry.getAction()=="SELL"){
            pnl+=entry.getPrice();
        }
        file2 << entry.getDate() << "," << pnl << endl;
    }
    pnl+=(Data[Data.size()-1].getPrice()*holding);
    ofstream file3("final_pnl.txt");
    file3 << pnl;
    file3.close();
    file2.close();
    return 0;
}