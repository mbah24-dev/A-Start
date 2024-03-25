#pragma once

#include <queue>
#include <vector>
#include <string>
#include "SDL.h"
#include "Vector2D.h"
#include "TextureLoader.h"

/**
 * @brief Représente un niveau du jeu.
 *
 */
class Level
{
private:
    static const unsigned char flowDistanceMax = 255;

    /**
     * @brief Structure représentant une tuile du niveau.
     *
     */
    struct Tile {
        bool isWall = false; /**< Indique si la tuile est un mur. */
        int flowDirectionX = 0; /**< Direction du flux sur l'axe X. */
        int flowDirectionY = 0; /**< Direction du flux sur l'axe Y. */
        unsigned char flowDistance = flowDistanceMax; /**< Distance du flux. */
    };

public:
    /**
     * @brief Constructeur de la classe Level.
     *
     * @param renderer Pointeur vers le renderer SDL.
     * @param setTileCountX Nombre de tuiles en largeur.
     * @param setTileCountY Nombre de tuiles en hauteur.
     */
    Level(SDL_Renderer* renderer, int setTileCountX, int setTileCountY);

    /**
     * @brief Dessine le niveau.
     *
     * @param renderer Pointeur vers le renderer SDL.
     * @param tileSize Taille d'une tuile.
     */
    void draw(SDL_Renderer* renderer, int tileSize);

    /**
     * @brief Vérifie si une tuile est un mur.
     *
     * @param x Coordonnée X de la tuile.
     * @param y Coordonnée Y de la tuile.
     * @return true si la tuile est un mur, sinon false.
     */
    bool isTileWall(int x, int y);

    /**
     * @brief Définit si une tuile est un mur ou non.
     *
     * @param x Coordonnée X de la tuile.
     * @param y Coordonnée Y de la tuile.
     * @param setWall true pour définir la tuile comme un mur, sinon false.
     */
    void setTileWall(int x, int y, bool setWall);

    /**
     * @brief Récupère la position de la cible.
     *
     * @return La position de la cible sous forme de vecteur.
     */
    Vector2D getTargetPos();

    /**
     * @brief Définit la cible et calcule le champ de flux.
     *
     * @param targetXNew Nouvelle coordonnée X de la cible.
     * @param targetYNew Nouvelle coordonnée Y de la cible.
     */
    void setTargetAndCalculateFlowField(int targetXNew, int targetYNew);

    /**
     * @brief Récupère le vecteur normal du flux à une position donnée.
     *
     * @param x Coordonnée X de la tuile.
     * @param y Coordonnée Y de la tuile.
     * @return Le vecteur normal du flux à cette position.
     */
    Vector2D getFlowNormal(int x, int y);

private:
    /**
     * @brief Dessine une tuile spécifique.
     *
     * @param renderer Pointeur vers le renderer SDL.
     * @param x Coordonnée X de la tuile.
     * @param y Coordonnée Y de la tuile.
     * @param tileSize Taille d'une tuile.
     */
    void drawTile(SDL_Renderer* renderer, int x, int y, int tileSize);

    /**
     * @brief Calcule les distances des tuiles par rapport à la cible.
     *
     */
    void calculateDistances();

    /**
     * @brief Calcule les directions du champ de flux.
     *
     */
    void calculateFlowDirections();

    std::vector<Tile> listTiles; /**< Liste des tuiles du niveau. */
    const int tileCountX, tileCountY; /**< Nombre de tuiles en largeur et en hauteur. */
    int targetX = -1, targetY = -1; /**< Coordonnées de la cible. */

    // Textures des tuiles
    SDL_Texture* textureTileWall = nullptr;
    SDL_Texture* textureTileTarget = nullptr;
    SDL_Texture* textureTileEmpty = nullptr;
    SDL_Texture* textureTileArrowUp = nullptr;
    SDL_Texture* textureTileArrowUpRight = nullptr;
    SDL_Texture* textureTileArrowRight = nullptr;
    SDL_Texture* textureTileArrowDownRight = nullptr;
    SDL_Texture* textureTileArrowDown = nullptr;
    SDL_Texture* textureTileArrowDownLeft = nullptr;
    SDL_Texture* textureTileArrowLeft = nullptr;
    SDL_Texture* textureTileArrowUpLeft = nullptr;
};
