#include<bits/stdc++.h>
using namespace std;
#include <fstream>
#include <cmath> // For 'fabs' (absolute value)
#include <sstream>
#include <iostream>
#include <vector>
#include <iomanip> // for std::get_time
#include <ctime> // for std::tm
typedef double ftype; // floating point type
int NMAX ; // maximum number of days
int NMAX_test; // maximum number of days in the test data
const int NVAR = 8;   // number of independent variables
vector<ftype> y;        // dependent variable for training data
vector<vector<ftype>> x;  // independent variables for taining data
vector<ftype> y_test;        // dependent variable for testing data
vector<vector<ftype>> x_test;  // independent variables for testing data
vector<ftype> B; // beta matrix
vector<string> action;
vector<ftype> CashFlow;
vector<ftype> Quantity;

std::vector<std::string> DATE_TRAIN, DATE_TEST, DATE_MODIFIED;
std::vector<ftype>  OPEN_TRAIN, HIGH_TRAIN, LOW_TRAIN, CLOSE_TRAIN, VWAP_TRAIN, NO_OF_TRADES_TRAIN;
std::vector<ftype> OPEN_TEST, HIGH_TEST, LOW_TEST, CLOSE_TEST, VWAP_TEST, NO_OF_TRADES_TEST;
void storeData(string file_train_data, string file_test_data) {
    std::ifstream file_test(file_test_data);
    std::ifstream file_train(file_train_data);

    std::string line;

    // Skip the first line (header)
    std::getline(file_test, line);
    std::getline(file_train, line);

    while (std::getline(file_test, line)) {
        std::stringstream ss(line);
        std::string field;

        // Assuming each line has 7 fields
        std::getline(ss, field, ',');
        DATE_TEST.push_back((field));
        
        std::getline(ss, field, ',');
        OPEN_TEST.push_back(stod(field));

        std::getline(ss, field, ',');
        HIGH_TEST.push_back(stod(field));

        std::getline(ss, field, ',');
        LOW_TEST.push_back(stod(field));

        std::getline(ss, field, ',');
        CLOSE_TEST.push_back(stod(field));

        std::getline(ss, field, ',');
        VWAP_TEST.push_back(stod(field));

        std::getline(ss, field, ',');
        NO_OF_TRADES_TEST.push_back(stod(field));
    }
    while (std::getline(file_train, line)) {
        std::stringstream ss(line);
        std::string field;

        // Assuming each line has 7 fields
        std::getline(ss, field, ',');
        DATE_TRAIN.push_back((field));
        
        std::getline(ss, field, ',');
        OPEN_TRAIN.push_back(stod(field));

        std::getline(ss, field, ',');
        HIGH_TRAIN.push_back(stod(field));

        std::getline(ss, field, ',');
        LOW_TRAIN.push_back(stod(field));

        std::getline(ss, field, ',');
        CLOSE_TRAIN.push_back(stod(field));

        std::getline(ss, field, ',');
        VWAP_TRAIN.push_back(stod(field));

        std::getline(ss, field, ',');
        NO_OF_TRADES_TRAIN.push_back(stod(field));
    }
}

void readData() {
    // SINCE I HAVE ALL THE TRAINING DATA IN THE VECTOR I DONT NEED TO PASS THE DATES TO THE FUNCTION
    NMAX = DATE_TRAIN.size()-1; //BECUASE I HAVE ONE EXTRA DAY IN THE TRAINING DATA
    int index_start = NMAX-1;
    int index_end = 0;
    // now i have to write data for test matrix
    NMAX_test= DATE_TEST.size()-1;
    int index_start_test = NMAX_test-1;
    int index_end_test = 0;
    y_test.resize(NMAX_test);
    x_test.resize(NMAX_test, vector<ftype>(NVAR));
    // Resize y and x
    y.resize(NMAX);
    x.resize(NMAX, vector<ftype>(NVAR));

    for(int i= index_end; i<=index_start; i++){
        y[i-index_end] = CLOSE_TRAIN[i];
    }
    // updating y_test matirx
    for(int i= index_end_test; i<=index_start_test; i++){
        y_test[i-index_end_test] = CLOSE_TEST[i];
    }
    //print y test
    // for(int i=0; i<y_test.size(); i++){
    //     cout<<y_test[i]<<" ";
    // }
    // updating matrix x_test
    for(int i=0; i<NVAR; i++){
        for(int j= index_end_test; j<=index_start_test; j++){
            if(i==0){
                x_test[j-index_end_test][i] = 1;
            }
            else if(i==1){
                x_test[j-index_end_test][i] = CLOSE_TEST[j+1];
            }
            else if(i==2){
                x_test[j-index_end_test][i] = OPEN_TEST[j+1];
            }
            else if(i==3){
                x_test[j-index_end_test][i] = VWAP_TEST[j+1];
            }
            else if(i==4){
                x_test[j-index_end_test][i] = LOW_TEST[j+1];
            }
            else if(i==5){
                x_test[j-index_end_test][i] = HIGH_TEST[j+1];
            }
            else if(i==6){
                x_test[j-index_end_test][i] = NO_OF_TRADES_TEST[j+1];
            }
            else if(i==7){
                x_test[j-index_end_test][i] = OPEN_TEST[j];
            }
        }
    }
    //print the matrix x_test
    // for(int i=0; i<x_test.size(); i++){
    //     for(int j=0; j<x_test[0].size(); j++){
    //         cout<<x_test[i][j]<<" ";
    //     }
    //     cout<<endl;
    // }
    // updating matrix x
    for(int i=0; i<NVAR; i++){
        for(int j= index_end; j<=index_start; j++){
            if(i==0){
                x[j-index_end][i] = 1;
            }
            else if(i==1){
                x[j-index_end][i] = CLOSE_TRAIN[j+1];
            }
            else if(i==2){
                x[j-index_end][i] = OPEN_TRAIN[j+1];
            }
            else if(i==3){
                x[j-index_end][i] = VWAP_TRAIN[j+1];
            }
            else if(i==4){
                x[j-index_end][i] = LOW_TRAIN[j+1];
            }
            else if(i==5){
                x[j-index_end][i] = HIGH_TRAIN[j+1];
            }
            else if(i==6){
                x[j-index_end][i] = NO_OF_TRADES_TRAIN[j+1];
            }
            else if(i==7){
                x[j-index_end][i] = OPEN_TRAIN[j];
            }
        }
    }
    
}

// function to calculate transpose of a matrix
std::vector<std::vector<ftype>> transpose(const std::vector<std::vector<ftype>>& matrix){
    int n = matrix.size();
    int m = matrix[0].size();
    std::vector<std::vector<ftype>> transposedMatrix(m, std::vector<ftype>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            transposedMatrix[j][i] = matrix[i][j];}}
    return transposedMatrix;}
    


const ftype EPSILON = 1e-5; // Threshold for considering values as zero

ftype determinant(const std::vector<std::vector<ftype>>& matrix) {
    int n = matrix.size();
    if (n == 1) {
        return matrix[0][0];
    }

    ftype det = 0;
    int sign = 1;
    for (int i = 0; i < n; ++i) {
        // Creating submatrix for minor calculation
        std::vector<std::vector<ftype>> submatrix(n - 1, std::vector<ftype>(n - 1));
        for (int row = 1; row < n; ++row) {
            int subRow = 0;
            for (int col = 0; col < n; ++col) {
                if (col != i) {
                    submatrix[row - 1][subRow] = matrix[row][col];
                    subRow++;
                }
            }
        }
        ftype minorDet = determinant(submatrix);

        // Check if the determinant of the minor is close to zero
        if (std::fabs(minorDet) < EPSILON) {
            minorDet = 0;
        }

        det += sign * matrix[0][i] * minorDet;
        sign *= -1;
    }
    return det;
}



std::vector<std::vector<ftype>> inverseMatrix(const std::vector<std::vector<ftype>>& matrix) {
    int n = matrix.size();
    if (n == 0 || n != matrix[0].size()) {
        return {}; 
    }

    // Matrix of minors & cofactor matrix
    std::vector<std::vector<ftype>> minors(n, std::vector<ftype>(n, 0));
    std::vector<std::vector<ftype>> cofactors(n, std::vector<ftype>(n, 0));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            // Submatrix for minor 
            std::vector<std::vector<ftype>> submatrix(n - 1, std::vector<ftype>(n - 1));
            int subRow = 0;
            for (int row = 0; row < n; row++) {
                if (row == i) continue; 
                int subCol = 0;
                for (int col = 0; col < n; col++) {
                    if (col == j) continue;
                    submatrix[subRow][subCol] = matrix[row][col];
                    subCol++;
                }
                subRow++;
            }
            minors[i][j] = determinant(submatrix);
            cofactors[i][j] = ((i + j) % 2 == 0) ? minors[i][j] : -minors[i][j];
        }
    }

    // Adjugate matrix & determinant
    std::vector<std::vector<ftype>> adjugate(n, std::vector<ftype>(n, 0)); 
    ftype det = determinant(matrix);

    adjugate = transpose(cofactors);

    // Invalid if determinant is zero
    if (fabs(det) <= std::numeric_limits<ftype>::epsilon()) { 
        return {}; 
    }

    //  Final inverse
    std::vector<std::vector<ftype>> inverse(n, std::vector<ftype>(n, 0)); 
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            inverse[i][j] = adjugate[i][j] / det; 
        }
    }

    return inverse;
}


//function to calculate multiplication of two matrices
std::vector<std::vector<ftype>> multiply(const std::vector<std::vector<ftype>>& matrix1, const std::vector<std::vector<ftype>>& matrix2){
    int n1 = matrix1.size();
    int m1 = matrix1[0].size();
    int n2 = matrix2.size();
    int m2 = matrix2[0].size();
    std::vector<std::vector<ftype>> result(n1, std::vector<ftype>(m2));
    if (m1 != n2) {
        std::cout << "Invalid dimensions for matrix multiplication!" << std::endl;
        exit(-1);
    }
    for (int i = 0; i < n1; ++i) {
        for (int j = 0; j < m2; ++j) {
            result[i][j] = 0;
            for (int k = 0; k < m1; ++k) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
    return result;
}


void solveNormalEqns(const vector<vector<ftype>>& x, const vector<ftype>& y,  vector<ftype>& B){
   // im going to calulate B(beta matrix) by using the formula B = (X^T*X)^-1*X^T*Y
   const vector<vector<ftype>> xT = transpose(x);
   //checking the transpose of x
//    cout<<"Transpose of x: \n";
//     // for(int i=0; i<xT.size(); i++){
//     //      for(int j=0; j<xT[0].size(); j++){
//     //           cout<<xT[i][j]<<" ";
//     //      }
//     //      cout<<endl;

//     // }


   const vector<vector<ftype>> xTx = multiply(xT, x);
    //checking the multiplication of xT and x
    // cout<<"Multiplication of xT and x: \n";
    // for(int i=0; i<xTx.size(); i++){
    //      for(int j=0; j<xTx[0].size(); j++){
    //           cout<<xTx[i][j]<<" ";
    //      }
    //      cout<<endl;
    // }

   const vector<vector<ftype>> invxTx = inverseMatrix(xTx);
        //checking the inverse of xTx
    // cout<<"Inverse of xTx: \n";
    // for(int i=0; i<invxTx.size(); i++){
    //      for(int j=0; j<invxTx[0].size(); j++){
    //           cout<<invxTx[i][j]<<" ";
    //      }
    //      cout<<endl;
    // }
  const  vector<vector<ftype>> invxTx_xT = multiply(invxTx, xT);
    //checking the multiplication of invxTx and xT
    // cout<<"Multiplication of invxTx and xT: \n";
    // for(int i=0; i<invxTx_xT.size(); i++){
    //      for(int j=0; j<invxTx_xT[0].size(); j++){
    //           cout<<invxTx_xT[i][j]<<" ";
    //      }
    //      cout<<endl;
    // }
    //just to check we will multiply it by x to see if i am getting unit matrix or not
   const vector<vector<ftype>> invxTx_xTx = multiply(invxTx_xT, x);

    B.resize(NVAR, 0.0);
    for(int i=0;i<NVAR;i++){
        for(int j=0;j<NMAX;j++){
            B[i] += invxTx_xT[i][j]*y[j];
        }
    }

}
int num_portfolios=0;

// // Function to predict response
// //you will be given the beta matrix and new row of the independent variables and you have to predict the response
ftype Decide(const vector<ftype>& B, int const P, int const X,  vector<ftype>& y_test, const vector<vector<ftype>>& x_test) {
    DATE_MODIFIED.assign(DATE_TEST.begin(), DATE_TEST.end() - 1);
    vector<ftype> predicted_prices;
    predicted_prices.resize(NMAX_test);
    for(int i=0; i<NMAX_test; i++){
        ftype price = 0;
        for(int j=0; j<NVAR; j++){
            price += B[j]*x_test[i][j];
        }
        predicted_prices[i] = price;
    }
    // reesverse the predicted prices
    reverse(predicted_prices.begin(), predicted_prices.end());
// reverse date_modified here 
    reverse(DATE_MODIFIED.begin(), DATE_MODIFIED.end());
    // reverse y_test and copy it to y_test_modified
    reverse(y_test.begin(), y_test.end());   
    vector<ftype> percentage_change;
    percentage_change.resize(NMAX_test);
    for(int i=0; i<NMAX_test; i++){
        percentage_change[i] = ((predicted_prices[i]-y_test[i])/y_test[i])*100; 
    }
    // reverse percentage change
    // reverse(percentage_change.begin(), percentage_change.end());
 
    action.resize(NMAX_test);
    for(int i=0; i<NMAX_test; i++){
        if(percentage_change[i]>=P && num_portfolios<X){
            action[i] = "BUY";
            num_portfolios++;
        }
        else if(percentage_change[i]<=-P && num_portfolios>-X){
            action[i] = "SELL";
            num_portfolios--;
        }
        else{
            action[i] = "";
        }
    //cout<<DATE_MODIFIED[i]<<" :"<<percentage_change[i]<<"   "<<predicted_prices[i]<<" :"<<y_test[i]<<"  "<<P<<"  X:  "<<X<<"  Num   "<<num_portfolios<<"  Action  "<< action[i]<<endl;

    }

    // Square off any positions held at the end of the trading period
    // if(num_portfolios != 0){
    //     action[NMAX_test - 1] = num_portfolios > 0 ? "SELL" : "BUY";
    //     num_portfolios = 0;
    // }

    Quantity.resize(NMAX_test,1);
    CashFlow.resize(NMAX_test,0);
    for(int i=0; i<NMAX_test; i++){
        if(action[i]=="BUY"){
            CashFlow[i] = -y_test[i]*Quantity[i];
        }
        if(action[i]=="SELL"){
            CashFlow[i] = y_test[i]*Quantity[i];
        }
        if(i>0){
            CashFlow[i] += CashFlow[i-1];
        }
    }
}

        
        
void writeCSV(){
    //from this code i will write the required data in two csv files daily_cashflow.csv and order_statistics.csv
    std::ofstream file_cashflow("daily_cashflow.csv");
    std::ofstream file_order("order_statistics.csv");
    std::ofstream pnl("final_pnl.txt");
    file_cashflow<<"Date,Cashflow"<<endl;
    file_order<<"Date,Order_dir,Quantity,Price"<<endl; //where price is just the closing price of the stock
    for(int i=0; i<NMAX_test; i++){
        file_cashflow<<DATE_MODIFIED[i]<<","<<CashFlow[i]<<endl;
        if(action[i]!=""){
        file_order<<DATE_MODIFIED[i]<<","<<action[i]<<","<<Quantity[i]<<","<<y_test[i]<<endl;}
        
    }
    // formula to calculate pni is cashflow at the end* closing price of the stock*no of portfolios
    ftype final_pnl = CashFlow[NMAX_test-1]*y_test[NMAX_test-1]*num_portfolios;
    pnl << std::fixed << std::setprecision(4) << final_pnl << endl;
    file_cashflow.close();
    file_order.close();
}


// I WANT TO PASS THESE as arguments to main function from my command line so that i can use them in my code 
int main(int argc, char *argv[]) {
    string symbol;
    int  X, P;

    if (argc == 9) { // Check if the correct number of arguments are passed
        symbol = string(argv[2]);
        X = stoi(argv[3]); // PORTFOLIO RANGE: [-X, X]
        P = stoi(argv[4]); // PERCENTAGE DIFFERENCE REQUIRED TO TRADE (IF calculated percentage change is greater than P then SELL or BUY)
    } else {
        cerr << "Invalid number of arguments." << endl;
        return 1;
    }

    string file_train_data= symbol+"_training_data.csv";
    string file_test_data= symbol+"_testing_data.csv";
    storeData(file_train_data, file_test_data);

    readData();  // Modify if needed to pass parameters
    solveNormalEqns(x, y, B);
    Decide(B, P, X, y_test, x_test);
    writeCSV();

    return 0;
}
