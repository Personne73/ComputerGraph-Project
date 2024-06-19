#include <cstring>
#include <cmath>

class mat4 {
public:
    float data[16];

    // Constructeur par défaut - matrice identité
    mat4() {
        identity();
    }

    // Met à zéro la matrice
    void zero() {
        memset(data, 0, sizeof(data));
    }

    // Matrice identité
    void identity() {
        zero();
        data[0] = data[5] = data[10] = data[15] = 1.0f;
    }

    // Multiplication de matrices
    mat4 operator*(const mat4& other) const {
        mat4 result;
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                result.data[col + row * 4] = 
                    data[0 + row * 4] * other.data[col + 0] +
                    data[1 + row * 4] * other.data[col + 4] +
                    data[2 + row * 4] * other.data[col + 8] +
                    data[3 + row * 4] * other.data[col + 12];
            }
        }
        return result;
    }

    // Crée une matrice de translation
    static mat4 translate(float x, float y, float z) {
        mat4 result;
        result.data[12] = x;
        result.data[13] = y;
        result.data[14] = z;
        return result;
    }

    // Crée une matrice de rotation autour de l'axe Y
    static mat4 rotateY(float angle) {
        mat4 result;
        float c = cos(angle);
        float s = sin(angle);
        result.data[0] = c;
        result.data[2] = -s;
        result.data[8] = s;
        result.data[10] = c;
        return result;
    }

    // Crée une matrice d'échelle
    static mat4 scale(float x, float y, float z) {
        mat4 result;
        result.data[0] = x;
        result.data[5] = y;
        result.data[10] = z;
        return result;
    }
};
