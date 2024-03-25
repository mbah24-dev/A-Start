#pragma once
#include <cmath>

/**
 * @brief Classe représentant un vecteur bidimensionnel.
 */
class Vector2D {
public:
    /**
     * @brief Constructeur par défaut initialisant les composantes du vecteur à zéro.
     */
    Vector2D() : x(0.0f), y(0.0f) {}

    /**
     * @brief Constructeur initialisant les composantes du vecteur avec les valeurs spécifiées.
     * @param setX Valeur de l'abscisse.
     * @param setY Valeur de l'ordonnée.
     */
    Vector2D(float setX, float setY) : x(setX), y(setY) {}

    /**
     * @brief Constructeur de copie.
     * @param other Vecteur à copier.
     */
    Vector2D(const Vector2D& other) : x(other.x), y(other.y) {}

    /**
     * @brief Constructeur initialisant les composantes du vecteur à partir d'un angle en radians.
     * @param angleRad Angle en radians.
     */
    Vector2D(float angleRad) : x(cos(angleRad)), y(sin(angleRad)) {}

    /**
     * @brief Calcule l'angle du vecteur.
     * @return L'angle du vecteur en radians.
     */
    float angle() { return atan2(y, x); }

    /**
     * @brief Calcule la magnitude (norme) du vecteur.
     * @return La magnitude du vecteur.
     */
    float magnitude() { return sqrt(x * x + y * y); }

    /**
     * @brief Normalise le vecteur.
     * @return Le vecteur normalisé.
     */
    Vector2D normalize();

    /**
     * @brief Calcule le produit scalaire avec un autre vecteur.
     * @param other Le vecteur avec lequel effectuer le produit scalaire.
     * @return Le produit scalaire.
     */
    float dot(const Vector2D& other) { return x * other.x + y * other.y; }

    /**
     * @brief Calcule le produit vectoriel avec un autre vecteur.
     * @param other Le vecteur avec lequel effectuer le produit vectoriel.
     * @return Le produit vectoriel.
     */
    float cross(const Vector2D& other) { return x * other.y - y * other.x; }

    /**
     * @brief Calcule l'angle entre ce vecteur et un autre vecteur.
     * @param other Le vecteur avec lequel calculer l'angle.
     * @return L'angle entre les deux vecteurs en radians.
     */
    float angleBetween(const Vector2D& other) { return atan2(cross(other), dot(other)); }

    // Opérateurs arithmétiques

    Vector2D operator+(const float amount) { return Vector2D(x + amount, y + amount); }
    Vector2D operator-(const float amount) { return Vector2D(x - amount, y - amount); }
    Vector2D operator*(const float amount) { return Vector2D(x * amount, y * amount); }
    Vector2D operator/(const float amount) { return Vector2D(x / amount, y / amount); }

    Vector2D operator+(const Vector2D& other) { return Vector2D(x + other.x, y + other.y); }
    Vector2D operator-(const Vector2D& other) { return Vector2D(x - other.x, y - other.y); }
    Vector2D operator*(const Vector2D& other) { return Vector2D(x * other.x, y * other.y); }
    Vector2D operator/(const Vector2D& other) { return Vector2D(x / other.x, y / other.y); }

    Vector2D& operator+=(const float amount) { x += amount; y += amount; return *this; }
    Vector2D& operator-=(const float amount) { x -= amount; y -= amount; return *this; }
    Vector2D& operator*=(const float amount) { x *= amount; y *= amount; return *this; }
    Vector2D& operator/=(const float amount) { x /= amount; y /= amount; return *this; }

    Vector2D& operator+=(const Vector2D& other) { x += other.x; y += other.y; return *this; }
    Vector2D& operator-=(const Vector2D& other) { x -= other.x; y -= other.y; return *this; }
    Vector2D& operator*=(const Vector2D& other) { x *= other.x; y *= other.y; return *this; }
    Vector2D& operator/=(const Vector2D& other) { x /= other.x; y /= other.y; return *this; }

    // Composantes du vecteur
    float x, y; /// cordonnée x , y du vecteur point
};
