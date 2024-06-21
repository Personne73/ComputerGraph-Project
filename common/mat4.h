#ifndef MAT4_H
#define MAT4_H

#include <array>
#include <cmath>

class mat4 {
public:
    std::array<std::array<float, 4>, 4> m;

    // Constructeur par défaut : initialise une matrice identité
    mat4();

    // Multiplication de matrices
    mat4 operator*(const mat4& other) const;

    // Méthodes statiques pour les transformations
    static mat4 translate(float x, float y, float z);
    static mat4 scale(float x, float y, float z);
    static mat4 rotateX(float angle);
    static mat4 rotateY(float angle);
    static mat4 rotateZ(float angle);

    // Conversion en tableau de float pour OpenGL
    const float* toArray() const;
};

#endif // MAT4_H
