#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>

using namespace std;

class StockData {
private:
    string date;
    double price;
    double Avg_gain;
    double Avg_loss;
    double RS;
    double RSI;
    string action="";
    int Holding=0;
public:
    // Constructor
    StockData(const string& date, double price) : date(date), price(price) {}
    // Getter methods
    string getDate() const {
        return date;
    }
    int getHolding() const{
        return Holding;
    }
    void setHolding(int Holding){
        this->Holding=Holding;
    }
    string getAction() const {
        return action;
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
    double getAvg_gain() const {
        return Avg_gain;
    }
    void setAvg_gain(double Avg_gain) {
        this->Avg_gain = Avg_gain;
    }
    double getAvg_loss() const {
        return Avg_loss;
    }
    void setAvg_loss(double Avg_loss) {
        this->Avg_loss = Avg_loss;
    }
    double getRS() const {
        return RS;
    }
    void setRS(double RS) {
        this->RS = RS;
    }
    double getRSI() const {
        return RSI;
    }
    void setRSI(double RSI) {
        this->RSI = RSI;
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


int main(int argc , char* argv[]) {
    int x,n;
    double oversold_threshold, overbought_threshold;
    string symbol;
    if (argc != 9) {
        cerr << "Usage: " << argv[0] << " <n> <x> <oversold_threshold> <overbought_threshold>" << endl;
        return 1;
    }
    try {
        n = stoi(argv[4]);
        x = stoi(argv[3]);
        symbol=argv[2];
        oversold_threshold = stod(argv[5]);
        overbought_threshold = stod(argv[6]);
    } catch (const invalid_argument& e) {
        cerr << "Invalid argument: " << e.what() << endl;
        return 1;
    } catch (const out_of_range& e) {
        cerr << "Out of range: " << e.what() << endl;
        return 1;
    }  
    int nnn=n;  
    int extra_days=n+1;
    string filename=symbol+".csv";
    vector<StockData> Data = StockData::readFromFile(filename);
    vector<StockData> Extra = StockData::readFromFile("extra.csv");
    vector<StockData> Dataf=getLastNElements(Extra,extra_days);
    Data.insert(Data.begin(), Dataf.begin(), Dataf.end());
    // cout << "Data read successfully!" << endl;
    // cout << Data.size()<<endl;
    double price;
    double EWM;
    double gain=0;
    double loss=0;
    for(int i=n+1;i<Data.size();i++){
        gain=0;
        loss=0;
        for(int j=i-n+1;j<=i;j++){
            if (Data[j].getPrice()>Data[j-1].getPrice()) {
                gain+=Data[j].getPrice()-Data[j-1].getPrice();
            }
            else {
                loss-=(Data[j].getPrice()-Data[j-1].getPrice());
            }
        }
        Data[i].setAvg_gain(gain);
        Data[i].setAvg_loss(loss);
        Data[i].setRS(Data[i].getAvg_gain()/Data[i].getAvg_loss());
        Data[i].setRSI(100-(100/(1+Data[i].getRS())));
        // cout<<i<< " Date: " << Data[i].getDate() << ", Price: " << Data[i].getPrice() << ", RSI: " << Data[i].getRSI() <<" Gain: "<<Data[i].getAvg_gain()<<" Loss: "<<Data[i].getAvg_loss()<<endl;
    }

    int holding=0;
    for(int i=n+1;i<Data.size();i++){
        if (Data[i].getRSI()<overbought_threshold && holding<x ) {
            Data[i].buy();
            holding++;
            Data[i].setHolding(holding);
        }
        else if (Data[i].getRSI()>oversold_threshold && holding>-x) {
            Data[i].sell();
            holding--;
            Data[i].setHolding(holding);
        }
    }
        for(int i=n+1;i<Data.size();i++){
        // cout<<i<< " Date: " << Data[i].getDate() << ", Price: " << Data[i].getPrice() << ", RSI: " << Data[i].getRSI() <<" Gain: "<<Data[i].getAvg_gain()<<" Loss: "<<Data[i].getAvg_loss()<<" Action: "<<Data[i].getAction()<<" Holding: "<<Data[i].getHolding()<<endl;
        // cout<<i<< " Date: " << Data[i].getDate() << ", Price: " << Data[i].getPrice() << ", RSI: " << Data[i].getRSI() << endl;
    }
    // for (int i = 0; i < Data.size(); i++) {
    //     cout<<i<< " Date: " << Data[i].getDate() << ", Price: " << Data[i].getPrice() << ", RSI: " << Data[i].getRSI() << ", Action: " << Data[i].getAction() << ", Holding: " << Data[i].getHolding() << endl;
    // }

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
    int k=0;
    // int nnn=n+1;
    for (const auto& entry : Data) {
        if (entry.getAction()=="BUY"){
            pnl-=entry.getPrice();
            // file2 << entry.getDate() << "," << pnl << endl;
        }
        else if(entry.getAction()=="SELL"){
            pnl+=entry.getPrice();
            
        }
        if (k>nnn){
        file2 << entry.getDate() << "," << pnl << endl;
        }
        k++;
    }
    file2.close();
    pnl+=(Data[Data.size()-1].getPrice()*holding);
    ofstream file3("final_pnl.txt");
    file3 << pnl;
    file3.close();
    return 0;
}