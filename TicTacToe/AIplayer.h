#include "PlayField.h"

/*
	ai_move simply determines what action is best for the ai and then tell game_state where to put its move

	game_progress is number of moves on the play field, it is relevant to how many depths are explored in hard mode

	difficulty determines the depth AI will explore
*/
void ai_move(Play_field& game_state,const int shape, int game_progress, int difficulty);