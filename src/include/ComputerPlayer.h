#pragma once

#include "Player.h"

/**@brief represents AI bots  */
class ComputerPlayer : public Player {
public:
    /**@brief sets initial player's position, loads textures * */
    ComputerPlayer(int start_pos_x, int start_pos_y, const std::string & name, int name_pos_x, int name_pos_y,
            const char * kitty_src, const char * dead_kitty_src);

    ~ComputerPlayer() override = default;
    /**@brief moves the player
     * @details this method decides every movement of the player
     * @param map[in] reference to the map
     * */
    void move (const SDL_Keycode &, Map & map) override;
protected:
    enum State { REGULAR, RANDOM };

    /**@brief plants bomb at the place the player is standing
     * @param reference to the map
     * */
    void plantBomb (Map & map) override;

    /**@brief checks whether the player would be safe from explosion at given tile
     * @param x[in] x coordinate of tile
     * @param y[in] y coordinate of tile
     * @param distance_to_nearest_bomb[out] returns distance to the nearest bomb
     * @returns bool if the player is safe, false otherwise
     * */
    bool safe(int x, int y, int & distance_to_nearest_bomb);

    /**@brief finds nearest tile where the player would be safe
     * @param start_pos[in] current players position
     * @returns valid coordinates of the nearest safe tile if any exists, {0,0} otherwise
     * */
    Vector2D findNearestSafeTile(const Vector2D & start_pos);

    /**@brief finds reachable tile, furthest away from all the bombs
     * @param start_pos[in] current players position
     * @returns tile with least probability to die on
     * */
    Vector2D findLeastDangerousTile(const Vector2D & start_pos);

    /**@brief finds safe path to some tile near boxes
     * @param start_pos[in] current players position
     * @returns valid tile to put a bomb on, {0,0} otherwise
     * */
    Vector2D findSafeToBombTile(const Vector2D & start_pos);

    /**@brief instructs player to hide away from explosion
     * */
    void hide();

    /**@brief gets path to the destination tile and fills int path_queue
     * @param dest_tile[in] valid destination tile
     * */
    void getPath(const Vector2D & dest_tile);

    /**@brief instructs player to actually move towards the next tile in path_queue
     * @param next_tile[in] next tile in the queue to move on
     * */
    void moveTowards(const Vector2D & next_tile);

    /**@brief moves player down
     * @param next_tile[in] to move on
     * */
    void moveDown(const Vector2D & next_tile);

    /**@brief moves player up
     * @param next_tile[in] to move on
     * */
    void moveUp(const Vector2D & next_tile);

    /**@brief moves player left
    * @param next_tile[in] to move on
    * */
    void moveLeft(const Vector2D & next_tile);

    /**@brief moves player right
     * @param next_tile[in] to move on
     * */
    void moveRight(const Vector2D & next_tile);

    Vector2D my_tile;

    /**@brief checks whether current position is almost equal to the other tile
     * @param other[in] other tile
     * @returns true if the tiles are almost equal, false otherwise
     * */
    bool posAlmostEqual(const Vector2D & other) const;

    /**@brief checks whether tile at given position is next to a box within bombable distance
     * @param x[in] x pos of the tile
     * @param y[in] y pos of the tile
     * @returns true if any box is within bombable distance, false otherwise
     * */
    bool nextToBox (int x, int y) const;

    /**@brief copies map into my_map to avoid editing of main map while searching it
     * @param reference to the main map
     * */
    void getCurrentMap (const Map & map);
    char my_map[ROWS_NBR][COLS_NBR];

    /**@brief checks whether there is bomb on the given position in my_tile map
     * @param col[in]
     * @param row[in]
     * @returns true if there is bomb, false otherwise
     * */
    bool isBomb (int col, int row) const { return my_map[row][col] == 'x'; }

    /**@brief checks whether the tile on the my_tile map is steppable
     * @param col[in] column to check
     * @param row[in] row to check
     * @returns true if player can step on the tile, false otherwise
     * */
    bool isSteppable (int col, int row) const { return my_map[row][col] < '#'; }

    /**@brief checks whether the given tile is a bombable box
     * @param col[in] column
     * @param row[in] row
     * @returns true if the tile is box, false otherwise
     * */
    bool isBox (int col, int row) const { return my_map[row][col] == '@'; }

    /**@brief checks whether the given tile was visited while searching
     * @param col[in] column
     * @param row[in] row
     * @returns true if the tile was not visited, false otherwise
     * */
    bool notVisited (int col, int row) const { return my_map[row][col] == 0; }

    /** @brief checks if there are any bombable boxes on the map left
     * @returns true if there are any, false otherwise*/
    bool checkBoxesLeft (const Map & map) const;

    std::deque<Vector2D> path;
    std::queue<Vector2D> places_to_bomb;

    State state = REGULAR;
};
