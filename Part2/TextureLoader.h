#pragma once

#include <string>
#include <unordered_map>
#include "SDL.h"

/**
 * @brief Classe utilitaire pour charger et gérer les textures SDL.
 */
class TextureLoader
{
public:
    /**
     * @brief Charge une texture à partir d'un fichier.
     *
     * @param renderer Le renderer SDL.
     * @param filename Le nom du fichier de la texture.
     * @return La texture chargée, ou nullptr en cas d'erreur.
     */
    static SDL_Texture* loadTexture(SDL_Renderer* renderer, std::string filename);

    /**
     * @brief Libère toutes les textures chargées.
     */
    static void deallocateTextures();

private:
    /**
     * @brief Map associatif pour stocker les textures chargées.
     *
     * La clé est le nom du fichier de la texture, la valeur est la texture SDL associée . Genial dud.
     */
    static std::unordered_map<std::string, SDL_Texture*> umapTexturesLoaded;
};

