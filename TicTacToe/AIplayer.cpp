#include <iostream>
#include <algorithm>
#include "PlayField.h"

using namespace std;

#define MAX_UTILITY 1000
#define MIN_UTILITY -1000
/*
	An action has a coordinate and the v value related to that action
*/
typedef pair<int, pair<int, int>> action;

int node_generate_count = 0;

struct node {
	node(Play_field& p_state, int p_alpha, int p_beta) 
		: game_state(p_state), alpha(p_alpha), beta(p_beta) {}

	Play_field game_state;
	int alpha, beta, v = 0;
};


pair<int, int> ALPHA_BETA_SEARCH(const Play_field& game_state,const int shape);
int MAX_VALUE(Play_field& game_state, int alpha, int beta, const int shape);
int MIN_VALUE(Play_field& game_state, int alpha, int beta, const int shape);


void ai_move(Play_field& game_state, const int shape) {
	pair<int, int> next_move;

	node_generate_count++;

	next_move = ALPHA_BETA_SEARCH(game_state, shape);
	cout << "next move: " << next_move.first << ", " << next_move.second << "\n";

	game_state.put(next_move.first, next_move.second, shape);
}


pair<int, int> ALPHA_BETA_SEARCH(const Play_field& game_state, const int shape) {
	/*
		A vector storing move(in terms of pair) and their related v value
	*/
	vector<action*> action_array;
	pair<int, int> output;
	int v = INT_MIN, alpha = MIN_UTILITY, beta = MAX_UTILITY, temp_v;

	for (int y = 0; y < PLAY_SIZE; ++y) {
		for (int x = 0; x < PLAY_SIZE; ++x) {
			if (game_state.get(x, y) == EMPTY) {
				/*
					Create a new state with action applied
				*/
				Play_field new_game_state = game_state;
				new_game_state.put(x, y, shape);
				/*
					recursively call MIN_VALUE and store the action and its v value
				*/
				node_generate_count++;
				temp_v = MIN_VALUE(new_game_state, MAX_UTILITY, MIN_UTILITY, -shape);
				action_array.push_back(new action(temp_v, pair<int,int>(x, y)));

				/*
					updating v, alpha and prune if needed
				*/
				v = max(v, temp_v);
				if (v >= beta)
					goto pruning;
				alpha = max(alpha, v);
			}
		}
	}

pruning:
	/*
		Choose the best action to return
	*/
	cout << "Total node count: " << node_generate_count << " v is " << v << "\n";
	for (action* i : action_array) {
		cout << i->first << ": " << i->second.first << ", " << i->second.second << "\n";
		if (i->first == v) {
			cout << "Being copied: " << i->second.first << ", " << i->second.second << "\n";
			output = i->second;
		}
		delete i;
	}

	return output;
}


int UTILITY(const Play_field& game_state, int shape) {
	int win_condition = game_state.check_win_condition();
	if (win_condition == shape)
		return MAX_UTILITY;
	else if (win_condition == -shape)
		return MIN_UTILITY;
	else
		return win_condition;
}


int MAX_VALUE(Play_field& game_state, int alpha, int beta, const int shape) {
	/*
		Utility node checking
	*/
	int utility = UTILITY(game_state, shape);
	if (utility != NOT_OVER)
		return utility;

	int v = INT_MIN, temp_v;

	for (int y = 0; y < PLAY_SIZE; ++y) {
		for (int x = 0; x < PLAY_SIZE; ++x) {
			if (game_state.get(x, y) == EMPTY) {
				/*
					Create a new state with action applied
				*/
				Play_field new_game_state = game_state;
				new_game_state.put(x, y, shape);

				/*
					Recursively call MIN_VALUE and store the action and its v value
				*/
				node_generate_count++;
				temp_v = MIN_VALUE(new_game_state, alpha, beta, -shape);

				/*
					Updating v, alpha and prune if needed
				*/
				v = max(v, temp_v);
				if (v >= beta)
					return v;
				alpha = max(alpha, v);
			}
		}
	}

	return v;
}


int MIN_VALUE(Play_field& game_state, int alpha, int beta, const int shape) {
	/*
	Utility node checking
	*/
	int utility = UTILITY(game_state, -shape);
	if (utility != NOT_OVER)
		return utility;

	int v = INT_MAX, temp_v;

	for (int y = 0; y < PLAY_SIZE; ++y) {
		for (int x = 0; x < PLAY_SIZE; ++x) {
			if (game_state.get(x, y) == EMPTY) {
				/*
					Create a new state with action applied
				*/
				Play_field new_game_state = game_state;
				new_game_state.put(x, y, shape);
				/*
					Recursively call MAX_VALUE and store the action and its v value
				*/
				node_generate_count++;
				temp_v = MAX_VALUE(new_game_state, alpha, beta, -shape);

				/*
					Updating v, beta and prune if needed
				*/
				v = min(v, temp_v);
				if (v <= alpha)
					return v;
				beta = min(beta, v);
			}
		}
	}

	return v;
}