#include "Vector2D.h"

/**
 * @brief Normalise le vecteur.
 *
 * Cette fonction normalise le vecteur en divisant ses composantes par sa magnitude,
 * rendant ainsi sa magnitude égale à 1 tout en préservant sa direction.
 *
 * @return Le vecteur normalisé.
 */
Vector2D Vector2D::normalize() {
    // Calcule la magnitude du vecteur
    float magnitudeStored = magnitude();

    // Vérifie si la magnitude est non nulle pour éviter une division par zéro
    if (magnitudeStored > 0.0f) {
        // Normalise les composantes du vecteur
        x /= magnitudeStored;
        y /= magnitudeStored;
    }

    // Retourne le vecteur normalisé
    return *this;
}
