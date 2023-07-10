#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <memory>
#include <fstream>
#include <thread>
#include "MatrixManager.h"

namespace matrix{

#define MATRIX_SIZE 200

class Matrix{
public:
    Matrix();
    Matrix(size_t number_of_threads);
    Matrix(const Matrix& matrix);
    Matrix(Matrix&& matrix);
    Matrix(std::string& path);

    // matrices parallel multiplication
    friend const Matrix operator*(const Matrix& left, const Matrix& right);

    // get elements
    std::vector<double>& operator[](const size_t index);
    const std::vector<double>& operator[](const size_t index) const;

    // comparator
    friend bool operator==(const Matrix& right, const Matrix& left);

    // read to stream
    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix);
    // write to stream
    friend std::istream& operator>>(std::istream& is, Matrix& matrix);

    // matrix size getter
    size_t get_size();

    // write result matrix to file
    void to_file(std::string& path);
    // read matrix from file
    void from_file(std::string& path);

    // static funcrion for creating thread. Call this function before calling construction of Matrix if needed
    static void create_threads(size_t number_of_threads);

    ~Matrix(){};
private:
    // manager
    static std::shared_ptr<MatrixManager> matrix_manager;
    // matrix size
    size_t size;
    // matrix
    std::vector<std::vector<double>> data;

    // compute element A[i][j] of multiplication matrix
    friend void compute_line(const std::vector<double>& l, const Matrix& r, std::vector<double>& res);
};

}

#endif