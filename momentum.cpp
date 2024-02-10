#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class StockData {
private:
    string date;
    double price;
    int change=0;
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
    int getChange() const {
        return change;
    }
    string getAction() const {
        return action;
    }
    void setChange(int change) {
        this->change = change;
    }
    void buy(){
        this->action="Buy";
    }
    void sell(){
        this->action="Sell";
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

    // Skip the first line
    bool firstLineSkipped = false;

    // Read lines in reverse order
    for (auto it = lines.rbegin(); it != lines.rend(); ++it) {
        if (!firstLineSkipped) {
            firstLineSkipped = true;
            continue; // Skip the first line
        }

        stringstream ss(*it);
        string date;
        string priceStr;
        if (getline(ss, date, ',') && getline(ss, priceStr, ',')) {
            try {
                double price = stod(priceStr);
                data.emplace_back(date, price);
            } catch (const invalid_argument& e) {
                cerr << "Invalid argument: " << e.what() << endl;
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

    // Function to read data from CSV file
// static vector<StockData> readFromFile(const string& filename) {
//     vector<StockData> data;
//     ifstream file(filename);
//     if (!file.is_open()) {
//         cerr << "Error: Unable to open file " << filename << endl;
//         return data; // Return empty vector
//     }

//     string line;
//     bool firstRow = true; // Flag to indicate if it's the first row
//     while (getline(file, line)) {
//         if (!firstRow) {
//             stringstream ss(line);
//             string date;
//             string priceStr;
//             if (getline(ss, date, ',') && getline(ss, priceStr, ',')) {
//                 try {
//                     double price = stod(priceStr);
//                     data.emplace_back(date, price);
//                 } catch (const invalid_argument& e) {
//                     cerr << "Invalid argument: " << e.what() << endl;
//                 } catch (const out_of_range& e) {
//                     cerr << "Out of range: " << e.what() << endl;
//                 }
//             } else {
//                 cerr << "Error: Invalid format in line: " << line << endl;
//             }
//         } else {
//             firstRow = false; // Set the flag to false after reading the first row
//         }
//     }

//     file.close();
//     return data;
// }

};

int main() {
    int n;
    cout<<"Enter the number of days: ";
    cin>>n;
    double inc[40]={0};
    double dec[40]={0};
    cout << "Reading data from HDFC.csv..." << endl;
    vector<StockData> Data = StockData::readFromFile("SBIN.csv");
    cout << "Data read successfully!" << endl;
        for (int i = 1; i < Data.size(); i++) {
        if (Data[i].getPrice() > Data[i - 1].getPrice()) {
            if (Data[i-1].getChange()<0){
                Data[i].setChange(1);
                inc[1]+=1;
            } 
            else{
                Data[i].setChange(Data[i-1].getChange() + 1);
                inc[Data[i-1].getChange()+1]+=1;
            }
        }
        else 
            if (Data[i-1].getChange()>0) {
                Data[i].setChange(-1);
                dec[1]+=1;
            }
            else{
                Data[i].setChange(Data[i-1].getChange() - 1);
                dec[Data[i-1].getChange() * (-1) +1]+=1;
            }
    }
    // Display the data
    cout << "Displaying the data..." << endl;
    cout << "Total entries: " << Data.size() << endl;
    int answer=0;
    for(int i=0;i<20;i++)
    {
        cout<<inc[i]<<" ";
        answer+=inc[i];
    }
    cout<<endl;
    for(int i=0;i<20;i++)
    {
        cout<<dec[i]<<" ";
        answer+=dec[i];
    }
    cout<<endl;
    cout<<answer<<endl;
    answer=0;
    int counter=0;
    for (int i=0;i<Data.size()-1;i++){
        if(Data[i].getChange()==n)
        {
            answer+=Data[i+1].getChange();
            answer-=Data[i].getChange();
            Data[i].buy();
            Data[i].setQuantity(1);
            Data[i+1].setQuantity(1);
            Data[i+1].sell();
            counter++;
        }
        else 
        if(Data[i].getChange()==-n)
        {
            answer-=Data[i+1].getChange();
            answer+=Data[i].getChange();
            Data[i].setQuantity(1);
            Data[i+1].setQuantity(1);
            Data[i].sell();
            Data[i+1].buy();
            counter++;
        }
    }
    for (const auto& entry : Data) {
        cout << "Date: " << entry.getDate() << ", Price: " << entry.getPrice() <<", Change: " << entry.getChange()<<", Action: "<<entry.getAction() << endl;
    }
    cout<<"Answer: "<<answer<<endl;
    cout<<"Counter: "<<counter<<endl;
    // cout<<"Ans: "<<answer<<"Counter"<< counter <<endl;
    ofstream file("order___statistics.csv");
    file << "Date,Order_dir,Quantity,Price" << endl;
    

    for (const auto& entry : Data) {
        if(entry.getQuantity()!=0){
            file << entry.getDate() << "," << entry.getAction()<<","<<entry.getQuantity()<<","<<entry.getPrice() << endl;
        }
    }
    file.close();
    double pnl=0;
    // ofstream file("daily___pnl.csv");
    // file << "Date,Cashflow" << endl;   
    // for (const auto& entry : Data) {
    //     if(entry.getAction()=="Buy")
    //     {
    //         pnl-=entry.getPrice();
    //     }
    //     else if(entry.getAction()=="Sell")
    //     {
    //         pnl+=entry.getPrice();
    //     }
    //     file << entry.getDate() << "," << pnl << endl;
    // }
    // file.close();

    return 0;
}