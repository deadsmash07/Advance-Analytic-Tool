#ifndef MATRIX_INVR_H
#define MATRIX_INVR_H

#include <vector>
typedef double ftype; // floating point type

// Function to print a matrix
void printMatrix(const std::vector<std::vector<ftype>>& matrix);

ftype determinant(const std::vector<std::vector<ftype>>& matrix);

// Function to swap two rows of a matrix
void swapRows(std::vector<std::vector<ftype>>& matrix, int row1, int row2);

// Function to perform Gauss-Jordan elimination for matrix inversion
std::vector<std::vector<ftype>> inverseMatrix(const std::vector<std::vector<ftype>>& originalMatrix);

#endif // MATRIX_INVR_H