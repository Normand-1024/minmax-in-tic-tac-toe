#include "AIplayer.h"
#include "Renderer.h"
#include <SDL.h>
#include <windows.h>
#undef main

using namespace std;

int run_game(Play_field& game_state, int player_shape, int difficulty, bool player_first, SDL_Window* win, SDL_Renderer* ren, vector<SDL_Texture*>& display_vec, vector<SDL_Texture*>& role_vec);
void start_game(SDL_Window* win, SDL_Renderer* ren, vector<SDL_Texture*>& display_vec, vector<SDL_Texture*>& role_vec);

int main()
{
	SDL_Init(SDL_INIT_VIDEO);

	/*
		Get the resolution of current monitor display, useful for determining where the window will be displayed
	*/
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	int horizontal_res = desktop.right, vertical_res = desktop.bottom;

	SDL_Window *win = SDL_CreateWindow("Tic Tac Toe Premium Edition", horizontal_res/2, vertical_res/8, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	/*
		Have 4 vectors, two storing path to each texture, two loading textures into the vector
	*/
	vector<string> display_path_vec = { 
		"texture\\background1.png", "texture\\background2.png",
		"texture\\endscreen.png", "texture\\endscreen2.png", "texture\\endscreen3.png", "texture\\replayscreen.png" };
	vector<string> role_path_vec = { "texture\\O.png", "texture\\X.png", "texture\\YES.png" };

	vector<SDL_Texture*> display_vec, role_vec;
	for (string& i : display_path_vec)
		display_vec.push_back(loadTexture(i, ren));
	for (string& i : role_path_vec)
		role_vec.push_back(loadTexture(i, ren));

	start_game(win, ren, display_vec, role_vec);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}

void start_game(SDL_Window* win, SDL_Renderer* ren, vector<SDL_Texture*>& display_vec, vector<SDL_Texture*>& role_vec) {
	int player_shape = O, difficulty = 3, game_over = -1;
	bool reset_setting = false, player_first = 0, valid_arg = false;
	SDL_Event windowEvent;
	Play_field play_field;

	const int easy_y_start = 2 * SCREEN_HEIGHT / 8, normal_y_start = (2 * SCREEN_HEIGHT / 8) + (SCREEN_HEIGHT / (3 * 8)), hard_y_start = (2 * SCREEN_HEIGHT / 8) + (2 * SCREEN_HEIGHT / (3 * 8));

	while (game_over != 1) {
		
		bool start_playing = false;
		game_over = -1;

		while (!start_playing) {
			SDL_RenderClear(ren);

			/*
				Catch and react to SDL_Event
			*/
			if (SDL_PollEvent(&windowEvent)) {
				if (windowEvent.type == SDL_QUIT) {
					SDL_DestroyWindow(win);
					game_over = true;
					break;
				}
				else if (windowEvent.type == SDL_MOUSEBUTTONDOWN) {
					//If player clicks on difficulty button
					if (windowEvent.button.y > easy_y_start && windowEvent.button.y < 3 * SCREEN_HEIGHT / 8 && windowEvent.button.x > 2 * SCREEN_WIDTH / 3) {
						if (windowEvent.button.y > easy_y_start && windowEvent.button.y < normal_y_start)
							difficulty = 1;
						else if (windowEvent.button.y > normal_y_start && windowEvent.button.y < hard_y_start)
							difficulty = 2;
						else if (windowEvent.button.y > hard_y_start && windowEvent.button.y < 3 * SCREEN_HEIGHT / 8)
							difficulty = 3;
					}

					//If player clicks on role
					if (windowEvent.button.y > 3 * SCREEN_HEIGHT / 8 && windowEvent.button.y < SCREEN_HEIGHT / 2 && windowEvent.button.x > 2 * SCREEN_WIDTH / 3)
						player_shape = -player_shape;

					//If player clicks on player first
					if (windowEvent.button.y > SCREEN_HEIGHT / 2 && windowEvent.button.y < 5 * SCREEN_HEIGHT / 8 && windowEvent.button.x > 2 * SCREEN_WIDTH / 3)
						player_first = !player_first;

					//If player clicks on PLAY GAME
					if (windowEvent.button.y > 5 * SCREEN_HEIGHT / 8)
						start_playing = true;
				}
			}

			/*
				Render setting screen
			*/
			renderTexture(display_vec[background_1], ren, 0, 0);

			//Render difficulty choice
			if (difficulty == 1)
				renderTexture(role_vec[YES], ren, 2 * SCREEN_WIDTH / 3, easy_y_start);
			else if (difficulty == 2)
				renderTexture(role_vec[YES], ren, 2 * SCREEN_WIDTH / 3, normal_y_start);
			else
				renderTexture(role_vec[YES], ren, 2 * SCREEN_WIDTH / 3, hard_y_start);

			//Render player shape
			if (player_shape == O)
				renderTexture(role_vec[O_role], ren, 2 * SCREEN_WIDTH / 3, 3 * SCREEN_HEIGHT / 8);
			else
				renderTexture(role_vec[X_role], ren, 2 * SCREEN_WIDTH / 3, 3 * SCREEN_HEIGHT / 8);

			//Render player play first choice
			if (player_first)
				renderTexture(role_vec[YES], ren, 2 * SCREEN_WIDTH / 3, 4 * SCREEN_HEIGHT / 8);

			SDL_RenderPresent(ren);
		}

		/*
			game_over variable is used to control player's choice on menu selection, particularly after the game ends
			-1: replay with the same setting (initialized as default)
			0: replay but change game setting
			1: exit game
		*/
		while(game_over == -1)
			game_over = run_game(play_field, player_shape, difficulty, player_first, win, ren, display_vec, role_vec);
	}
}

int run_game(Play_field& game_state, int player_shape, int difficulty, bool player_first, SDL_Window* win, SDL_Renderer* ren, vector<SDL_Texture*>& display_vec, vector<SDL_Texture*>& role_vec) {
	int win_condition = NOT_OVER, ai_shape = -player_shape, player_x = -1, player_y = -1, game_progress = 0;
	SDL_Event windowEvent;
	bool player_played = !player_first, replacing_tile = true;

	game_state.clear();

	do {
		if (player_played)
			ai_move(game_state, ai_shape, game_progress, difficulty);
		else {
			while (true) {
				player_x = -1, player_y = -1;
				SDL_RenderClear(ren);

				//Listen and react to SDL_Event
				if (SDL_PollEvent(&windowEvent)) {
					if (windowEvent.type == SDL_QUIT) {
						SDL_DestroyWindow(win);
						return 1;
					}
					else if (windowEvent.type == SDL_MOUSEBUTTONDOWN) {
						player_x = windowEvent.button.x / (SCREEN_WIDTH / PLAY_SIZE);
						player_y = windowEvent.button.y / (SCREEN_HEIGHT / PLAY_SIZE);
					}
				}

				game_state.display(ren, display_vec, role_vec);
				SDL_RenderPresent(ren);

				//If clicked
				if (game_state.valid_input(player_x, player_y)) break;
			}

			game_state.put(player_x, player_y, player_shape);
		}

		player_played = !player_played;
		game_progress++;

	} while ((win_condition = game_state.check_win_condition()) == NOT_OVER);

	/*
	
		A while loop for displaying the screen after the game is over

	*/
	while (true) {
		SDL_RenderClear(ren);
		game_state.display(ren, display_vec, role_vec);

		if(win_condition == player_shape)
			renderTexture(display_vec[win_screen], ren, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4);
		else if(win_condition == -player_shape)
			renderTexture(display_vec[lost_screen], ren, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4);
		else
			renderTexture(display_vec[draw_screen], ren, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4);

		if (SDL_PollEvent(&windowEvent)) {
			if (windowEvent.type == SDL_QUIT) {
				SDL_DestroyWindow(win);
				return true;
			}
			else if (windowEvent.type == SDL_MOUSEBUTTONDOWN) {
				if (windowEvent.button.x > SCREEN_WIDTH / PLAY_SIZE && windowEvent.button.x < 3 * SCREEN_WIDTH / PLAY_SIZE) {
					if (windowEvent.button.y > SCREEN_HEIGHT / 2 && windowEvent.button.y < (SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 8))
						break;
					else if (windowEvent.button.y >(SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 8) && windowEvent.button.y < (SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 4))
						return 1;
				}
			}
		}

		SDL_RenderPresent(ren);
	}

	/*

		A while loop for displaying the screen after the game is over and the player chose PLAY AGAIN option

	*/
	while (true) {
		SDL_RenderClear(ren);
		game_state.display(ren, display_vec, role_vec);

		renderTexture(display_vec[replay_screen], ren, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4);

		if (SDL_PollEvent(&windowEvent)) {
			if (windowEvent.type == SDL_QUIT) {
				SDL_DestroyWindow(win);
				return true;
			}
			else if (windowEvent.type == SDL_MOUSEBUTTONDOWN) {
				if (windowEvent.button.x > SCREEN_WIDTH / PLAY_SIZE && windowEvent.button.x < 3 * SCREEN_WIDTH / PLAY_SIZE) {
					if (windowEvent.button.y > SCREEN_HEIGHT / 2 && windowEvent.button.y < (SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 8))
						return 0;
					else if (windowEvent.button.y >(SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 8) && windowEvent.button.y < (SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 4))
						return -1;
				}
			}
		}

		SDL_RenderPresent(ren);
	}

	return 1;

}