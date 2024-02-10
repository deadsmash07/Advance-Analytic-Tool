#ifndef LR_H
#define LR_H

#include <vector>
using namespace std;
typedef double ftype; // floating point type

const int NMAX = 100; // maximum number of samples
const int NVAR = 2;   // number of independent variables

// Function declarations
void readData();
ftype mean(vector<ftype>& arr);
vector<vector<ftype>> calcCovMatrix(const vector<ftype>& y, const vector<vector<ftype>>& x, int n);
vector<ftype> solveNormalEqns(vector<vector<ftype>> C);
ftype predictResponse(vector<ftype>& b, vector<ftype>& x);

#endif // LR_H