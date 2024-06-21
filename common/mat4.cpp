#include "mat4.h"

mat4::mat4() {
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            m[i][j] = (i == j) ? 1.0f : 0.0f;
}

mat4 mat4::operator*(const mat4& other) const {
    mat4 result;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.m[i][j] = 0.0f;
            for (int k = 0; k < 4; ++k) {
                result.m[i][j] += m[i][k] * other.m[k][j];
            }
        }
    }
    return result;
}

mat4 mat4::translate(float x, float y, float z) {
    mat4 result;
    result.m[0][3] = x;
    result.m[1][3] = y;
    result.m[2][3] = z;
    return result;
}

mat4 mat4::scale(float x, float y, float z) {
    mat4 result;
    result.m[0][0] = x;
    result.m[1][1] = y;
    result.m[2][2] = z;
    return result;
}

mat4 mat4::rotateX(float angle) {
    mat4 result;
    float rad = angle * M_PI / 180.0f;
    result.m[1][1] = cos(rad);
    result.m[1][2] = -sin(rad);
    result.m[2][1] = sin(rad);
    result.m[2][2] = cos(rad);
    return result;
}

mat4 mat4::rotateY(float angle) {
    mat4 result;
    float rad = angle * M_PI / 180.0f;
    result.m[0][0] = cos(rad);
    result.m[0][2] = sin(rad);
    result.m[2][0] = -sin(rad);
    result.m[2][2] = cos(rad);
    return result;
}

mat4 mat4::rotateZ(float angle) {
    mat4 result;
    float rad = angle * M_PI / 180.0f;
    result.m[0][0] = cos(rad);
    result.m[0][1] = -sin(rad);
    result.m[1][0] = sin(rad);
    result.m[1][1] = cos(rad);
    return result;
}

const float* mat4::toArray() const {
    return &m[0][0];
}
