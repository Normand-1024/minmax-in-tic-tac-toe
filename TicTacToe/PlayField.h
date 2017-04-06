#include <vector>
#include <iostream>

using namespace std;

#define PLAY_SIZE 4
#define X 1
#define EMPTY 0
#define O -1
#define X_WIN 1
#define DRAW 0
#define X_LOSE -1
#define NOT_OVER -2

class Play_field {
public:
	Play_field();
	void put(int x, int y, int shape);
	int get(int x, int y) const;

	void display() const;

	void clear();

	int check_win_condition()  const;

private:
	vector<vector<int>> field;
};
