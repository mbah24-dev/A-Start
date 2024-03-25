#include "Unit.h"
//#include "Game.h"

// Définition des constantes de vitesse et de taille pour les instances de la classe Unit
const float Unit::speed = 5.0f; // La vitesse de déplacement de l'unité
const float Unit::size = 0.4f; // La taille de l'unité

// Constructeur de la classe Unit
Unit::Unit(SDL_Renderer* renderer, Vector2D setPos) :
        pos(setPos), posDraw(pos) {
    // Initialisation de la position et de la position de dessin de l'unité

    // Chargement de la texture de l'unité
    texture = TextureLoader::loadTexture(renderer, "Unit.bmp");
}

// Fonction de mise à jour de l'unité
void Unit::update(float dT, Level& level, std::vector<Unit>& listUnits) {
    // Calcul de la distance à la cible depuis la position actuelle de l'unité
    float distanceToTarget = (level.getTargetPos() - pos).magnitude(); //Un type vecteur est retourner par la difference , magnitude() pour l'heristic

    // Calcul de la distance à déplacer pendant cette frame
    //En physique d = v * t haha
    float distanceMove = speed * dT;
    if (distanceMove > distanceToTarget) //Yes destination atteinte
        distanceMove = distanceToTarget;

    // Calcul de la direction de déplacement combinant la direction du champ de flux et de la séparation normale
    Vector2D directionNormalFlowField(level.getFlowNormal((int)pos.x, (int)pos.y));
    Vector2D directionNormalSeparation(computeNormalSeparation(listUnits));
    Vector2D directionNormalCombined = directionNormalFlowField + directionNormalSeparation * 5.0f;
    directionNormalCombined.normalize();
    Vector2D posAdd = directionNormalCombined * distanceMove;

    // Vérification des collisions avec les autres unités
    bool moveOk = true;
    for (int i = 0; i < listUnits.size() && moveOk; i++) {
        auto& unitSelected = listUnits[i];
        if (&unitSelected != this && unitSelected.checkOverlap(pos, size)) {
            // Vérification de la direction du mouvement par rapport à l'autre unité
            Vector2D directionToOther = (unitSelected.pos - pos);
            if (directionToOther.magnitude() > 0.01f) {
                Vector2D normalToOther(directionToOther.normalize());
                float angleBtw = abs(normalToOther.angleBetween(directionNormalCombined));
                if (angleBtw < 3.14159265359f / 4.0f)
                    moveOk = false;

            }
        }
    }

    // Mise à jour de la position si aucun obstacle rencontré
    if (moveOk) {
        const float spacing = 0.35f;
        int x = (int)(pos.x + posAdd.x + copysign(spacing, posAdd.x));
        int y = (int)(pos.y);
        if (posAdd.x != 0.0f && level.isTileWall(x, y) == false)
            pos.x += posAdd.x;

        x = (int)(pos.x);
        y = (int)(pos.y + posAdd.y + copysign(spacing, posAdd.y));
        if (posAdd.y != 0.0f && level.isTileWall(x, y) == false)
            pos.y += posAdd.y;
    }

    // Calcul de la position de dessin en fonction de la position actuelle
    const float fKeep = 0.93f;
    posDraw = posDraw * fKeep + pos * (1.0f - fKeep);
}

// Fonction de dessin de l'unité
void Unit::draw(SDL_Renderer* renderer, int tileSize) {
    if (renderer != nullptr) {
        // Dessin de l'unité à sa position de dessin
        SDL_Rect rect = {
                (int)((posDraw.x - size / 2) * tileSize),
                (int)((posDraw.y - size / 2) * tileSize),
                (int)(size * tileSize),
                (int)(size * tileSize) };
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }
}

// Fonction de vérification de chevauchement avec une autre position et taille
bool Unit::checkOverlap(Vector2D posOther, float sizeOther) {
    return (posOther - pos).magnitude() <= (sizeOther + size) / 2.0f;
}

// Fonction de calcul de la séparation normale par rapport aux autres unités
Vector2D Unit::computeNormalSeparation(std::vector<Unit>& listUnits) {
    Vector2D output;

    //Le rayon comme la taille du sprite
    const float radiusSeparation = size;
    for (auto& unitSelected : listUnits) {
        if (&unitSelected != this) {
            // Vérification de la proximité avec l'autre unité
            Vector2D directionToOther = (unitSelected.pos - pos);
            float distanceToOther = directionToOther.magnitude();
            if (distanceToOther <= radiusSeparation) {
                if (directionToOther.magnitude() > 0.01f) {
                    // Ajout de la direction normale à la sortie
                    Vector2D normalToThis(directionToOther.normalize() * -1.0f);
                    output += normalToThis;
                }
            }
        }
    }
    output.normalize();

    return output;
}
