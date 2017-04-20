#include <iostream>
#include <algorithm>
#include "PlayField.h"

using namespace std;

enum utility { MAX_UTILITY = 1000, MIN_UTILITY = -1000 };
/*
	An action has a coordinate(second) and the v value(first) related to that action
*/
typedef pair<int, pair<int, int>> action;

/*
	Important global variables for status report
*/
int node_generate_count = 0, max_depth = 0, static_cutoff = 0, if_cutoff = 0, max_prune = 0, min_prune = 0;

struct node {
	node(Play_field& p_state, int p_alpha, int p_beta) 
		: game_state(p_state), alpha(p_alpha), beta(p_beta) {}

	Play_field game_state;
	int alpha, beta, v = 0;
};


pair<int, int> ALPHA_BETA_SEARCH(const Play_field& game_state,int shape, int cutoff);
int MAX_VALUE(Play_field& game_state, int alpha, int beta, int shape, int cutoff);
int MIN_VALUE(Play_field& game_state, int alpha, int beta, int shape, int cutoff);


void ai_move(Play_field& game_state, int shape, int game_progress, int difficulty) {
	pair<int, int> next_move = {-1,-1};
	/*
		The thought behind cutoff point:
		Easy: Only explore 1 levels
		Medium: Only explore 2 level
		Hard: More moves played, the more depths should be explored (game_progress = # of moves)

		static_cutoff is used to calculate the actual cutoff point, since cutoff will be changed as the search goes on
	*/
	if (difficulty == 1)	static_cutoff = 1;
	else if (difficulty == 2) static_cutoff = 3;
	else static_cutoff = game_progress;
	node_generate_count = 1;
	max_depth = 0, max_prune = 0, min_prune = 0;

	if (game_progress == 0) {
		// Randomly generate the next move to make it more interesting
		while (!game_state.valid_input(next_move.first, next_move.second)) next_move = pair<int, int>(rand() % PLAY_SIZE, rand() % PLAY_SIZE);
		cout << "\nA random move was chosen instead of using ALPHA_BETA_SEARCH\nNo cutoff occured\nOnly generated 1 node\nMax depth is 0\nPruning for both MIN and Max is 0\n";
	}
	else {
		next_move = ALPHA_BETA_SEARCH(game_state, shape, static_cutoff);
		cout << "\nStatus Report:\n";
		if (if_cutoff == 1) cout << "\n-Cutoff occured\n";
		else cout << "\n-No cutoff occured\n";
		cout << "-Total node count: " << node_generate_count << "\n-Max depth: " << max_depth << "\n-Pruning count for MAX_VALUE: " << max_prune << "\n-Pruning count for MIN_VALUE: " << min_prune << endl;
	}

	game_state.put(next_move.first, next_move.second, shape);
}


pair<int, int> ALPHA_BETA_SEARCH(const Play_field& game_state, int shape, int cutoff) {
	/*
		A vector storing move(in terms of pair) and their related v value
	*/
	vector<action*> action_array;
	pair<int, int> output;
	bool if_cutoff = false;
	int v = INT_MIN, alpha = MIN_UTILITY, beta = MAX_UTILITY, temp_v;

	for (int y = 0; y < PLAY_SIZE; ++y) {
		for (int x = 0; x < PLAY_SIZE; ++x) {
			if (game_state.get(x, y) == EMPTY) {
				/*
					Create a new state with action applied
				*/
				Play_field new_game_state(game_state);
				new_game_state.put(x, y, shape);
				/*
					recursively call MIN_VALUE and store the action and its v value
				*/
				node_generate_count++;
				temp_v = MIN_VALUE(new_game_state, alpha, beta, -shape, cutoff - 1);
				action_array.push_back(new action(temp_v, pair<int,int>(x, y)));

				/*
					updating v, alpha and prune if needed
				*/
				v = max(v, temp_v);
				if (v >= beta) {
					++max_prune;
					goto pruning;
				}
				alpha = max(alpha, v);
			}
		}
	}

pruning:
	/*
		Choose the best action to return
	*/
	bool copied = false;
	for (action* i : action_array) {
		if (!copied && i->first == v) {
			output = i->second;
			copied = true;
		}
		delete i;
	}
	return output;
}

/*
	Helper functions for EVAL
*/
void increment_counter(int& X_counter, int& O_counter, int role) {
	if (role == X)
		X_counter++;
	else
		O_counter++;
}

void increment_variable(int& X_1, int& X_2, int& X_3, int& O_1, int& O_2, int& O_3, int X_counter, int O_counter) {
	switch (X_counter) {
	case 0: {
		switch (O_counter) {
		case 1: {
			X_1++;
			break;
		}
		case 2: {
			X_2++;
			break;
		}
		case 3: {
			X_3++;
			break;
		}
		default: break;
		}
	}
			break;
	case 1: {
		X_1++;
		break;
	}
	case 2: {
		X_2++;
		break;
	}
	case 3: {
		X_3++;
		break;
	}
	}
}

/*
	EVAL function:  utility = 6X_3 + 3X_2 + X_1 - (6O_3 + 3 O_2 + O_1)

	where X represents AI's role, O represents player's role

	X_3 means number of rows, columns and diagonals that have and ONLY have 3 X-es, no O-s. etc. 
*/
int EVAL(const Play_field& game_state, int shape) {
	//Variables for calculating EVAL
	int X_1 = 0, X_2 = 0, X_3 = 0, O_1 = 0, O_2 = 0, O_3 = 0;
	//Variables for counting
	int X_counter = 0, O_counter = 0, role = 0, current_shape = 0;

	//Counting rows
	for (int y = 0; y < PLAY_SIZE; ++y) {
		for (int x = 0; x < PLAY_SIZE; ++x) {
			current_shape = game_state.get(x, y);

			if (current_shape != EMPTY) {
				if (role == 0)
					role = current_shape;

				if (current_shape == role)
					increment_counter(X_counter, O_counter, role);
				else {
					role = 0;
					break;
				}
			}
		}
		if (role != 0)
			increment_variable(X_1, X_2, X_3, O_1, O_2, O_3, X_counter, O_counter);
		role = X_counter = O_counter = 0;
	}

	//Counting columns
	for (int x = 0; x < PLAY_SIZE; ++x) {
		for (int y = 0; y < PLAY_SIZE; ++y) {
			current_shape = game_state.get(x, y);

			if (current_shape != EMPTY) {
				if (role == 0)
					role = current_shape;

				if (current_shape == role)
					increment_counter(X_counter, O_counter, role);
				else {
					role = 0;
					break;
				}
			}
		}
		if (role != 0)
			increment_variable(X_1, X_2, X_3, O_1, O_2, O_3, X_counter, O_counter);
		role = X_counter = O_counter = 0;
	}

	//Counting diagonal
	for (int y = 0, x = 0; y < PLAY_SIZE; ++y, ++x) {
			current_shape = game_state.get(x, y);

			if (current_shape != EMPTY) {
				if (role == 0)
					role = current_shape;

				if (current_shape == role)
					increment_counter(X_counter, O_counter, role);
				else {
					role = 0;
					break;
				}
			}
	}
	if (role != 0)
		increment_variable(X_1, X_2, X_3, O_1, O_2, O_3, X_counter, O_counter);
	role = X_counter = O_counter = 0;

	// Counting the other diagonal
	for (int y = PLAY_SIZE - 1, x = PLAY_SIZE - 1; y >= 0; --y, --x) {
		current_shape = game_state.get(x, y);

		if (current_shape != EMPTY) {
			if (role == 0)
				role = current_shape;

			if (current_shape == role)
				increment_counter(X_counter, O_counter, role);
			else {
				role = 0;
				break;
			}
		}
	}
	if (role != 0)
		increment_variable(X_1, X_2, X_3, O_1, O_2, O_3, X_counter, O_counter);

	int X_eval = 6 * X_3 + 3 * X_2 + X_1, O_eval = 6 * O_3 + 3 * O_2 + O_1;
	if (shape == X)
		return X_eval - O_eval;
	else
		return O_eval - X_eval;
}


int UTILITY(const Play_field& game_state, int shape, int cutoff) {
	if (cutoff > 0) {
		int win_condition = game_state.check_win_condition();
		if (win_condition == shape)
			return MAX_UTILITY;
		else if (win_condition == -shape)
			return MIN_UTILITY;
		else
			return win_condition;
	}
	else {
		/*
			Pass the game state to evaluate expected utility
		*/
		if_cutoff = 1;
		return EVAL(game_state, shape);
	}
}


int MAX_VALUE(Play_field& game_state, int alpha, int beta, int shape, int cutoff) {
	/*
		Update max_depth
	*/
	int current_depth = static_cutoff - cutoff;
	if (current_depth > max_depth) max_depth = current_depth;

	/*
		Utility node checking
	*/
	int utility = UTILITY(game_state, shape, cutoff);
	if (utility != NOT_OVER)
		return utility;

	int v = INT_MIN, temp_v;

	for (int y = 0; y < PLAY_SIZE; ++y) {
		for (int x = 0; x < PLAY_SIZE; ++x) {
			if (game_state.get(x, y) == EMPTY) {
				/*
					Create a new state with action applied
				*/
				Play_field new_game_state(game_state);
				new_game_state.put(x, y, shape);

				/*
					Recursively call MIN_VALUE
				*/
				node_generate_count++;
				temp_v = MIN_VALUE(new_game_state, alpha, beta, -shape, cutoff - 1);

				/*
					Updating v, alpha and prune if needed
				*/
				v = max(v, temp_v);
				if (v >= beta) {
					++max_prune;
					return v;
				}
				alpha = max(alpha, v);
			}
		}
	}
	return v;
}


int MIN_VALUE(Play_field& game_state, int alpha, int beta, int shape, int cutoff) {
	/*
		Update max_depth
	*/
	int current_depth = static_cutoff - cutoff;
	if (current_depth > max_depth) max_depth = current_depth;

	/*
		Utility node checking
	*/
	int utility = UTILITY(game_state, -shape, cutoff);
	if (utility != NOT_OVER)
		return utility;

	int v = INT_MAX, temp_v;

	for (int y = 0; y < PLAY_SIZE; ++y) {
		for (int x = 0; x < PLAY_SIZE; ++x) {
			if (game_state.get(x, y) == EMPTY) {
				/*
					Create a new state with action applied
				*/
				Play_field new_game_state(game_state);
				new_game_state.put(x, y, shape);
				/*
					Recursively call MAX_VALUE
				*/
				node_generate_count++;
				temp_v = MAX_VALUE(new_game_state, alpha, beta, -shape, cutoff - 1);

				/*
					Updating v, beta and prune if needed
				*/
				v = min(v, temp_v);
				if (v <= alpha) {
					++min_prune;
					return v;
				}
				beta = min(beta, v);
			}
		}
	}

	return v;
}