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
    double dma;
    string action="";
    int Holding=0;
    double ER;
    double SF;
    double AMA;
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
    double getER() const {
        return ER;
    }
    void setER(double ER) {
        this->ER = ER;
    }
    double getSF() const {
        return SF;
    }
    void setSF(double SF) {
        this->SF = SF;
    }
    double getAMA() const {
        return AMA;
    }
    void setAMA(double AMA) {
        this->AMA = AMA;
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
    int x;
    double p,n,c1,c2,max_holding;
    string symbol;
    // int x=4;
    // double n=14;
    // double c1=2;
    // double c2=0.2;
    // int max_holding=28;
    if(argc==11){
        x=stoi(argv[3]);
        p=stod(argv[4]);
        n=stod(argv[5]);
        c1=stod(argv[7]);
        c2=stod(argv[8]);
        max_holding=stoi(argv[6]);
        symbol=argv[2];
    }
    else{
        cout<<"Invalid number of arguments"<<endl;
        return 0;
    }
    string filename = symbol + ".csv";

    int extra_days=n-1;
    vector<StockData> Data = StockData::readFromFile(filename);
    vector<StockData> Extra = StockData::readFromFile("extra.csv");
    vector<StockData> Dataf=getLastNElements(Extra,extra_days);
    Data.insert(Data.begin(), Dataf.begin(), Dataf.end());
    // cout << "Data read successfully!" << endl;
    // cout << Data.size()<<endl;

    for (int i = extra_days; i < Data.size(); i++) {
        double abs_price_change=0;
        for (int j=i-n+1; j<i+1; j++) {
            if (Data[j].getPrice()>Data[j-1].getPrice()) {
                abs_price_change+=Data[j].getPrice()-Data[j-1].getPrice();
            }
            else {
                abs_price_change-=(Data[j].getPrice()-Data[j-1].getPrice());
            }
        }
        Data[i].setER((Data[i].getPrice()-Data[i-n].getPrice())/abs_price_change);
    }

    Data[n-1].setSF(0.5);
    Data[n-1].setAMA(Data[n-1].getPrice());
    double SF_l=0.5;
    double SF;
    double ER;
    double AMA_l=Data[n-1].getPrice();
    double AMA;
    double price;
    for (int i = n; i < Data.size(); i++) {
        ER=Data[i].getER();
        SF=SF_l+c1*((((2*ER)/(1+c2)-1)/((2*ER)/(1+c2)+1))-SF_l);
        SF_l=SF;
        Data[i].setSF(SF);
        // if (SF<0) SF=-1*SF;
        AMA=AMA_l+(SF)*(Data[i].getPrice()-AMA_l);
        Data[i].setAMA(AMA);
        AMA_l=AMA;
    }
    // cout<<Data.size()<<endl;
    // for (int i = n-1; i < Data.size(); i++) {
    //     auto entry = Data[i];
    //     cout <<i<< " Date: " << entry.getDate() << ", Price: " << entry.getPrice() << ", AMA: "<<entry.getAMA()<<", SF: "<<entry.getSF()<<", ER: "<<entry.getER()<< endl;
    // }
    queue<int> Stop_loss_buy;
    queue<int> Stop_loss_sell;
    int holding=0;
    int triger=0;
    for (int i = n-1; i < Data.size(); i++) {
        // if (Stop_loss_buy.size()>0 && Stop_loss_buy.front()+max_holding<=i){
        //     triger+=1;
        //     Data[i].sell();
        //     holding--;
        //     Data[i].setHolding(holding);
        //     Stop_loss_buy.pop();

        // }
        // else if (Stop_loss_sell.size()>0 && Stop_loss_sell.front()+max_holding<=i){
        //     triger+=1;
        //     Data[i].buy();
        //     holding++;
        //     Data[i].setHolding(holding);
        //     Stop_loss_sell.pop();
        // }
        // else 
        if (Data[i].getPrice()-Data[i].getAMA()>(p/100)*Data[i].getPrice()){
            // cout<<Data[i].getPrice()-Data[i].getAMA()<<"   "<<(p/100)*Data[i].getPrice()<<endl;
            if (holding<0){
                Data[i].buy();
                holding++;
                Data[i].setHolding(holding);
                Stop_loss_sell.pop();
            }
            else if (holding<x){
                Data[i].buy();
                holding++;
                Data[i].setHolding(holding);
                Stop_loss_buy.push(i);
            }
        }
        else if (Data[i].getAMA()-Data[i].getPrice()>p*Data[i].getPrice()/100 ){
            if (holding>0){
                Data[i].sell();
                holding--;
                Data[i].setHolding(holding);
                Stop_loss_buy.pop();
            }
            else if (holding>-x){
                Data[i].sell();
                holding--;
                Data[i].setHolding(holding);
                Stop_loss_sell.push(i);
            }
        }
       // cout<<i<< " Date: " << Data[i].getDate() << ", Price-AMA: " << Data[i].getPrice()-Data[i].getAMA()<<" p/100 "<<p*Data[i].getPrice()/100 <<" Action: "<<Data[i].getAction()<<", Holding: "<<Data[i].getHolding() << endl;
    }
    for (int i = n+1; i < Data.size(); i++) {
        auto entry = Data[i];
        // cout <<i<< " Date: " << entry.getDate() << ", Price: " << entry.getPrice() <<" Action: "<<entry.getAction()<<", Holding: "<<entry.getHolding() << ", AMA: "<<entry.getAMA()<<", SF: "<<entry.getSF()<<", ER: "<<entry.getER()<< endl;
    }
    // cout<<"Triger"<<triger<<endl;
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
    file2.close();
    pnl+=(Data[Data.size()-1].getPrice()*holding);
    ofstream file3("final_pnl.txt");
    file3 << pnl;
    file3.close();
    return 0;
}