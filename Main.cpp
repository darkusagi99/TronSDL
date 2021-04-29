//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
int speed = 4;
bool field [SCREEN_WIDTH][SCREEN_HEIGHT] = { 0 };
const int PLAYER_WIDTH = 3;
int a, b, delta;
const int MAX_PLAYER = 4;

// Struct for TRON Player
struct player {
	int x, y, dir, active;
	Uint32 color;

	// Constructor
	player(Uint32 c) {
		x = rand() % SCREEN_WIDTH;
		y = rand() % SCREEN_HEIGHT;
		color = c;
		dir = rand() % 4;
	}

	void tick() {
		if (dir == 0) y += 1;
		if (dir == 1) x+= 1;
		if (dir == 2) y -= 1;
		if (dir == 3) x -= 1;

		// Allow to loop outside screen
		if (x >= SCREEN_WIDTH) x = 0;
		if (x < 0) x = SCREEN_WIDTH - 1;
		if (y >= SCREEN_HEIGHT) y = 0;
		if (y < 0) y = SCREEN_HEIGHT - 1;
	
	}

	void reset() {
		x = rand() % SCREEN_WIDTH;
		y = rand() % SCREEN_HEIGHT;
		dir = rand() % 4;
	}

};

int main(int argc, char* args[])
{
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	// Event and Exit management
	int quit = 0;
	SDL_Event event;

	//The surface contained by the window
	SDL_Surface* screenSurface = NULL;


	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		//Create window
		window = SDL_CreateWindow("TRON", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else
		{

			bool game = 1;
			bool reset = 0;
			int playernbr = 4;

			//Fill the surface black at the beginning
			//Get window surface
			screenSurface = SDL_GetWindowSurface(window);
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));

			// Init players (1 - red, 2 green, 3 blue, 4 yellow)
			player players[MAX_PLAYER] = { player(SDL_MapRGB(screenSurface->format, 0x255, 0x00, 0x00)),
										player(SDL_MapRGB(screenSurface->format, 0x00, 0x255, 0x00)),
										player(SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x255)),
										player(SDL_MapRGB(screenSurface->format, 0x255, 0x255, 0x00)) };

			/* Loop until an SDL_QUIT event is found */
			while (!quit) {

				// reset game when needed
				if (reset) {

					game = 1;
					SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));
					memset(*field, 0, sizeof(field));

					for (int p = 0; p < MAX_PLAYER; p++) {
						players[p].reset();
					}
				
					// Reset the reset flag
					reset = 0;
				}

				// Capping to 60 FPS
				a = SDL_GetTicks();
				delta = a - b;


				if (delta > 1000 / 60.0) {

					// FPS Management
					b = a;

					/* Poll for events */
					while (SDL_PollEvent(&event)) {

						switch (event.type) {
							/* Keyboard event */
						case SDL_KEYDOWN:
							/* Touche appuyée, changement de statut */
							switch (event.key.keysym.sym) {
							case SDLK_LEFT:
								// Player 2 LEFT
								players[1].dir = (players[1].dir + 1) % 4;
								break;
							case SDLK_RIGHT:
								// Player 2 RIGHT
								players[1].dir = (players[1].dir + 3) % 4;
								break;
							case SDLK_x:
								// Player 1 LEFT
								players[0].dir = (players[0].dir + 1) % 4;
								break;
							case SDLK_c:
								// Player 1 RIGHT
								players[0].dir = (players[0].dir + 3) % 4;
								break;

							case SDLK_a:
								// Player 3 LEFT
								players[2].dir = (players[2].dir + 1) % 4;
								break;
							case SDLK_q:
								// Player 3 RIGHT
								players[2].dir = (players[2].dir + 3) % 4;
								break;


							case SDLK_KP_6:
								// Player 4 LEFT
								players[3].dir = (players[3].dir + 1) % 4;
								break;
							case SDLK_KP_9:
								// Player 4 RIGHT
								players[3].dir = (players[3].dir + 3) % 4;
								break;
							case SDLK_ESCAPE:
								// Exit game
								quit = 1;
								break;
							case SDLK_r:
								// Reset game
								reset = 1;
								break;

							case SDLK_F1 :
								playernbr = 1;
								break;
							case SDLK_F2:
								playernbr = 2;
								break;
							case SDLK_F3:
								playernbr = 3;
								break;
							case SDLK_F4:
								playernbr = 4;
								break;

							default:
								break;
							}


							break;

						/* SDL_QUIT event (window close) */
						case SDL_QUIT:
							quit = 1;
							break;

						default:
							break;
						}

					}

					// End of game management
					if (!game) continue;

					// Physical update
					for (int i = 0; i < speed; i++) {

						// player update
						for (int u = 0; u < playernbr; u++) {
							players[u].tick();
						}

						// Endgame check
						for (int u = 0; u < playernbr; u++) {

							if (field[players[u].x][players[u].y] == 1) game = 0;
						}

						// Update game field
						for (int u = 0; u < playernbr; u++) {
							field[players[u].x][players[u].y] = 1;
						}

						// Draw players
						for (int u = 0; u < playernbr; u++) {
							SDL_Rect affp1 = { players[u].x, players[u].y, PLAYER_WIDTH, PLAYER_WIDTH };
							SDL_FillRect(screenSurface, &affp1, players[u].color);

						}

					}



					//Update the surface
					SDL_UpdateWindowSurface(window);

				}
			}

		}
	}

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}