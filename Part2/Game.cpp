#include "Game.h"

// Constructeur de la classe Game avec par defaut le mode 1(Ajout des murs)
Game::Game(SDL_Window* window, SDL_Renderer* renderer, int windowWidth, int windowHeight) :
        placementModeCurrent(PlacementMode::wall),
        level(renderer, windowWidth-tileSize, windowHeight-tileSize) {

    // Initialisation du jeu
    if (window != nullptr && renderer != nullptr) {
        // Chargement de la texture d'overlay
        textureOverlay = TextureLoader::loadTexture(renderer, "Overlay.bmp");
        if(textureOverlay == nullptr) {
            // Affichage d'une erreur si le chargement de la texture échoue
            SDL_Log("Erreur lors du chargement de la texture : %s", SDL_GetError());
        }

        // Stockage du temps actuel pour le temps écoulé
        auto time1 = std::chrono::system_clock::now();
        //auto time2 = std::chrono::system_clock::now();

        // Temps pour chaque frame (80 fps)
        const float dT = 1.0f / 80.0f;

        // Boucle principale du jeu
        bool running = true;
        while (running) {
            // Calcul du temps écoulé depuis la dernière frame
            auto time2 = std::chrono::system_clock::now();
            std::chrono::duration<float> timeDelta = time2 - time1;
            float timeDeltaFloat = timeDelta.count();

            // Si suffisamment de temps s'est écoulé, je génère la prochaine frame
            if (timeDeltaFloat >= dT) {
                // Mise à jour du temps pour la prochaine frame
                time1 = time2;

                // Traitement des événements, mise à jour et dessin du jeu
                processEvents(renderer, running);
                update(dT);
                draw(renderer);
            }
        }
    }
}

// Destructeur de la classe Game
Game::~Game() {
    // Nettoyage des ressources
    TextureLoader::deallocateTextures(); // Libère toutes les textures
}

// Traitement des événements du jeu
void Game::processEvents(SDL_Renderer* renderer, bool& running) {
    // Variables pour le suivi du clic de souris
    bool mouseDownThisFrame = false;

    // Traitement des événements SDL
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                // Arrête le jeu si l'utilisateur ferme la fenêtre
                running = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                // Gére les clics de souris
                mouseDownThisFrame = (mouseDownStatus == 0);
                if (event.button.button == SDL_BUTTON_LEFT)
                    mouseDownStatus = SDL_BUTTON_LEFT;
                else if (event.button.button == SDL_BUTTON_RIGHT)
                    mouseDownStatus = SDL_BUTTON_RIGHT;
                break;
            case SDL_MOUSEBUTTONUP:
                // Réinitialisons le statut du clic de souris
                mouseDownStatus = 0;
                break;
            case SDL_KEYDOWN:
                // Gére les pressions de touche
                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_ESCAPE:
                        // Quitte le jeu en appuyant sur la touche Echap
                        running = false;
                        break;
                    case SDL_SCANCODE_1:
                        // Change le mode de placement en mur
                        placementModeCurrent = PlacementMode::wall;
                        break;
                    case SDL_SCANCODE_2:
                        // Change le mode de placement en unités
                        placementModeCurrent = PlacementMode::units;
                        break;
                    case SDL_SCANCODE_H:
                        // Affiche/masque l'overlay en appuyant sur la touche H
                        overlayVisible = !overlayVisible;
                        break;
                }
        }
    }

    // Obtenir la position de la souris
    int mouseX = 0, mouseY = 0;
    SDL_GetMouseState(&mouseX, &mouseY);
    Vector2D posMouse((float)mouseX / tileSize, (float)mouseY / tileSize);
    level.setTargetAndCalculateFlowField((int)posMouse.x, (int)posMouse.y);

    // Gére l'action du clic de souris
    if (mouseDownStatus > 0) {
        switch (mouseDownStatus) {
            case SDL_BUTTON_LEFT:
                // Gére le clic gauche de la souris
                switch (placementModeCurrent) {
                    case PlacementMode::wall:
                        // Ajoute un mur à la position de la souris
                        level.setTileWall((int)posMouse.x, (int)posMouse.y, true);
                        break;
                    case PlacementMode::units:
                        // Ajoute une unité à la position de la souris
                        if (mouseDownThisFrame) //Si cette ligne est commenter on aura plusieur Enemy d'un coup
                            addUnit(renderer, posMouse);
                        break;
                }
                break;
            case SDL_BUTTON_RIGHT:
                // Gére le clic droit de la souris
                // Supprime un mur ou une unité à la position de la souris
                level.setTileWall((int)posMouse.x, (int)posMouse.y, false);
                removeUnitsAtMousePosition(posMouse);
                break;
        }
    }
}

// Mettre à jour l'état du jeu
void Game::update(float dT) {
    // Mettre à jour les unités
    for (Unit & unitSelected : listUnits)
        unitSelected.update(dT, level, listUnits);
}

// Dessine le contenu du jeu
void Game::draw(SDL_Renderer* renderer) {
    // Efface l'écran
    SDL_SetRenderDrawColor(renderer, 57, 51, 82, 0.37);
    SDL_RenderClear(renderer);

    // Dessine le niveau
    level.draw(renderer, tileSize);

    // Dessine les unités
    for (auto& unitSelected : listUnits)
        unitSelected.draw(renderer, tileSize);

    // Dessine l'overlay si visible
    if (textureOverlay != nullptr && overlayVisible) {
        int w = 0, h = 0;
        SDL_QueryTexture(textureOverlay, NULL, NULL, &w, &h);
        SDL_Rect rect = { 40, 40, w, h };
        SDL_RenderCopy(renderer, textureOverlay, NULL, &rect);
    }

    // Met à jour l'affichage
    SDL_RenderPresent(renderer);
}

// Ajoute une unités/Enemy à la position donnée
void Game::addUnit(SDL_Renderer* renderer, Vector2D posMouse) {
    listUnits.push_back(Unit(renderer, posMouse));
}

// Supprime les unités/Enemy à la position de la souris
void Game::removeUnitsAtMousePosition(Vector2D posMouse) {
    for (int i = 0; i < listUnits.size(); i++) {
        auto& unitSelected = listUnits[i];
        if (unitSelected.checkOverlap(posMouse, 0.0f)) {
            listUnits.erase(listUnits.begin() + i);
            i--;
        }
    }
}
