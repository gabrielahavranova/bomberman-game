#pragma once

#include "Map.h"
#include <fstream>
#include <iostream>
#include <string>

class Map;

/** @brief FileManager is "static" class which manages loading/saving from/to files */
class FileManager {
public:
    /** @brief loads and validates map
     *
     * @param[in] map_id determines ID of the map which matches with file map[map_id]
     * @param[in] map - reference to a map to fill in loaded data
     *
     * @return 0 on success, 1 if ifstream was not opened, 4 if there are too many/too
     * little chars in the line, 5 if there are any non-valid chars, 6 if any of the
     * inital player position is not steppable
     * */
    static int loadMap (const int & map_id, Map & map);

    /** @brief loads high score from a file
     * @returns int loaded score on success, 0 if any error has occurred */
    static int loadHighScore();

    /**@brief checks whether given char c is valid game map char
     * @returns true if is valid, false otherwise*/
    static bool isValidChar (char c);

    /**@brief saves high score in the highscore file
     * @param[in] score to save
     * @returns bool on success, false if something failed while saving
     * */
    static bool saveHighScore (int score);
};

