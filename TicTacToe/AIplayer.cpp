#include <iostream>
#include "PlayField.h"

using namespace std;

void ai_move(Play_field& game_state, int shape) {
	for (int y = 0; y < PLAY_SIZE; ++y) {
		for (int x = 0; x < PLAY_SIZE; ++x)
			if (game_state.get(x, y) == EMPTY) { 
				game_state.put(x, y, shape);
				return;
			}
	}
}