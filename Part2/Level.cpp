#include "Level.h"



Level::Level(SDL_Renderer* renderer, int setTileCountX, int setTileCountY) :
    tileCountX(setTileCountX), tileCountY(setTileCountY) {
    textureTileWall = TextureLoader::loadTexture(renderer, "Tile Wall.bmp");
    textureTileTarget = TextureLoader::loadTexture(renderer, "Tile Target.bmp");

    textureTileEmpty = TextureLoader::loadTexture(renderer, "Tile Empty.bmp");
    textureTileArrowUp = TextureLoader::loadTexture(renderer, "Tile Arrow Up.bmp");
    textureTileArrowUpRight = TextureLoader::loadTexture(renderer, "Tile Arrow Up Right.bmp");
    textureTileArrowRight = TextureLoader::loadTexture(renderer, "Tile Arrow Right.bmp");
    textureTileArrowDownRight = TextureLoader::loadTexture(renderer, "Tile Arrow Down Right.bmp");
    textureTileArrowDown = TextureLoader::loadTexture(renderer, "Tile Arrow Down.bmp");
    textureTileArrowDownLeft = TextureLoader::loadTexture(renderer, "Tile Arrow Down Left.bmp");
    textureTileArrowLeft = TextureLoader::loadTexture(renderer, "Tile Arrow Left.bmp");
    textureTileArrowUpLeft = TextureLoader::loadTexture(renderer, "Tile Arrow Up Left.bmp");

    /**
     * size_t est un type de données utilisé pour représenter la taille ou
     * l'indice d'un tableau ou d'une structure de données.
     * Il est généralement défini comme un alias de type non signé
     */

    size_t listTilesSize = (size_t)tileCountX * tileCountY; //Nombre de tiles = largeur * hauteur car tableau 2D
    listTiles.assign(listTilesSize, Tile{});
}



void Level::draw(SDL_Renderer* renderer, int tileSize) {
    //Draw the arrow (and empty) tiles.
    for (int i = 0; i < listTiles.size(); i++)
        drawTile(renderer, (i % tileCountX), (i / tileCountY), tileSize);

    //Draw the target tile.
    if (textureTileTarget != nullptr) {
        SDL_Rect rect = { targetX * tileSize, targetY * tileSize, tileSize, tileSize };
        SDL_RenderCopy(renderer, textureTileTarget, NULL, &rect);
    }
    
    //Draw the wall tiles.
    for (int y = 0; y < tileCountY; y++) {
        for (int x = 0; x < tileCountX; x++) {
            if (isTileWall(x, y)) {
                SDL_Rect rect = { x * tileSize, y * tileSize, tileSize, tileSize };
                SDL_RenderCopy(renderer, textureTileWall, NULL, &rect);
            }
        }
    }
}



void Level::drawTile(SDL_Renderer* renderer, int x, int y, int tileSize) {
    //Set the default texture image to be empty.
    SDL_Texture* textureSelected = textureTileEmpty;

    //Ensure that the input tile exists.
    int index = x + y * tileCountX; //La formule passe de 2D a 1D
    if (index > -1 && index < listTiles.size() &&
        x > -1 && x < tileCountX &&
        y > -1 && y < tileCountY) {
        Tile& tileSelected = listTiles[index];

        //Select the correct tile texture based on the flow direction.
        if (tileSelected.flowDirectionX == 0 && tileSelected.flowDirectionY == -1) // {0,-1} goUp
            textureSelected = textureTileArrowUp;
        else if (tileSelected.flowDirectionX == 1 && tileSelected.flowDirectionY == -1)
            textureSelected = textureTileArrowUpRight;
        else if (tileSelected.flowDirectionX == 1 && tileSelected.flowDirectionY == 0) // {1,-1} goRight
            textureSelected = textureTileArrowRight;
        else if (tileSelected.flowDirectionX == 1 && tileSelected.flowDirectionY == 1) // {1, 1} goDownObliq
            textureSelected = textureTileArrowDownRight;
        else if (tileSelected.flowDirectionX == 0 && tileSelected.flowDirectionY == 1)
            textureSelected = textureTileArrowDown;
        else if (tileSelected.flowDirectionX == -1 && tileSelected.flowDirectionY == 1)
            textureSelected = textureTileArrowDownLeft;
        else if (tileSelected.flowDirectionX == -1 && tileSelected.flowDirectionY == 0)
            textureSelected = textureTileArrowLeft;
        else if (tileSelected.flowDirectionX == -1 && tileSelected.flowDirectionY == -1)
            textureSelected = textureTileArrowUpLeft;

    }

    //Draw the tile.
    if (textureSelected != nullptr) {
        SDL_Rect rect = { x * tileSize, y * tileSize, tileSize, tileSize };
        SDL_RenderCopy(renderer, textureSelected, NULL, &rect);
    }
}



bool Level::isTileWall(int x, int y) {
    //Je teste avant tout que les coordonnées son valides
    int index = x + y * tileCountX; //2D to 1D
    if (index > -1 && index < listTiles.size() && 
        x > -1 && x < tileCountX &&
        y > -1 && y < tileCountY)
        return listTiles[index].isWall;

    return true;// false;
}


void Level::setTileWall(int x, int y, bool setWall) {
    int index = x + y * tileCountX;
    if (index > -1 && index < listTiles.size() &&
        x > -1 && x < tileCountX &&
        y > -1 && y < tileCountY)
        listTiles[index].isWall = setWall;
}



Vector2D Level::getTargetPos() {
    return Vector2D((float)targetX, (float)targetY);
}


void Level::setTargetAndCalculateFlowField(int targetXNew, int targetYNew) {
    //Check if the target is new.
    if (targetX != targetXNew || targetY != targetYNew) {
        targetX = targetXNew;
        targetY = targetYNew;

        //Ensure the target is in bounds. est dans la fenetre
        int indexTarget = targetX + targetY * tileCountX;
        if (indexTarget > -1 && indexTarget < listTiles.size() &&
            targetX > -1 && targetX < tileCountX &&
            targetY > -1 && targetY < tileCountY) {

            //Reset the tile flow data.
            for (auto& tileSelected : listTiles) {
                tileSelected.flowDirectionX = 0; //{0,0} pas de direction
                tileSelected.flowDirectionY = 0;
                tileSelected.flowDistance = flowDistanceMax;
            }

            //Calculate the flow field.
            calculateDistances();
            calculateFlowDirections();
        }
    }
}


void Level::calculateDistances() {
    int indexTarget = targetX + targetY * tileCountX;

    //Create a queue that will contain the indices to be checked.
    std::queue<int> listIndicesToCheck;
    //Set the target tile's flow value to 0 and add it to the queue.
    listTiles[indexTarget].flowDistance = 0;
    listIndicesToCheck.push(indexTarget);

    //The offset of the neighboring tiles to be checked.
    const int listNeighbors[][2] = { { -1, 0}, {1, 0}, {0, -1}, {0, 1} };

    //Loop through the queue and assign distance to each tile.
    while (!listIndicesToCheck.empty()) {
        int indexCurrent = listIndicesToCheck.front();
        listIndicesToCheck.pop();

        //Check each of the neighbors;
        for (int count = 0; count < 4; count++) {
            int neighborX = listNeighbors[count][0] + indexCurrent % tileCountX;
            int neighborY = listNeighbors[count][1] + indexCurrent / tileCountX;
            int indexNeighbor = neighborX + neighborY * tileCountX;

            //Ensure that the neighbor exists and isn't a wall.
            if (indexNeighbor > -1 && indexNeighbor < listTiles.size() && 
                neighborX > -1 && neighborX < tileCountX &&
                neighborY > -1 && neighborY < tileCountY &&
                !listTiles[indexNeighbor].isWall) {

                //Check if the tile has been assigned a distance yet or not.
                if (listTiles[indexNeighbor].flowDistance == flowDistanceMax) {
                    //If not the set it's distance and add it to the queue.
                    listTiles[indexNeighbor].flowDistance = listTiles[indexCurrent].flowDistance + 1;
                    listIndicesToCheck.push(indexNeighbor);
                }
            }
        }
    }
}


void Level::calculateFlowDirections() {
    //The offset of the neighboring tiles to be checked.
    const int listNeighbors[][2] = {
        {-1, 0}, {-1, 1}, {0, 1}, {1, 1},
        {1, 0}, {1, -1}, {0, -1}, {-1, -1} };

    for (int indexCurrent = 0; indexCurrent < listTiles.size(); indexCurrent++) {
        //Ensure that the tile has been assigned a distance value.
        if (listTiles[indexCurrent].flowDistance != flowDistanceMax) {
            //Set the best distance to the current tile's distance.
            unsigned char flowFieldBest = listTiles[indexCurrent].flowDistance;

            //Check each of the neighbors;
            for (int count = 0; count < 8; count++) {
                int offsetX = listNeighbors[count][0];
                int offsetY = listNeighbors[count][1];

                int neighborX = offsetX + indexCurrent % tileCountX;
                int neighborY = offsetY + indexCurrent / tileCountX;
                int indexNeighbor = neighborX + neighborY * tileCountX;

                //Ensure that the neighbor exists.
                if (indexNeighbor > -1 && indexNeighbor < listTiles.size() &&
                    neighborX > -1 && neighborX < tileCountX &&
                    neighborY > -1 && neighborY < tileCountY) {
                    //If the current neighbor's distance is lower than the best then use it.
                    if (listTiles[indexNeighbor].flowDistance < flowFieldBest) {
                        flowFieldBest = listTiles[indexNeighbor].flowDistance;
                        listTiles[indexCurrent].flowDirectionX = offsetX;
                        listTiles[indexCurrent].flowDirectionY = offsetY;
                    }
                }
            }
        }
    }
}



Vector2D Level::getFlowNormal(int x, int y) {
    int index = x + y * tileCountX;
    if (index > -1 && index < listTiles.size() &&
        x > -1 && x < tileCountX &&
        y > -1 && y < tileCountY)
        return Vector2D((float)listTiles[index].flowDirectionX, (float)listTiles[index].flowDirectionY).normalize();

    return Vector2D();
}