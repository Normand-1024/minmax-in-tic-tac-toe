#include <vector>
#include <iostream>
#include "Renderer.h"

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;

enum tictactoe {
	PLAY_SIZE = 4,

	//roles are distinguished by their enum value
	X = 1, EMPTY = 0, O = -1,

	//Return values in check_win_condition
	X_WIN = 1, DRAW = 0, X_LOSE = -1, NOT_OVER = -1001
};

enum display_texture { background_1 = 0, background_2 = 1, win_screen = 2, draw_screen = 3, lost_screen = 4, replay_screen = 5 };

enum role_texture { O_role = 0, X_role = 1, YES = 2 };

/*
	Play_field is a class that has a PLAY_SIZE * PLAY_SIZE 2D vector

	The constructor initializes all the entries in the array to EMPTY
*/
class Play_field {
public:
	Play_field();

	/*
		x and y are numbers indicating coordinates, starting from (1,1) at the bottom left

		valid_input checks if the move is either out of bound or not on an empty tile
	*/
	void put(int x, int y, int shape);
	int get(int x, int y) const;
	bool valid_input(int x, int y) const;

	//display the playfield using SDL 2.0 interface, renderer and vectors of textures
	void display(SDL_Renderer* ren, vector<SDL_Texture*>& display_vec, vector<SDL_Texture*>& role_vec) const;

	//assign all entries to EMPTY
	void clear();

	//check if the game is over, if so, return the role of winner
	int check_win_condition()  const;

private:
	vector<vector<int>> field;
};
