#include <cstddef>
#include <iostream>
#include <fstream>
#include <string>
#include "logic.h"

using std::cout, std::endl, std::ifstream, std::string;

/**
 * TODO: Student implement this function
 * Load representation of the dungeon level from file into the 2D map.
 * Calls createMap to allocate the 2D array.
 * @param   fileName    File name of dungeon level.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference to set starting position.
 * @return  pointer to 2D dynamic array representation of dungeon map with player's location., or nullptr if loading fails for any reason
 * @updates  maxRow, maxCol, player
 */
char** loadLevel(const string& fileName, int& maxRow, int& maxCol, Player& player) {
    ifstream levelFile;
    levelFile.open(fileName);
    if (!levelFile){
        return nullptr;
    }
    levelFile >> maxRow;
    if (levelFile.fail() || maxRow <1){
        return nullptr;
    }
    levelFile >> maxCol;
    if (levelFile.fail() || maxCol < 1){
        return nullptr;
    }
    if (maxRow > INT32_MAX/maxCol){
        return nullptr;
    }
    else if (maxCol > INT32_MAX/maxRow){
        return nullptr;
    }


    levelFile >> player.row;
    if (levelFile.fail() || player.row < 0 || player.row >= maxRow){
        return nullptr;
    }
    levelFile >> player.col;
    if (levelFile.fail() || player.col < 0 || player.col >= maxCol){
        return nullptr;
    }

    
    char **dungMap = new char*[maxRow];
    for (int y = 0; y < maxRow; ++y){
        dungMap[y] = new char[maxCol];
    }

    bool isExit = false;
    for (int y = 0; y < maxRow; ++y){
        for (int x = 0; x < maxCol; ++x){
            
            levelFile >> dungMap[y][x];
           
            if (dungMap[y][x] == TILE_DOOR || dungMap[y][x] == TILE_EXIT){
                isExit = true;
            }
            if (levelFile.fail()){
                return nullptr;
            }
        }
    }
    dungMap[player.row][player.col] = TILE_PLAYER; 
    if (!isExit){
        return nullptr;
    }
    char test;
    if (levelFile >> test){
        return nullptr;
    }
    return dungMap;
}

/**
 * TODO: Student implement this function
 * Translate the character direction input by the user into row or column change.
 * That is, updates the nextRow or nextCol according to the player's movement direction.
 * @param   input       Character input by the user which translates to a direction.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @updates  nextRow, nextCol
 */
void getDirection(char input, int& nextRow, int& nextCol) {
    if (input == 'w'){
        nextRow -= 1; 
    }
    else if (input == 's'){
        nextRow += 1;
    }
    else if (input == 'a'){
        nextCol -= 1;
    }
    else if (input == 'd'){
        nextCol += 1;
    }
    
}

/**
 * TODO: [suggested] Student implement this function
 * Allocate the 2D map array.
 * Initialize each cell to TILE_OPEN.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @return  2D map array for the dungeon level, holds char type.
 */
char** createMap(int maxRow, int maxCol) {
    if (maxRow <= 0 || maxCol <= 0){
        return nullptr;
    }
    char **newArr = new char*[maxRow];
    for (int i =0; i < maxRow; ++i){
        newArr[i] = new char[maxCol];
    }

    for (int y =0; y < maxRow; ++y){
        for (int x =0; x < maxCol; ++x){
            newArr[y][x] = TILE_OPEN;
        }
    }
    return newArr;
}

/**
 * TODO: Student implement this function
 * Deallocates the 2D map array.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @return None
 * @update map, maxRow
 */
void deleteMap(char**& map, int& maxRow) {
    if (map != nullptr){
        for (int i =0; i < maxRow; ++i){
            delete[] map[i];
        }
    }
    else {
        maxRow =0;
    }
    delete[] map;
    map = nullptr;
    maxRow =0;
}

/**
 * TODO: Student implement this function
 * Resize the 2D map by doubling both dimensions.
 * Copy the current map contents to the right, diagonal down, and below.
 * Do not duplicate the player, and remember to avoid memory leaks!
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height), to be doubled.
 * @param   maxCol      Number of columns in the dungeon table (aka width), to be doubled.
 * @return  pointer to a dynamically-allocated 2D array (map) that has twice as many columns and rows in size.
 * @update maxRow, maxCol
 */
char** resizeMap(char** map, int& maxRow, int& maxCol) {
    if (map == nullptr || maxRow < 1 || maxCol < 1){
        return nullptr;
    }
    int origRow = maxRow;
    int origCol = maxCol;
    if (maxRow > INT32_MAX/2){
        return nullptr;
    }
    else if (maxCol > INT32_MAX/2){
        return nullptr;
    }
    maxRow *= 2;
    maxCol *= 2;

    if (maxRow > INT32_MAX/maxCol){
        return nullptr;
    }
    else if (maxCol > INT32_MAX/maxRow){
        return nullptr;
    }

    char **bigMap = new char *[maxRow];
    for (int i =0; i < maxRow; ++i){
        bigMap[i] = new char[maxCol];
    }
    for (int row = 0; row < maxRow; ++row){
        for (int col =0; col < maxCol; ++col){
            if (row >= origRow && col >= origCol){
                if (map[row-origRow][col-origCol] == TILE_PLAYER){
                    bigMap[row][col] = TILE_OPEN;
                }
                else {
                    bigMap[row][col] = map[row-origRow][col-origCol];
                }
                
            }
            else if (row >= origRow){
                if (map[row-origRow][col] == TILE_PLAYER){
                    bigMap[row][col] = TILE_OPEN;
                }
                else {
                    bigMap[row][col] = map[row-origRow][col];
                }
                
            }
            else if (col >= origCol){
                if (map[row][col-origCol] == TILE_PLAYER){
                    bigMap[row][col] = TILE_OPEN;
                }
                else {
                    bigMap[row][col] = map[row][col-origCol];
                }
                
            }
            else{
                bigMap[row][col] = map[row][col];
            }
        }
    }

    deleteMap(map, origRow);


    return bigMap;
}

/**
 * TODO: Student implement this function
 * Checks if the player can move in the specified direction and performs the move if so.
 * Cannot move out of bounds or onto TILE_PILLAR or TILE_MONSTER.
 * Cannot move onto TILE_EXIT without at least one treasure. 
 * If TILE_TREASURE, increment treasure by 1.
 * Remember to update the map tile that the player moves onto and return the appropriate status.
 * You can use the STATUS constants defined in logic.h to help!
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object to by reference to see current location.
 * @param   nextRow     Player's next row on the dungeon map (up/down).
 * @param   nextCol     Player's next column on dungeon map (left/right).
 * @return  Player's movement status after updating player's position.
 * @update map contents, player
 */
int doPlayerMove(char** map, int maxRow, int maxCol, Player& player, int nextRow, int nextCol) {
    if (nextRow < 0 || nextRow >= maxRow || nextCol < 0 || nextCol >= maxCol || map[nextRow][nextCol] == TILE_PILLAR || map[nextRow][nextCol] == TILE_MONSTER){
        nextRow = player.row;
        nextCol = player.col;
        return STATUS_STAY;
    }
    else if (map[nextRow][nextCol] == TILE_TREASURE){
        map[player.row][player.col] = TILE_OPEN;
        player.row = nextRow;
        player.col = nextCol;
        map[player.row][player.col] = TILE_PLAYER;
        player.treasure += 1;
        return STATUS_TREASURE;
    }
    else if (map[nextRow][nextCol] == TILE_AMULET){
        map[player.row][player.col] = TILE_OPEN;
        player.row = nextRow;
        player.col = nextCol;
        map[player.row][player.col] = TILE_PLAYER;
        return STATUS_AMULET;
    }
    else if (map[nextRow][nextCol] == TILE_DOOR){
        map[player.row][player.col] = TILE_OPEN;
        player.row = nextRow;
        player.col = nextCol;
        map[player.row][player.col] = TILE_PLAYER;
        return STATUS_LEAVE;
    }
    else if (map[nextRow][nextCol] == TILE_EXIT){
        if (player.treasure > 0){
            map[player.row][player.col] = TILE_OPEN;
            player.row = nextRow;
            player.col = nextCol;
            map[player.row][player.col] = TILE_PLAYER;
            return STATUS_ESCAPE;
        }
        nextRow = player.row;
        nextCol = player.col;
        return STATUS_STAY;
    }
    else {
        map[player.row][player.col] = TILE_OPEN;
        player.row = nextRow;
        player.col = nextCol;
        map[player.row][player.col] = TILE_PLAYER;
        return STATUS_MOVE;
    }
    return 0;
}

/**
 * TODO: Student implement this function
 * Update monster locations:
 * We check up, down, left, right from the current player position.
 * If we see an obstacle, there is no line of sight in that direction, and the monster does not move.
 * If we see a monster before an obstacle, the monster moves one tile toward the player.
 * We should update the map as the monster moves.
 * At the end, we check if a monster has moved onto the player's tile.
 * @param   map         Dungeon map.
 * @param   maxRow      Number of rows in the dungeon table (aka height).
 * @param   maxCol      Number of columns in the dungeon table (aka width).
 * @param   player      Player object by reference for current location.
 * @return  Boolean value indicating player status: true if monster reaches the player, false if not.
 * @update map contents
 */
bool doMonsterAttack(char** map, int maxRow, int maxCol, const Player& player) {
    int row = player.row;
    
    while (row != 0){
        if (map[row -1][player.col] == TILE_MONSTER){
            map[row -1][player.col] = TILE_OPEN;
            map[row][player.col] = TILE_MONSTER;
        }
        else if (map[row -1][player.col] == TILE_PILLAR){
            break;
        }
        row -= 1;
    }
    row = player.row;
    while (row != maxRow -1){
        if (map[row +1][player.col] == TILE_MONSTER){
            map[row +1][player.col] = TILE_OPEN;
            map[row][player.col] = TILE_MONSTER;
        }
        else if (map[row +1][player.col] == TILE_PILLAR){
            break;
        }
        row += 1;
    }

    int column = player.col;

    while(column != 0){
        if (map[player.row][column-1] == TILE_MONSTER){
            map[player.row][column-1] = TILE_OPEN;
            map[player.row][column] = TILE_MONSTER;
        }
        else if (map[player.row][column-1] == TILE_PILLAR){
            break;
        }
        
        column -= 1;
    }
    column = player.col;
    while(column != maxCol -1){
        if (map[player.row][column+1] == TILE_MONSTER){
            map[player.row][column+1] = TILE_OPEN;
            map[player.row][column] = TILE_MONSTER;
        }
        else if (map[player.row][column+1] == TILE_PILLAR){
            break;
        }
        column += 1;
    }

    if (map[player.row][player.col] == TILE_MONSTER){
        return true;
    }
    return false;
}
