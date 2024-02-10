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

public:
    // Constructor
    StockData(const string& date, double price) : date(date), price(price) {}

    // Getter methods
    string getDate() const {
        return date;
    }
    int getChange() const {
        return change;
    }
    void setChange(int change) {
        this->change = change;
    }

    double getPrice() const {
        return price;
    }

    // Function to read data from CSV file
static vector<StockData> readFromFile(const string& filename) {
    vector<StockData> data;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return data; // Return empty vector
    }

    string line;
    bool firstRow = true; // Flag to indicate if it's the first row
    while (getline(file, line)) {
        if (!firstRow) {
            stringstream ss(line);
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
                cerr << "Error: Invalid format in line: " << line << endl;
            }
        } else {
            firstRow = false; // Set the flag to false after reading the first row
        }
    }

    file.close();
    return data;
}

};

int main() {
    double arr[20]={0};
    vector<StockData> Data = StockData::readFromFile("HDFC.csv");
        for (int i = 1; i < Data.size(); i++) {
        if (Data[i].getPrice() > Data[i - 1].getPrice()) {
            Data[i].setChange(Data[i-1].getChange() + 1);
            arr[Data[i-1].getChange() + 1]+=1;
        }
        else if (Data[i].getPrice() < Data[i - 1].getPrice()) {
            Data[i].setChange(0);
            arr[0]+=1;
        }
    }
    // Display the data
    // for (const auto& entry : Data) {
    //     cout << "Date: " << entry.getDate() << ", Price: " << entry.getPrice() <<", Change: " << entry.getChange() << endl;
    // }
    cout << "Total entries: " << Data.size() << endl;
    for(int i=0;i<20;i++)
    {
        cout<<arr[i]<<" ";
    }
    cout<<endl;
    for( int i=1;i<20;i++)
    {
        double x=arr[i]*100/arr[i-1];
        if(arr[i]==0) break;
        cout<<i<<":"<<x<<"%  ";
    }
    cout<<endl;
    double answer=0;
    int counter=0;
    for(int i=0;i< Data.size()-1;i++)
    {
        if(Data[i].getChange()==11)
        {
            answer-=Data[i].getChange();
            answer+=Data[i+1].getChange();
            counter++;
        }
    }
    cout<<"Ans: "<<answer<<"Counter"<< counter <<endl;
    return 0;
}