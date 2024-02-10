#include<bits/stdc++.h>
using namespace std;
#include <fstream>
#include <sstream>
#include "matrix_invr.h"
typedef double ftype; // floating point type
const int NMAX = 100; // maximum number of samples
const int NVAR = 7;   // number of independent variables
int n;                // number of samples
vector<ftype> y(NMAX);        // dependent variable
vector<vector<ftype>> x(NMAX, vector<ftype>(NVAR));  // independent variables
vector<vector<ftype>> inverseMatrix(const vector<vector<ftype>>& originalMatrix); // Function to invert a matrix
ftype determinant(const vector<vector<ftype>>& matrix); // Function to calculate the determinant of a matrix
// linear regression functions : Close(t) = β0+β1Close(t−1)+β2Open(t−1)+β3VWAP(t−1)+β4Low(t−1)+β5High(t−1)+β6(No of Trades)(t−1)+β7Open(t)
// i have to read the data from the csv file and then calculate the covariance matrix and then solve the normal equations and then predict the response
/* here is the csv format of the data: DATE,OPEN,HIGH,LOW,PREV. CLOSE,CLOSE,VWAP,NO OF TRADES
                                      2024-02-09,703.65,728.35,694.2,699.55,725.25,711.47,757763 */
// Function to read data

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
        cout<<y[i]<<" ";
        for(int j=0;j<NVAR;j++){
            cout<<x[i][j]<<" ";
        }
        cout<<endl;
    }


    file.close();
}
// Function to compute sample mean
ftype mean(vector<ftype>& arr) {
    ftype sum = 0;
    for (int i = 0; i < arr.size(); ++i) {
        sum += arr[i];
    }
    return sum / arr.size();
}

vector<vector<ftype>> calcCovMatrix(const vector<vector<ftype>>& x, int n, int NVAR) {    
    vector<vector<ftype>> C(NVAR, vector<ftype>(NVAR, 0));
    vector<ftype> mX(NVAR);
    for (int j = 0; j < NVAR; ++j) {
        vector<ftype> column(n);
        for (int i = 0; i < n; ++i) {
            column[i] = x[i][j];
        }
        mX[j] = accumulate(column.begin(), column.end(), 0.0) / n;
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < NVAR; ++j) {
            for (int k = 0; k < NVAR; ++k) {
                C[j][k] += (x[i][j] - mX[j])*(x[i][k] - mX[k]);
            }
        }
    }
    for (int j = 0; j < NVAR; ++j) {
        for (int k = 0; k < NVAR; ++k) {
            C[j][k] /= (n-1);
        }
    }
    return C;
}
// Function to solve normal equations
vector<ftype> solveNormalEqns(vector<vector<ftype>> C) {
    vector<ftype> b(NVAR+1, 0);
    // using my inverseMatrix function to calculate the inverse of C
    vector<vector<ftype>> invC = inverseMatrix(C);
    ftype det = determinant(C);
    ftype mY = mean(y);
    vector<ftype> mX(NVAR);
    for (int j = 0; j < NVAR; ++j) {
        vector<ftype> column(n);
        for (int i = 0; i < n; ++i) {
            column[i] = x[i][j];
        }
        mX[j] = mean(column);
    }
    b[0] = mY;
    for (int i = 0; i < NVAR; ++i) {
        b[i+1] = 0;
        for (int j = 0; j < NVAR; ++j) {
            b[i+1] -= invC[i][j]*mX[j];
        }
    }


    return b;
}


// Function to predict response
ftype predictResponse(vector<ftype>& b, vector<ftype>& x) {
    ftype response = b[0];
    for (int i = 0; i < NVAR; ++i) {
        response += b[i+1]*x[i];
    }
    return response;
}

// int main() {
//     readData();                       // read data
//     vector<vector<ftype>> C = calcCovMatrix(y,x,n);                 // compute covariance matrix
//     vector<ftype> b = solveNormalEqns(C);            // solve normal equations
//     ftype predResp = predictResponse(b, x[0]);
//     cout << "Predicted Response: " << fixed << setprecision(2) << predResp << endl;
//     return 0;
// }