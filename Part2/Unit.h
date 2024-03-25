#pragma once

#include <vector>
#include "SDL.h"
#include "Vector2D.h"
#include "Level.h"
#include "TextureLoader.h"

// Forward declaration de la classe Game
//class Game;

/**
 * @brief Représente une unité dans le jeu.
 */
class Unit
{
public:
    /**
     * @brief Constructeur de la classe Unit.
     *
     * @param renderer Le renderer SDL pour le dessin.
     * @param setPos La position initiale de l'unité.
     */
    Unit(SDL_Renderer* renderer, Vector2D setPos);

    /**
     * @brief Met à jour l'unité.
     *
     * @param dT Le temps écoulé depuis la dernière mise à jour.
     * @param level Le niveau sur lequel l'unité évolue.
     * @param listUnits La liste des unités présentes dans le jeu.
     */
    void update(float dT, Level& level, std::vector<Unit>& listUnits);

    /**
     * @brief Dessine l'unité sur l'écran.
     *
     * @param renderer Le renderer SDL.
     * @param tileSize La taille des tuiles du jeu.
     */
    void draw(SDL_Renderer* renderer, int tileSize);

    /**
     * @brief Vérifie si l'unité chevauche une autre unité.
     *
     * @param posOther La position de l'autre unité.
     * @param sizeOther La taille de l'autre unité.
     * @return true si les unités se chevauchent, sinon false.
     */
    bool checkOverlap(Vector2D posOther, float sizeOther);

    /**
     * @brief Calcule la séparation normale entre cette unité et d'autres unités.
     *
     * @param listUnits La liste des unités présentes dans le jeu.
     * @return Un vecteur 2D représentant la séparation normale.
     */
    Vector2D computeNormalSeparation(std::vector<Unit>& listUnits);

private:
    Vector2D pos; /**< Position de l'unité dans le monde. */
    Vector2D posDraw; /**< Position de l'unité pour le dessin. */
    static const float speed; /**< Vitesse de l'unité. */
    static const float size; /**< Taille de l'unité. */
    SDL_Texture* texture = nullptr; /**< Texture de l'unité. */
};
