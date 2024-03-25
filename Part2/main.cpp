#include <iostream>
#include "SDL.h"
#include "Game.h"

int main(int argc, char* args[]) {
    // J'initialise le générateur de nombres aléatoires avec l'heure actuelle pour obtenir des nombres différents à chaque exécution du jeu.
    srand((unsigned)time(NULL));

    // J'initialise SDL pour la gestion de la vidéo.
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Erreur : Je ne peux pas initialiser SDL Vidéo = " << SDL_GetError() << std::endl;
        return 1;
    } else {
        // Je crée la fenêtre.
        SDL_Window* window = SDL_CreateWindow("Simulation de Flux de Terrain", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 668, 568, 0);
        if (window == nullptr) {
            std::cout << "Erreur : Je ne peux pas créer la fenêtre = " << SDL_GetError() << std::endl;
            return 1;
        } else {
            // Je crée un renderer pour le dessin accéléré par GPU.
            SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (renderer == nullptr) {
                std::cout << "Erreur : Je ne peux pas créer le renderer = " << SDL_GetError() << std::endl;
                return 1;
            } else {
                // J'assure le dessin correct des graphiques transparents.
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

                // J'affiche le nom du pilote de rendu.
                SDL_RendererInfo rendererInfo;
                SDL_GetRendererInfo(renderer, &rendererInfo);
                std::cout << "Renderer = " << rendererInfo.name << std::endl;

                // J'obtiens les dimensions de la fenêtre.
                int windowWidth = 0, windowHeight = 0;
                SDL_GetWindowSize(window, &windowWidth, &windowHeight);

                // Je démarre le jeu.
                Game game(window, renderer, windowWidth, windowHeight);

                // Nettoyage.
                SDL_DestroyRenderer(renderer);
            }

            // Nettoyage.
            SDL_DestroyWindow(window);
        }

        // Nettoyage.
        SDL_Quit();
    }
    return 0;
}
