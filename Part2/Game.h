#pragma once

#include <vector>
#include <chrono>
#include <memory>
#include "SDL.h"
#include "Unit.h"
#include "Level.h"

/**
 * @brief Classe représentant le jeu principal.
 *
 * Cette classe gère la logique principale du jeu.
 */
class Game
{
private:
    enum class PlacementMode {
        wall,
        units
    } placementModeCurrent; /**< Mode de placement actuel. */

public:
    /**
     * @brief Constructeur de la classe Game.
     *
     * @param window Fenêtre SDL pour le rendu.
     * @param renderer Rendu SDL.
     * @param windowWidth Largeur de la fenêtre.
     * @param windowHeight Hauteur de la fenêtre.
     */
    Game(SDL_Window* window, SDL_Renderer* renderer, int windowWidth, int windowHeight);

    /**
     * @brief Destructeur de la classe Game.
     */
    ~Game();

private:
    /**
     * @brief Traite les événements du jeu.
     *
     * @param renderer Rendu SDL.
     * @param running Indique si le jeu est en cours d'exécution.
     */
    void processEvents(SDL_Renderer* renderer, bool& running);

    /**
     * @brief Met à jour l'état du jeu.
     *
     * @param dT Temps écoulé depuis la dernière mise à jour.
     */
    void update(float dT);

    /**
     * @brief Dessine le contenu du jeu.
     *
     * @param renderer Rendu SDL.
     */
    void draw(SDL_Renderer* renderer);

    /**
     * @brief Ajoute une unité/Enemy à une position donnée.
     *
     * @param renderer Rendu SDL.
     * @param posMouse Position de la souris.
     */
    void addUnit(SDL_Renderer* renderer, Vector2D posMouse);

    /**
     * @brief Supprime les unités/Enemy à la position de la souris.
     *
     * @param posMouse Position de la souris.
     */
    void removeUnitsAtMousePosition(Vector2D posMouse);

    int mouseDownStatus = 0; /**< Statut de la souris (clic ou pas de clic). */
    const int tileSize = 48; /**< Taille des sprites en unités de Mapping (tiles). */
    Level level; /**< Niveau du jeu. */
    std::vector<Unit> listUnits; /**< Liste dynamique des unités. */
    SDL_Texture* textureOverlay = nullptr; /**< Texture de superposition. */
    bool overlayVisible = true; /**< Indique si la superposition est visible. */
};
