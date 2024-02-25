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
    double Long_EWM;
    double Short_EWM;
    double MACD;
    double signal;
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
    double getLong_EWM() const {
        return Long_EWM;
    }
    void setLong_EWM(double Long_EWM) {
        this->Long_EWM = Long_EWM;
    }
    double getShort_EWM() const {
        return Short_EWM;
    }
    void setShort_EWM(double Short_EWM) {
        this->Short_EWM = Short_EWM;
    }
    double getMACD() const {
        return MACD;
    }
    void setMACD(double MACD) {
        this->MACD = MACD;
    }
    double getsignal() const {
        return signal;
    }
    void setsignal(double signal) {
        this->signal = signal;
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

int main(int argc, char *argv[]) {
    int x;
    string symbol;
    if(argc ==6){
        x=stoi(argv[3]);
        symbol=argv[2];
    }
    else{
        cout<<"Invalid number of arguments"<<endl;
        return 0;
    }
    string filename = symbol + ".csv";

    vector<StockData> Data = StockData::readFromFile(filename);
    // cout << "Data read successfully!" << endl;
    // cout << Data.size()<<endl;
    double price;
    double EWM;
    Data[0].setShort_EWM(Data[0].getPrice());
    Data[0].setLong_EWM(Data[0].getPrice());
    for (int i = 1; i < Data.size(); i++) {
        price=Data[i].getPrice();
        EWM=Data[i-1].getShort_EWM();  
        EWM=EWM+(price-EWM)*2/(1+12);
        Data[i].setShort_EWM(EWM);
        EWM=Data[i-1].getLong_EWM();
        EWM=EWM+(price-EWM)*2/(1+26);
        Data[i].setLong_EWM(EWM);
        // cout<<Data[i].getDate()<<","<<Data[i].getShort_EWM()<<","<<Data[i].getLong_EWM()<<endl;
        Data[i].setMACD(Data[i].getShort_EWM()-Data[i].getLong_EWM());
        // cout<<Data[i].getDate()<<","<<Data[i].getMACD()<<endl;
    }
    
    double Signal;
    Data[0].setsignal(Data[0].getMACD());
    for (int i = 1; i < Data.size(); i++) {
        price=Data[i].getMACD();
        Signal=Data[i-1].getsignal();
        Signal=Signal+(price-Signal)*2/(1+9);
        Data[i].setsignal(Signal);
    }

    // for (int i = 0; i < Data.size(); i++) {
    //     cout<<i<< " Date: " << Data[i].getDate() << ", Price: " << Data[i].getPrice() << ", Long_EWM: " << Data[i].getLong_EWM() << ", Short_EWM: " << Data[i].getShort_EWM() << ", MACD: " << Data[i].getMACD() << ", Signal: " << Data[i].getsignal() << endl;
    // }
    // cout<<"done"<<endl;
    // for (int i=0; i<Data.size(); i++) {
    //     cout<<Data[i].getDate()<<","<<Data[i].getMACD()<<","<<Data[i].getsignal()<<endl;
    // }
    int holding=0;
    for (int i = 0; i<Data.size(); i++) {
        if (Data[i].getMACD()>Data[i].getsignal() && holding<x) {
            Data[i].buy();
            holding++;
            Data[i].setHolding(holding);
            // cout<<Data[i].getDate()<<endl;
        }
        else if (Data[i].getMACD()<Data[i].getsignal() && holding>-x) {
            Data[i].sell();
            holding--;
            Data[i].setHolding(holding);
        }
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
    for (const auto& entry : Data) {
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