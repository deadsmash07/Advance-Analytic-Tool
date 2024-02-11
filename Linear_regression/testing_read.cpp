#include<bits/stdc++.h>
using namespace std;
#include <fstream>
#include <sstream>
#include <iomanip> // for std::get_time
#include <ctime> // for std::tm
typedef double ftype; // floating point type
const int NMAX = 100; // maximum number of samples
const int NVAR = 8;   // number of independent variables
int n;                // number of samples
vector<ftype> y(NMAX);        // dependent variable
vector<vector<ftype>> x(NMAX, vector<ftype>(NVAR)); 

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
int main() {
    // print the x and y values
    readData("02/01/2024", "10/01/2024");
    for(int i=0;i<n;i++){
        cout<<y[i]<<" ";
        for(int j=0;j<NVAR;j++){
            cout<<x[i][j]<<" ";
        }
        cout<<endl;
    }

    return 0;
}
