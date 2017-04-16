#include <vector>
#include <iostream>

using namespace std;

enum tictactoe {
	PLAY_SIZE = 4,

	//roles are distinguished by their enum value
	X = 1, EMPTY = 0, O = -1,

	//Return values in check_win_condition
	X_WIN = 1, DRAW = 0, X_LOSE = -1, NOT_OVER = -1001
};

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

	//display the playfield in ASCII, * for empty, X and O for player moves
	void display() const;

	//assign all entries to EMPTY
	void clear();

	//check if the game is over, if so, return the role of winner
	int check_win_condition()  const;

private:
	vector<vector<int>> field;
};
