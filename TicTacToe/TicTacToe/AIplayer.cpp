#include <iostream>
#include "PlayField.h"

using namespace std;

void ai_move(Play_field& game_state, int shape) {
	for (int i = 0; i < PLAY_SIZE; ++i) {
		for (int j = 0; j < PLAY_SIZE; ++j)
			if (game_state.get(i,j) == EMPTY) game_state.put(i,j,shape);
	}
}