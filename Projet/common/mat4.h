#ifndef MAT4_H
#define MAT4_H

#include <cmath>
#include <cstring>

class mat4 {
public:
    float data[16];

    // Constructeur par défaut - matrice identité
    mat4();

    // Réinitialise à la matrice identité
    void identity();

    // Réinitialise à une matrice nulle
    void zero();

    // Multiplication de matrices
    mat4 operator*(const mat4& other) const;

    // Méthodes statiques pour les transformations spécifiques
    static mat4 translate(float x, float y, float z);
    static mat4 rotateY(float angle);
    static mat4 scale(float x, float y, float z);

private:
    // Fonctions utilitaires internes (si nécessaire)
};

#endif // MAT4_H
