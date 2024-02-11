#include<bits/stdc++.h>
using namespace std;
#include <fstream>
#include <sstream>
#include <iomanip> // for std::get_time
#include <ctime> // for std::tm
#include "matrix_functions.h"
typedef double ftype; // floating point type
const int NMAX = 100; // maximum number of days
const int NVAR = 8;   // number of independent variables
int n;                // number of samples
vector<ftype> y(NMAX);        // dependent variable
vector<vector<ftype>> x(NMAX, vector<ftype>(NVAR));  // independent variables
vector<ftype> B(NVAR, 0); // beta matrix
vector<vector<ftype>> inverseMatrix(const vector<vector<ftype>>& originalMatrix); // Function to invert a matrix
vector<vector<ftype>> transpose(const vector<vector<ftype>>& matrix); // Function to calculate the transpose of a matrix
vector<vector<ftype>> multiply(const vector<vector<ftype>>& matrix1, const vector<vector<ftype>>& matrix2); // Function to calculate the multiplication of two matrices
ftype determinant(const vector<vector<ftype>>& matrix); // Function to calculate the determinant of a matrix

// linear regression functions : Close(t) = β0+β1Close(t−1)+β2Open(t−1)+β3VWAP(t−1)+β4Low(t−1)+β5High(t−1)+β6(No of Trades)(t−1)+β7Open(t)
// i have to read the data from the csv file and then calculate the covariance matrix and then solve the normal equations and then predict the response
/* here is the csv format of the data: DATE,OPEN,HIGH,LOW,CLOSE,VWAP,NO OF TRADES
                                      2024-02-09,703.65,728.35,699.55,725.25,711.47,757763 */
// Function to read data

void readData(string start_training_date, string end_training_date) {
    string filename = "SBIN.csv";
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Could not open file " << filename << endl;
        return;
    }

    // Convert the input dates to time_t for comparison
    std::tm tm_start = {}, tm_end = {};
    std::istringstream ss_start(start_training_date), ss_end(end_training_date);
    ss_start >> std::get_time(&tm_start, "%d/%m/%Y");
    ss_end >> std::get_time(&tm_end, "%d/%m/%Y");
    tm_start.tm_mday--; // Shift the start date one day earlier
    tm_end.tm_mday--; // Shift the end date one day earlier
    time_t start_time = mktime(&tm_start), end_time = mktime(&tm_end);

    string line;
    getline(file, line); // Skip the header line

    vector<string> lines;
    while (getline(file, line)) {
        lines.push_back(line);
    }

    file.close();

    n = 0;
    vector<ftype> prev_day(NVAR, 0); // Store the previous day's data
    vector<ftype> current_day(NVAR, 0); // Store the current day's data
    for (int idx = lines.size() - 1; idx >= 0; --idx) {
        stringstream ss(lines[idx]);
        string date;
        getline(ss, date, ','); // Read the date

        // Convert the date to time_t for comparison
        std::tm tm = {};
        std::istringstream ss_date(date);
        ss_date >> std::get_time(&tm, "%Y-%m-%d");
        time_t current_time = mktime(&tm);

        // Skip the rows outside the date range
        if (current_time < start_time || current_time > end_time) {
            continue;
        }

        // Read the dependent variable (CLOSE)
        for (int i = 0; i < 4; ++i) {
            getline(ss, line, ',');
        }
        y[n] = stod(line);

        // Read the independent variables
        ss.seekg(0, ios::beg);
        getline(ss, line, ','); // Skip the date

        // First column should be 1
        x[n][0] = 1;

        // Rest of the columns should contain the previous day's data
        for (int j = 1; j < NVAR; ++j) {
            getline(ss, line, ',');
            prev_day[j] = stod(line); // Store the current day's data for the next iteration
            x[n][j] = prev_day[j];
        }

        // The 8th column should contain the current day's opening price
        x[n][7] = prev_day[1];

        ++n;
        if (n >= NMAX) {
            break;
        }
    }
}
// Function to compute sample mean
// ftype mean(vector<ftype>& arr) {
//     ftype sum = 0;
//     for (int i = 0; i < arr.size(); ++i) {
//         sum += arr[i];
//     }
//     return sum / arr.size();
// }



// Function to solve normal equations
void solveNormalEqns(const vector<vector<ftype>> x, const vector<ftype>y, vector<ftype>& B){
   // im going to calulate B(beta matrix) by using the formula B = (X^T*X)^-1*X^T*Y
   vector<vector<ftype>> xT = transpose(x);
    vector<vector<ftype>> xTx = multiply(xT, x);
    vector<vector<ftype>> invxTx = inverseMatrix(xTx);
    vector<vector<ftype>> invxTx_xT = multiply(invxTx, xT);
   // now i need to multiply the above matrix(NVAR*n) with y which is n*1 matrix
    for(int i=0;i<NVAR;i++){
        for(int j=0;j<n;j++){
            B[i] += invxTx_xT[i][j]*y[j];
        }
    }
    }


// Function to predict response
//you will be given the beta matrix and new row of the independent variables and you have to predict the response
ftype predictResponse(vector<ftype>& x0) {
 void solveNormalEqns(const vector<vector<ftype>> x, const vector<ftype>y, vector<ftype>& B);
 ftype response=0.0;
 for(int i=0 ;i<NVAR;i++){
      response += B[i]*x0[i];
 }
    return response;
}


int main(){
    string start_training_date = "02/01/2024";
    string end_training_date = "08/01/2024";
    readData(start_training_date, end_training_date);
    predictResponse(x[0]);
    cout<<predictResponse(x[0]);
    return 0;
    
}