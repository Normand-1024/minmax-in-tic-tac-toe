#include <iostream>
#include "AIplayer.h"

using namespace std;

void run_game();

int main()
{

	cout << "=====================================\nWelcome to Tic Tac Toe Premium Edition! \n=====================================\n";

	Play_field play_field;

	play_field.display();

	start_game();

	return 0;
}

void start_game() {
	int player_shape, ai_shape, difficulty;
	char input;
	bool valid_input = false, player_first;

	//Determine difficulty
	while (!valid_input) {
		cout << "Please Choose Your Difficulty!\n1: Easy\n2 : Medium\n3 : Hard\nEnter Number:";
		cin >> difficulty;
		if (difficulty < 4 && difficulty > 0) valid_input = true;
		else cout << "Invalid Input\n";
	}

	valid_input = false;

	//Determine player role
	while (!valid_input) {
		cout << "Enter your role, O or X:\nEnter Role:";
		valid_input = true;
		cin >> input;
		if (input == 'O') {
			player_shape = O;
			ai_shape = X;
		}
		else if (input == 'X') {
			player_shape = X;
			ai_shape = O;
		}
		else {
			cout << "Invalid Input\n";
			valid_input = false;
		}
	}

	valid_input = false;

	//Determine player play order
	while (!valid_input) {
		cout << "Enter playing order, 1 if player plays first, 0 if player plays first.\nEnter Order:";
		cin >> input;
		valid_input = true;
		if (input == '1') player_first = true;
		else if (input == '0') player_first = false;
		else {
			cout << "Invalid Input\n";
			valid_input = false;
		}
	}

	cout << "Your difficulty is " << difficulty << "\nYour role is " << player_shape << "\nYour play first? " << player_first << "\n" ;

	cin >> input;
}