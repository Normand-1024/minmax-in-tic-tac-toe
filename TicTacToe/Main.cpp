#include <iostream>
#include <string>
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
	string dumpster;
	bool game_over = false, reset_setting = false, player_first = 0, valid_arg = false;
	Play_field play_field;

	while (!game_over) {
		/*
			Determine Difficulty
		*/
		while (!valid_arg){
			cout << "\nPlease Choose Your Difficulty!\n1: Easy\n2: Medium\n3: Hard\nEnter Number:";
			cin.get(input);
			getline(cin, dumpster);
			valid_arg = true;
			if (input == '1')	difficulty = 1;
			else if (input == '2')	difficulty = 2;
			else if (input == '3')	difficulty = 3;
			else {
				valid_arg = false;
				cout << "\n Invalid Input \n";
			}
		}

		valid_arg = false;

		/*
			Determine Player Role
		*/
		while (!valid_arg) {
			cout << "\nEnter your role, O or X:\nEnter Role:";
			cin.get(input);
			getline(cin, dumpster);
			valid_arg = true;
			if (input == 'O')	player_shape = O;
			else if (input == 'X')	player_shape = X;
			else {
				valid_arg = false;
				cout << "\n Invalid Input \n";
			}
		}

		valid_arg = false;

		/*
			Determine Player Order
		*/
		while (true) {
			cout << "\nEnter playing order, 1 if player plays first, 0 if AI plays first.\nEnter Order:";
			cin.get(input);
			getline(cin, dumpster);
			if (player_first = (input == '1')) break;
			else if (input == '0')	break;
			else cout << "\n Invalid Input \n";
		}

		/*
			Run Game
		*/
		reset_setting = false;
		while (!game_over && !reset_setting) {
			play_field.clear();

			cout << "==============\nGame Start     \n============== difficulty: " << difficulty <<"\n";

			win_condition = run_game(play_field, player_shape, difficulty, player_first);
			
			/*
				End Game Text
			*/
			if (win_condition == player_shape)
				cout << "==============\nYOU WON!!!!\n==============\n";
			else if (win_condition == 0)
				cout << "==============\nDRAW! GOOD ENOUGH\n==============\n";
			else
				cout << "==============\nI mean, good spirit is the most important thing\n==============\n";

			/*
				Game Over? Reset Settings?
			*/
			while (true) {
				cout << "\nExit Game? 1 to exit, 0 TO REPLAY.\nEnter Command:";
				cin.get(input);
				getline(cin, dumpster);
				if (game_over = (input == '1')) break;
				else if (input == '0') break;
				else cout << "\n Invalid Input \n";
			}

			if (!game_over) {
				while (true) {
					cout << "\nReset Settings? 1 to reset, 0 to play now.\nEnter Command:";
					cin.get(input);
					getline(cin, dumpster);
					if (reset_setting = (input == '1')) break;
					else if (input == '0') break;
					else cout << "\n Invalid Input \n";
				}
			}
		}
	}
}

int run_game(Play_field& game_state, int player_shape, int difficulty, bool player_first) {
	int win_condition = NOT_OVER, ai_shape, player_x = 1, player_y = 1, game_progress = 0;
	char input;
	string dumpster;
	bool player_played = !player_first, replacing_tile = true;

	if (player_shape == X)
		ai_shape = O;
	else
		ai_shape = X;

	do {
		game_state.display();

		if (player_played)
			ai_move(game_state, ai_shape, game_progress, difficulty);
		else {
			while (replacing_tile) {
				while (true) {
					cout << "\nEnter X coordiante(start with 1 from the left): ";
					cin.get(input);
					getline(cin, dumpster);
					player_x = (int)input - 48;
					if (player_x >= 1 && player_x <= PLAY_SIZE ) break;
					else cout << "\n Invalid Input \n";
				}
				--player_x;

				while (true) {
					cout << "\nEnter Y coordiante(start with 1 from the bottom): ";
					cin.get(input);
					getline(cin, dumpster);
					player_y = (int)input - 48;
					if (player_y >= 1 && player_y <= PLAY_SIZE) break;
					else cout << "\n Invalid Input \n";
				}
				player_y = PLAY_SIZE - player_y;

				if (game_state.get(player_x, player_y) != EMPTY) cout << "\nTile already exists\n";
				else break;
			}

			game_state.put(player_x, player_y, player_shape);
		}

		player_played = !player_played;
		game_progress++;

	} while ((win_condition = game_state.check_win_condition()) == NOT_OVER);

	game_state.display();

	return win_condition;

}