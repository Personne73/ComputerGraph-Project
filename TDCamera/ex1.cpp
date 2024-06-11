#include <iostream>
#include <array>

using Mat4 = std::array<std::array<float, 4>, 4>;

// implémenter en C++ une fonction multipliant deux matrices et qui renvoit le résultat
Mat4 multiplyMatrix(const Mat4& a, const Mat4& b) {
    Mat4 result;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            result[row][col] = 0;
            for (int k = 0; k < 4; k++) {
                result[row][col] += a[row][k] * b[k][col];
            }
        }
    }

    return result;
}

int main() {
    Mat4 mat1 = {1, 2, 3, 4, 
                 0, 1, 0, 0, 
                 0, 0, 1, 0, 
                 0, 0, 0, 1};

    Mat4 mat2 = {1, 2, 2, 2, 
                 0, 1, 0, 0, 
                 0, 0, 1, 0, 
                 0, 0, 0, 1};

    Mat4 result = multiplyMatrix(mat1, mat2);

    for (const auto& row : result) {
        for (const auto& val : row) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}