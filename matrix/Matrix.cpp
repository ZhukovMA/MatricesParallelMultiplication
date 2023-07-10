#include "Matrix.h"
#include <iostream>

namespace matrix {

std::shared_ptr<matrix::MatrixManager> Matrix::matrix_manager(nullptr);

Matrix::Matrix(){
    size = MATRIX_SIZE;
    data.assign(size, std::vector<double>(size, 0.0));
}

Matrix::Matrix(const Matrix& matrix){
    size = matrix.size;
    data = matrix.data;
}

Matrix::Matrix(Matrix&& matrix){
    size = matrix.size;
    data = std::move(matrix.data);
    matrix.data.assign(size, std::vector<double>(size, 0.0));
}

Matrix::Matrix(std::string& path) : Matrix(){
    from_file(path);
}

void Matrix::create_threads(size_t number_of_threads) {
    matrix_manager = std::make_shared<MatrixManager>(number_of_threads);
}

void Matrix::from_file(std::string& path){
    std::ifstream file(path, std::ios::in);
    if (!file.is_open()) {
        std::cout << "Could not open the file " << path << std::endl;
        exit(EXIT_FAILURE);
    }

    file >> *this;
    file.close();
}

void Matrix::to_file(std::string& path){
    std::ofstream file(path, std::ios::out);

    if (!file.is_open()) {
        std::cout << "Could not open the file " << path << std::endl;
        exit(EXIT_FAILURE);
    }

    file << *this;
    file.close();
}

std::ostream& operator<<(std::ostream& os, const Matrix& matrix){
    for(size_t i = 0; i < matrix.size; ++i){
        for(size_t j = 0; j < matrix.size; ++j){
            os << matrix.data[i][j] << '\t';
        }
        os << std::endl;
    }
    return os;
}

std::istream& operator>>(std::istream& is, Matrix& matrix){
    for(size_t i = 0; i < matrix.size; ++i){
        for(size_t j = 0; j < matrix.size; ++j){
            is >> matrix.data[i][j];
        }
    }
    return is;
}

std::vector<double>& Matrix::operator[](const size_t index){
    return data[index];
}

const std::vector<double>& Matrix::operator[](const size_t index) const{
    return data[index];
}

// get result matrix line 
void compute_line(const std::vector<double>& line, const Matrix& matrix, std::vector<double>& result){
    for(size_t j = 0; j < matrix.size; ++j){
        double answer = 0.0;
        for(size_t k = 0; k < matrix.size; ++k){
            answer += line[k] * matrix[k][j];
        }
        result[j] = answer;
    }
}

// matrices parallel multiplication
const Matrix operator*(const Matrix& left_matrix, const Matrix& right_matrix){
    Matrix answer;
    std::vector<std::future<void>> futures(answer.size);
    // In each thread multiply the line by the matrix. For each of the lines we will store a feature
    for(size_t i = 0; i < answer.size; ++i){
        futures[i] = Matrix::matrix_manager->create_task(compute_line, std::ref(left_matrix[i]), std::ref(right_matrix), std::ref(answer[i]));
    }

    // wait results
    for(size_t i = 0; i < answer.size; ++i){
        futures[i].get();
    }

    return answer;
}

size_t Matrix::get_size(){
    return size;
}

bool operator==(const Matrix& right_matrix, const Matrix& left_matrix){
    for(size_t i = 0; i < right_matrix.size; ++i){
        for(size_t j = 0; j < right_matrix.size; ++j){
            if(right_matrix[i][j] != left_matrix[i][j]){
                return false;
            }
        }
    }
    return true;
}

}