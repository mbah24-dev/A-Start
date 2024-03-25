#include "TextureLoader.h"

// Initialisation de la map de textures chargées
std::unordered_map<std::string, SDL_Texture*> TextureLoader::umapTexturesLoaded;

// Charge une texture à partir d'un fichier
SDL_Texture* TextureLoader::loadTexture(SDL_Renderer* renderer, std::string filename) {
    if (filename != "") {
        // Je Vérifie si la texture est déjà chargée en cherchant la cle qui est le nom du fichier
        auto found = umapTexturesLoaded.find(filename);

        if (found != umapTexturesLoaded.end()) {
            // La texture est déjà chargée, donc je la retourne
            return found->second; //car first est la clé et second la texture , pareil en lifapf
        }
        else {
            // Définit le chemin relatif vers le dossier des images en utilisant le nom du fichier d'entrée
            std::string filepath = "../data/" + filename;

            // Essaie de créer une surface en utilisant le chemin du fichier
            //c_str() caster en char *
            SDL_Surface* surfaceTemp = SDL_LoadBMP(filepath.c_str());
            if (surfaceTemp != nullptr) {
                // La surface a été créée avec succès, alors j'essaie de créer une texture avec elle
                SDL_Texture* textureOutput = SDL_CreateTextureFromSurface(renderer, surfaceTemp);
                // Libère la surface car elle n'est plus nécessaire
                SDL_FreeSurface(surfaceTemp);

                if (textureOutput != nullptr) {
                    // Active la transparence pour la texture
                    SDL_SetTextureBlendMode(textureOutput, SDL_BLENDMODE_BLEND);

                    // Ajoute la texture à la map des textures chargées pour la gestion et le nettoyage
                    umapTexturesLoaded[filename] = textureOutput;

                    return textureOutput;
                }
            }
        }
    }

    return nullptr; // Retourne nullptr si la texture ne peut pas être chargée
}

// Libère toutes les textures chargées
void TextureLoader::deallocateTextures() {
    // Détruit toutes les textures dans la map
    while (!umapTexturesLoaded.empty()) {
        auto it = umapTexturesLoaded.begin();
        if (it->second != nullptr)
            SDL_DestroyTexture(it->second); // Détruit la texture

        umapTexturesLoaded.erase(it); // Supprime la texture de la map
    }
}
