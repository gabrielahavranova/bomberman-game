#include "./include/FileManager.h"


int FileManager::loadMap(const int &map_id, Map & map) {
    char char_buff[20];
    std::string str_buff;

    std::string str_map_id = std::to_string(map_id);
    std::ifstream ifs ("src/maps/map" + str_map_id, std::ios::in);

    if (!ifs.good()) return 1;

    for (int i = 0; i < 16; i++) map.map[0][i] = '#'; //first line of #'s

    for (int row = 1; row < 11; row++) { //rows 1 to 10
        ifs.getline(char_buff, 20, '\n');

        if (ifs.gcount() != 15) {
            ifs.close();
            return 4;
        }

        map.map[row][0] = '#';
        for (int col = 1; col < 15; col++) {
             if (isValidChar(char_buff[col - 1])) map.map[row][col] = char_buff[col - 1];
             else return 5;
        }
        map.map[row][15] = '#';
    }

    for (int i = 0; i < 16; i++) map.map[11][i] = '#'; //last line of #'s

    if (map.map[1][1] != '.' || map.map[1][COLS_NBR - 2] != '.'
        || map.map[ROWS_NBR - 2][1] != '.' || map.map[ROWS_NBR -2][COLS_NBR - 2] != '.') return 6;

    ifs.close();
    return 0;
}

bool FileManager::saveHighScore (int score) {
    std::ofstream ofs("src/saves/highscore.txt", std::ios::out);
    if (!ofs.good()) std::cout << "unable to save highscore file" << std::endl;
    ofs << score;

    if (ofs.good()) {
        std::cout << " score successfully saved" << std::endl;
        ofs.close();
        return true;
    }
    else {
        std::cout << "unable to save highscore file" << std::endl;
        ofs.close();
        return false;
    }
}

int FileManager::loadHighScore() {
    std::string score_str;
    int score_int;

    std::ifstream ifs("src/saves/highscore.txt", std::ios::in);
    if (!ifs.good()) {
        std::cout << "unable to open highscore file!" << std::endl;
        return 0;
    }
    ifs >> score_str;
    ifs.close();

    if (score_str.empty())  return 0;

    try { score_int = std::stoi(score_str); }
    catch (std::invalid_argument &) {
        std::cout << "invalid highscore file!" << std::endl;
        return 0;
    }
    catch (std::out_of_range &) {
        std::cout << "invalid highscore! You've tried to cheat, didn't you? ;)" << std::endl;
        return 0;
    }

    if (score_int < 0) {
        std::cout << "invalid highscore!" << std::endl;
        return 0;
    }

    return score_int;
}

bool FileManager::isValidChar(char c) {
    return c == '#' || c == '@' || c == '.';
}
