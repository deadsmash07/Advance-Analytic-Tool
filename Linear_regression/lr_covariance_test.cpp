#include <gtest/gtest.h>
#include <vector>
#include <cmath>

// Assuming ftype is double
using ftype = double;

// Function prototype
std::vector<std::vector<ftype>> calcCovMatrix(const std::vector<std::vector<ftype>>& x, int n, int NVAR);

TEST(CalcCovMatrixTest, Handles2VariableData) {
    // Define a small dataset
    std::vector<std::vector<ftype>> x = {
        {1, 2},
        {2, 3},
        {3, 4},
        {4, 5},
        {5, 6}
    };
    int n = x.size();
    int NVAR = x[0].size();

    // Calculate the covariance matrix
    std::vector<std::vector<ftype>> C = calcCovMatrix(x, n, NVAR);

    // Check the size of the covariance matrix
    ASSERT_EQ(C.size(), NVAR);
    for (const auto& row : C) {
        ASSERT_EQ(row.size(), NVAR);
    }

    // Check the values of the covariance matrix
    // These values are calculated manually for this dataset
    std::vector<std::vector<ftype>> expected = {
        {2.5, 2.5},
        {2.5, 2.5}
    };
    for (int i = 0; i < NVAR; ++i) {
        for (int j = 0; j < NVAR; ++j) {
            EXPECT_NEAR(C[i][j], expected[i][j], 1e-9);
        }
    }
}