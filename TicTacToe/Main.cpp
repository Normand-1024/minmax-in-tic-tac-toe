#include <iostream>
#include "AIplayer.h"

using namespace std;

int run_game(Play_field& game_state, int player_shape, int difficulty, bool player_first);
void start_game();

int main()
{

	cout << "=====================================\nWelcome to Tic Tac Toe Premium Edition! \n=====================================\n";

	start_game();

	return 0;
}

void start_game() {
	int player_shape, difficulty, win_condition;
	char input;
	bool game_over = false, reset_setting = false, player_first;
	Play_field play_field;

	while (!game_over) {
		/*
			Determine Difficulty
		*/
		cout << "\nPlease Choose Your Difficulty!\n1: Easy\n2: Medium\n3: Hard\nEnter Number:";
		cin >> difficulty;

		/*
			Determine Player Role
		*/
		cout << "\nEnter your role, O or X:\nEnter Role:";
		cin >> input;
		if (input == 'O')
			player_shape = O;
		else
			player_shape = X;

		/*
			Determine Player Order
		*/
		cout << "\nEnter playing order, 1 if player plays first, 0 if player plays first.\nEnter Order:";
		cin >> input;
		player_first = (input == '1');

		/*
			Run Game
		*/
		reset_setting = false;
		while (!game_over && !reset_setting) {
			play_field.clear();

			cout << "==============\nGame Start     \n==============\n";

			win_condition = run_game(play_field, player_shape, difficulty, player_first);
			
			/*
				End Game Text
			*/
			if (win_condition == player_shape)
				cout << "==============\nYOU WON!!!!     \n==============\n";
			else if (win_condition == 0)
				cout << "==============\nDRAW! GOOD ENOUGH     \n==============\n";
			else
				cout << "==============\nbetter luck next time      \n==============\n";

			/*
				Game Over? Reset Settings?
			*/
			cout << "Exit Game? 1 to exit, 0 TO REPLAY.\nEnter Command:";
			cin >> input;
			game_over = (input == '1');

			if (!game_over) {
				cout << "\nReset Settings? 1 to reset, 0 to play now.\nEnter Command:";
				cin >> input;
				reset_setting = (input == '1');
			}
		}
	}
}

int run_game(Play_field& game_state, int player_shape, int difficulty, bool player_first) {
	int win_condition = NOT_OVER, ai_shape, player_x, player_y;
	bool player_played = !player_first;

	if (player_shape == X)
		ai_shape = O;
	else
		ai_shape = X;

	do {
		game_state.display();

		if (player_played)
			ai_move(game_state, ai_shape);
		else {
			cout << "\nEnter X coordiante(start with 1 from the left): ";
			cin >> player_x;
			--player_x;

			cout << "\nEnter Y coordiante(start with 1 from the bottom): ";
			cin >> player_y;
			player_y = PLAY_SIZE - player_y;

			game_state.put(player_x, player_y, player_shape);
		}

		player_played = !player_played;

	} while ((win_condition = game_state.check_win_condition()) == NOT_OVER);

	game_state.display();

	return win_condition;

}