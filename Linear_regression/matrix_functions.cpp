#include <iostream>
#include <vector>
#include <cmath>
typedef double ftype; // floating point type
using namespace std;



// function to calculate transpose of a matrix
std::vector<std::vector<ftype>> transpose(const std::vector<std::vector<ftype>>& matrix){
    int n = matrix.size();
    int m = matrix[0].size();
    std::vector<std::vector<ftype>> transposedMatrix(m, std::vector<ftype>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            transposedMatrix[j][i] = matrix[i][j];}}
    return transposedMatrix;}

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

// Function to print a matrix
void printMatrix(const vector<vector<ftype>>& matrix) {
    for (const auto& row : matrix) {
        for (ftype element : row) {
            cout << element << " ";
        }
        cout << "\n";
    }
}


ftype determinant(const vector<vector<ftype>>& matrix) {
    int n = matrix.size();
    vector<vector<ftype>> lu(n, vector<ftype>(n));
    vector<int> perm(n);
    int toggle;
    ftype result = 1.0;

    // Perform LU decomposition with partial pivoting
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            lu[i][j] = matrix[i][j];
        }
    }

    for (int i = 0; i < n; ++i) {
        perm[i] = i;
    }

    toggle = 1;

    for (int c = 0; c < n - 1; ++c) {
        ftype max = abs(lu[c][c]);
        int maxRow = c;

        for (int r = c + 1; r < n; ++r) {
            if (abs(lu[r][c]) > max) {
                max = abs(lu[r][c]);
                maxRow = r;
            }
        }

        if (maxRow != c) {
            vector<ftype> temp = lu[c];
            lu[c] = lu[maxRow];
            lu[maxRow] = temp;

            int t = perm[c];
            perm[c] = perm[maxRow];
            perm[maxRow] = t;

            toggle = -toggle;
        }

        for (int i = c + 1; i < n; ++i) {
            lu[i][c] /= lu[c][c];
            for (int j = c + 1; j < n; ++j) {
                lu[i][j] -= lu[i][c] * lu[c][j];
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        result *= lu[i][i];
    }

    result= toggle * result;
    if(abs(result) < 1e-10) return 0;
    return result;
}

// Function to swap two rows of a matrix
void swapRows(vector<vector<ftype>>& matrix, int row1, int row2) {
    vector<ftype> temp = matrix[row1];
    matrix[row1] = matrix[row2];
    matrix[row2] = temp;
}

// Function to perform Gauss-Jordan elimination for matrix inversion
vector<vector<ftype>> inverseMatrix(const vector<vector<ftype>>& originalMatrix) {
    int n = originalMatrix.size();
    vector<vector<ftype>> matrix = originalMatrix;
    //adding a condition to check if determinant is zero
    if (determinant(matrix) == 0) {
        cout << "Singular Matrix! Cannot invert." << endl;
        exit(-1);
    }
    // Augment the matrix with an identity matrix
    vector<vector<ftype>> augmentedMatrix(n, vector<ftype>(2 * n, 0.0));
    for (int i = 0; i < n; ++i) {
        augmentedMatrix[i][i + n] = 1.0;
        for (int j = 0; j < n; ++j) {
            augmentedMatrix[i][j] = matrix[i][j];
        }
    }

    // Perform Gauss-Jordan elimination with partial pivoting
    for (int i = 0; i < n; ++i) {
        // Partial pivoting
        int pivotRow = i;
        for (int j = i + 1; j < n; ++j) {
            if (abs(augmentedMatrix[j][i]) > abs(augmentedMatrix[pivotRow][i])) {
                pivotRow = j;
            }
        }
        swapRows(augmentedMatrix, i, pivotRow);

        // Scale the pivot row
        ftype pivotElement = augmentedMatrix[i][i];
        for (int j = 0; j < 2 * n; ++j) {
            augmentedMatrix[i][j] /= pivotElement;
        }

        // Eliminate other rows
        for (int j = 0; j < n; ++j) {
            if (j != i) {
                ftype factor = augmentedMatrix[j][i];
                for (int k = 0; k < 2 * n; ++k) {
                    augmentedMatrix[j][k] -= factor * augmentedMatrix[i][k];
                }
            }
        }
    }

    // Extract the inverted matrix from the augmented matrix
    vector<vector<ftype>> invertedMatrix(n, vector<ftype>(n, 0.0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            invertedMatrix[i][j] = augmentedMatrix[i][j + n];
        }
    }

    return invertedMatrix;
}

// int main() {
//     // Assuming you have a given 8x8 matrix A
//     vector<vector<ftype>> A = {
//         // Initialize your matrix A with your data
//         {1, 2, 3, 4},
//         {5, 6, 2, 8},
//         {9, 1, 11, 2}
//     };
//     vector<vector<ftype>> B = {
//         // Initialize your matrix A with your data
//         {1, 2, 3, 4},
//         {5, 6, 4, 8},
//         {9, 10, 11, 12},
//         {13, 14, 15, 16}
//     };

//     // Get the inverse of the matrix A
//     vector<vector<ftype>> A_inv = inverseMatrix(A);
//     vector<vector<ftype>> transposed_A = transpose(A);
//     vector<vector<ftype>> C = multiply(A,B);

//     // Calculate the determinant of the matrix A
//     ftype det_A = determinant(A);
//     cout << "Determinant of A: " << det_A << "\n";
//     // Print the inverse matrix
//     cout << "Inverse Matrix: \n";
//     printMatrix(A_inv);
//     cout << "Transpose of A: \n";
//     printMatrix(transposed_A);
//     cout << "Multiplication of A and B: \n";
//     cout<< transposed_A.size()<<endl;
//     printMatrix(C);
//     return 0;
// }