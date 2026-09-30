//Name: Erick Marte
//Lab Assignment 5 (Wed 1:00)
//Description: program performs a variety of matrix operations on two given matrices
// Reads N and two N x N matrices from a file, then performs:
//   1. print both matrices          5. swap two rows
//   2. A + B                        6. swap two columns
//   3. A * B                        7. update one element
//   4. main / secondary diagonal sums
//Inputs: a txt file containing 2 matrices and a dimension (N)
//Output: text of the modified matrices
//Sources: Claude
//Date: 9/30/2026
//Revised: 9/30/2026

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <utility>   // std::swap

using namespace std;

using Matrix = vector<vector<int>>;

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

// Number of characters needed to print one value (including a '-' sign).
int valueWidth(int value) {
    return static_cast<int>(to_string(value).length());
}

// Print a matrix with right-aligned columns. The column width is derived
// from the widest value in the matrix (plus one space of padding), with a
// minimum of 4 so small numbers match the sample layout.
void printMatrix(const Matrix& m) {
    int width = 4;
    for (const auto& row : m)
        for (int v : row)
            if (valueWidth(v) + 1 > width)
                width = valueWidth(v) + 1;

    for (const auto& row : m) {
        for (int v : row)
            cout << setw(width) << v;
        cout << '\n';
    }
    cout << '\n';
}

bool validIndex(int index, int n) {
    return index >= 0 && index < n;
}

// ---------------------------------------------------------------------------
// Problem 1: read N and two N x N matrices from a file
// Returns false (with a message on cerr) if anything goes wrong.
// ---------------------------------------------------------------------------
bool readMatrices(const string& filename, Matrix& a, Matrix& b) {
    ifstream in(filename);
    if (!in) {
        cerr << "Error: could not open file '" << filename << "'.\n";
        return false;
    }

    int n;
    if (!(in >> n)) {
        cerr << "Error: could not read matrix size N from the file.\n";
        return false;
    }
    if (n <= 0) {
        cerr << "Error: matrix size N must be a positive integer (got " << n << ").\n";
        return false;
    }

    a.assign(n, vector<int>(n));
    b.assign(n, vector<int>(n));

    for (Matrix* m : {&a, &b}) {
        for (int i = 0; i < n; ++i) {        //iterates over each row
            for (int j = 0; j < n; ++j) {    //iterates over each column
                if (!(in >> (*m)[i][j])) {    //checks if the input contains a full matrix
                    cerr << "Error: file ended early or contains a non-integer value "
                         << "(expected " << 2 * n * n << " matrix values).\n";
                    return false;
                }
            }
        }
    }
    return true;
}

// ---------------------------------------------------------------------------
// Problem 2: C = A + B
// ---------------------------------------------------------------------------
Matrix addMatrices(const Matrix& a, const Matrix& b) {
    size_t n = a.size();            //NxN matrices
    Matrix c(n, vector<int>(n));    //initialize result matrix
    for (size_t i = 0; i < n; ++i)    //iterates over each row
        for (size_t j = 0; j < n; ++j)    //iterates over each column
            c[i][j] = a[i][j] + b[i][j];    //adding all corresponding entries
    return c;
}

// ---------------------------------------------------------------------------
// Problem 3: C = A * B   (c[i][j] = sum over k of a[i][k] * b[k][j])
// ---------------------------------------------------------------------------
Matrix multiplyMatrices(const Matrix& a, const Matrix& b) {
    size_t n = a.size();
    Matrix c(n, vector<int>(n));            //initialize result matrix
    for (size_t i = 0; i < n; ++i) {          // result row
        for (size_t j = 0; j < n; ++j) {      // result column
            int sum = 0;                      // reset for every entry
            for (size_t k = 0; k < n; ++k)    // walk row i of A / column j of B
                sum += a[i][k] * b[k][j];    //summing the products of row entries x col entires
            c[i][j] = sum;
        }
    }
    return c;
}

// ---------------------------------------------------------------------------
// Problem 4: main diagonal (a[i][i]) and secondary diagonal (a[i][N-1-i])
// For odd N the centre element belongs to both diagonals and is counted in each.
// ---------------------------------------------------------------------------
void printDiagonalSums(const Matrix& m) {
    int n = static_cast<int>(m.size());
    int mainSum = 0, secondarySum = 0;
    for (int i = 0; i < n; ++i) {        //matrix has one diagonal so only one iteration is needed
        mainSum += m[i][i];                //sums entries along main diagonal
        secondarySum += m[i][n - 1 - i];    //sums entires along secondary diagonal
    }
    cout << "Main diagonal sum: " << mainSum << '\n';
    cout << "Secondary diagonal sum: " << secondarySum << "\n\n";
}

// ---------------------------------------------------------------------------
// Problem 5: swap two rows (0-based). Matrix is taken by value so the
// caller's original stays untouched for the later problems.
// ---------------------------------------------------------------------------
void swapRows(Matrix m, int r1, int r2) {
    int n = static_cast<int>(m.size());
    if (!validIndex(r1, n) || !validIndex(r2, n)) {        //validity checking
        cout << "Invalid row index (" << r1 << ", " << r2
             << "). Valid range is 0 to " << n - 1 << ". Matrix unchanged.\n\n";
        return;
    }
    swap(m[r1], m[r2]);   // swaps two whole row vectors in one step
    cout << "Problem 5 - Rows " << r1 << " and " << r2 << " swapped:\n";
    printMatrix(m);
}

// ---------------------------------------------------------------------------
// Problem 6: swap two columns (0-based)
// ---------------------------------------------------------------------------
void swapColumns(Matrix m, int c1, int c2) {
    int n = static_cast<int>(m.size());
    if (!validIndex(c1, n) || !validIndex(c2, n)) {        //validity checking
        cout << "Invalid column index (" << c1 << ", " << c2
             << "). Valid range is 0 to " << n - 1 << ". Matrix unchanged.\n\n";
        return;
    }
    for (auto& row : m)
        swap(row[c1], row[c2]);        //swaps rows
    cout << "Problem 6 - Columns " << c1 << " and " << c2 << " swapped:\n";
    printMatrix(m);
}

// ---------------------------------------------------------------------------
// Problem 7: set m[row][col] = value (0-based)
// ---------------------------------------------------------------------------
void updateElement(Matrix m, int row, int col, int value) {
    int n = static_cast<int>(m.size());
    if (!validIndex(row, n) || !validIndex(col, n)) {    //validity checking
        cout << "Invalid position (" << row << ", " << col
             << "). Valid range is 0 to " << n - 1 << ". Matrix unchanged.\n\n";
        return;
    }
    m[row][col] = value;        //sets new value
    cout << "Problem 7 - Updated matrix:\n";
    printMatrix(m);
}

// ---------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    string filename;
    cout << "Enter input filename: " << endl;
    if (argc > 1) {
        filename = argv[1];              // optional: ./main input.txt
    } else if (!(cin >> filename)) {
        cerr << "Error: no filename entered.\n";
        return 1;
    }

    Matrix a, b;
    if (!readMatrices(filename, a, b))
        return 1;

    // Problem 1
    cout << "Matrix A:\n";
    printMatrix(a);
    cout << "Matrix B:\n";
    printMatrix(b);

    // Problem 2
    cout << "A + B:\n";
    printMatrix(addMatrices(a, b));

    // Problem 3
    cout << "A * B:\n";
    printMatrix(multiplyMatrices(a, b));

    // Problem 4
    cout << "Diagonal sums for Matrix A:\n";
    printDiagonalSums(a);

    // Problems 5-7 each start from the original Matrix A
    swapRows(a, 0, 2);        //swapping rows 0, 2
    swapColumns(a, 0, 2);        //swapping columns 0, 2
    updateElement(a, 1, 2, 99);    //changing (1, 2) to 99

    return 0;
}
