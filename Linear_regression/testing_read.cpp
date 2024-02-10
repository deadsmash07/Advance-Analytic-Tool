#include<bits/stdc++.h>
using namespace std;
#include <fstream>
#include <sstream>
typedef double ftype; // floating point type
const int NMAX = 100; // maximum number of samples
const int NVAR = 7;   // number of independent variables
int n;                // number of samples
vector<ftype> y(NMAX);        // dependent variable
vector<vector<ftype>> x(NMAX, vector<ftype>(NVAR)); 


void readData() {
    string filename = "SBIN.csv";
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Could not open file " << filename << endl;
        return;
    }

    string line;
    getline(file, line); // Skip the header line

    n = 0;
    while (getline(file, line)) {
        stringstream ss(line);
        string date;
        getline(ss, date, ','); // Skip the date

        // Read the dependent variable (CLOSE)
        for (int i = 0; i < 5; ++i) {
            getline(ss, line, ',');
        }
        y[n] = stod(line);

        // Read the independent variables (OPEN, HIGH, LOW, PREV. CLOSE, VWAP, NO OF TRADES)
        ss.seekg(0, ios::beg);
        getline(ss, line, ','); // Skip the date
        for (int j = 0; j < NVAR; ++j) {
            getline(ss, line, ',');
            x[n][j] = stod(line);
        }

        ++n;
        if (n >= NMAX) {
            break;
        }
    }
    //print the x and y values
    for(int i=0;i<n;i++){
        //cout<<y[i]<<" ";
        for(int j=0;j<NVAR;j++){
            cout<<x[i][j]<<" ";
        }
        cout<<endl;
    }
    

    file.close();
}
int main() {
    readData();
    return 0;
}
