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
    int n,x;
    // int extra_days=n+1;
    string symbol;
    if (argc !=7) {
        cerr << "Usage: " << argv[0] << " <n> <x> <filename>" << endl;
        return 1;
    }
    try {
        n = stoi(argv[3]);
        x = stoi(argv[4]);
        symbol = argv[2];
    } catch (const invalid_argument& e) {
        cerr << "Invalid argument: " << e.what() << endl;
        return 1;
    } catch (const out_of_range& e) {
        cerr << "Out of range: " << e.what() << endl;
        return 1;
    }
    int extra_days=n+1;
    string filename= symbol+".csv";
    vector<StockData> Data = StockData::readFromFile(filename);
    vector<StockData> Extra = StockData::readFromFile("extra.csv");
    vector<StockData> Dataf=getLastNElements(Extra,extra_days);
    Data.insert(Data.begin(), Dataf.begin(), Dataf.end());
    int holding=0;
    int p_momentum=0;
    // cout << Data.size() << " records read" << endl;
    for (int i=1; i<Data.size(); i++){
        if (Data[i].getPrice()>Data[i-1].getPrice()){
            p_momentum+=1;
            if (p_momentum<1) p_momentum=1;
            if (p_momentum>=n && holding<x){
                Data[i].buy();
                holding+=1;
            }
        }
        else if (Data[i].getPrice()<Data[i-1].getPrice()){
            p_momentum-=1;
            if (p_momentum>-1) p_momentum=-1;
            if (p_momentum<=-n && holding>-x){
                Data[i].sell();
                holding-=1;
            }
        }
        else{
            p_momentum=0;
        }
    }
    // for( int i=0;i<Data.size();i++){
    //     cout<<Data[i].getDate()<<","<<Data[i].getPrice()<<","<<Data[i].getAction()<<endl;
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
