#include <iostream>
#include <cmath>
#include "mat4.cpp"

// Fonction pour comparer deux matrices
bool compareMatrices(const mat4& m1, const mat4& m2, float epsilon = 0.0001f) {
    for (int i = 0; i < 16; i++) {
        if (std::fabs(m1.data[i] - m2.data[i]) > epsilon) {
            return false;
        }
    }
    return true;
}

void printMatrix(const mat4& m) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            std::cout << m.data[col + row * 4] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void testIdentity() {
    mat4 m;
    mat4 identity;
    if (compareMatrices(m, identity)) {
        std::cout << "Test Identity: PASSED\n";
    } else {
        std::cout << "Test Identity: FAILED\n";
    }
}

void testTranslation() {
    mat4 translation = mat4::translate(1.0f, 2.0f, 3.0f);
    mat4 expected;
    expected.data[12] = 1.0f;
    expected.data[13] = 2.0f;
    expected.data[14] = 3.0f;

    if (compareMatrices(translation, expected)) {
        std::cout << "Test Translation: PASSED\n";
    } else {
        std::cout << "Test Translation: FAILED\n";
    }
}

void testRotationY() {
    float angle = M_PI / 4; // 45 degrees
    mat4 rotation = mat4::rotateY(angle);
    mat4 expected;
    expected.data[0] = expected.data[10] = std::cos(angle);
    expected.data[2] = -std::sin(angle);
    expected.data[8] = std::sin(angle);

    if (compareMatrices(rotation, expected)) {
        std::cout << "Test RotationY: PASSED\n";
    } else {
        std::cout << "Test RotationY: FAILED\n";
    }
}

void testScale() {
    mat4 scale = mat4::scale(2.0f, 3.0f, 4.0f);
    mat4 expected;
    expected.data[0] = 2.0f;
    expected.data[5] = 3.0f;
    expected.data[10] = 4.0f;

    if (compareMatrices(scale, expected)) {
        std::cout << "Test Scale: PASSED\n";
    } else {
        std::cout << "Test Scale: FAILED\n";
    }
}

void testMultiplication() {
    mat4 m1 = mat4::translate(1.0f, 2.0f, 3.0f);
    mat4 m2 = mat4::scale(2.0f, 2.0f, 2.0f);
    mat4 result = m1 * m2;

    mat4 expected;
    expected.data[0] = 2.0f;
    expected.data[5] = 2.0f;
    expected.data[10] = 2.0f;
    expected.data[12] = 1.0f;
    expected.data[13] = 2.0f;
    expected.data[14] = 3.0f;
    expected.data[15] = 1.0f;

    std::cout << "Matrix m1 (Translation):\n";
    printMatrix(m1);

    std::cout << "Matrix m2 (Scale):\n";
    printMatrix(m2);

    std::cout << "Result Matrix (m1 * m2):\n";
    printMatrix(result);

    std::cout << "Expected Matrix:\n";
    printMatrix(expected);

    if (compareMatrices(result, expected)) {
        std::cout << "Test Multiplication: PASSED\n";
    } else {
        std::cout << "Test Multiplication: FAILED\n";
    }
}

int main() {
    testIdentity();
    testTranslation();
    testRotationY();
    testScale();
    testMultiplication();

    return 0;
}


// Path: Projet/common/mat4.cpp
// compile :  g++ -o tm4 testsMat4.cpp mat4.cpp
