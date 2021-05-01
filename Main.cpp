//Using SDL and standard IO
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
int speed = 4;
bool field [SCREEN_WIDTH][SCREEN_HEIGHT] = { 0 };
const int PLAYER_WIDTH = 3;
int a, b, delta, winner;
const int MAX_PLAYER = 4;
bool fullscreen = 0;
bool victoryDisplayed = 0;
char victoryString[256];
SDL_Color whiteColor = { 0x255, 0x255, 0x255 };
SDL_Color blackColor = { 0x00, 0x00, 0x00 };
SDL_Color blueColor = { 0x00, 0x00, 0x255 };
SDL_Color redColor = { 0x255, 0x00, 0x00 };
SDL_Color greenColor = { 0x00, 0x255, 0x00 };
SDL_Color yellowColor = { 0x255, 0x255, 0x00 };

// Struct for TRON Player
struct player {
	int x, y, dir, active;
	Uint32 color;
	SDL_Color sdlColor;

	// Constructor
	player(SDL_Color c, SDL_PixelFormat* format) {
		x = rand() % SCREEN_WIDTH;
		y = rand() % SCREEN_HEIGHT;
		sdlColor = c;
		color = SDL_MapRGB(format, c.r, c.g, c.b);
		dir = rand() % 4;
		active = 1;
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
		active = 1;
	}

};

// Toggle to Fullscreen
void ToggleFullscreen(SDL_Window* Window) {

	Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN;
	bool IsFullscreen = SDL_GetWindowFlags(Window) & FullscreenFlag;
	SDL_SetWindowFullscreen(Window, IsFullscreen ? 0 : FullscreenFlag);
	SDL_ShowCursor(IsFullscreen);

}

int main(int argc, char* args[])
{
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	// Event and Exit management
	int quit = 0;
	SDL_Event event;

	//The surface contained by the window
	SDL_Surface* screenSurface = NULL;

	// Rect for text
	SDL_Rect textPosition;
	SDL_Surface* victoryText;

	// Font for TTF
	TTF_Font* police = NULL;

	// Initialize TTF
	if (TTF_Init() == -1)
	{
		fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
	}

	// Load font
	police = TTF_OpenFont("comic.ttf", 65);


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
			int aliveplayers = 4;

			//Fill the surface black at the beginning
			//Get window surface
			screenSurface = SDL_GetWindowSurface(window);
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));

			// Init players (1 - red, 2 green, 3 blue, 4 yellow)
			player players[MAX_PLAYER] = { player(redColor, screenSurface->format),
										player(greenColor,  screenSurface->format),
										player(blueColor, screenSurface->format),
										player(yellowColor, screenSurface->format)};

			/* Loop until an SDL_QUIT event is found */
			while (!quit) {

				// reset game when needed
				if (reset) {

					game = 1;
					SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));
					memset(*field, 0, sizeof(field));
					aliveplayers = playernbr;

					for (int p = 0; p < MAX_PLAYER; p++) {
						players[p].reset();
					}

					// reset victory text
					victoryDisplayed = 0;
				
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


							/*case SDLK_F12:
								if (fullscreen) {
									SDL_SetWindowFullscreen(window, 0);
									SDL_SetWindowSize(window, SCREEN_WIDTH, SCREEN_HEIGHT);
									screenSurface = SDL_GetWindowSurface(window);
								}
								else {
									SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
									screenSurface = SDL_GetWindowSurface(window);
								}
								break;
								*/
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

					// Game management (if)
					// End of game management (else)
					if (game) {

						// Physical update + player DRAW
						for (int i = 0; i < speed; i++) {

							// player update
							for (int u = 0; u < playernbr; u++) {
								if (players[u].active == 1)
								players[u].tick();
							}

							// Endgame check
							aliveplayers = 0;
							for (int u = 0; u < playernbr; u++) {

								if (field[players[u].x][players[u].y] == 1) players[u].active = 0;
								if (players[u].active == 1) aliveplayers++;
							}
							if (aliveplayers <= 1) game = 0;

							// Update game field
							for (int u = 0; u < playernbr; u++) {
								if (players[u].active == 1)
								field[players[u].x][players[u].y] = 1;
							}

							// Draw players
							for (int u = 0; u < playernbr; u++) {
								if (players[u].active == 1) {
									SDL_Rect affp1 = { players[u].x, players[u].y, PLAYER_WIDTH, PLAYER_WIDTH };
									SDL_FillRect(screenSurface, &affp1, players[u].color);
								}

							}

						}
					} else {
						// End of game
						if (!victoryDisplayed) {

							// Recherche du gagnant
							winner = 99;
							for (int u = 0; u < playernbr; u++) {
								if (players[u].active == 1)
									winner = u;
							}

							if(winner == 99) {
								snprintf(victoryString, 256, "Match nul");
								// prépatation du texte
								victoryText = TTF_RenderText_Blended(police, victoryString, whiteColor);
							}
							else {
								snprintf(victoryString, 256, "Joueur %d gagne", winner + 1);
								// préparation du texte
								victoryText = TTF_RenderText_Blended(police, victoryString, players[winner].sdlColor);
							}

							// Affichage du texte
							textPosition.x = 180;
							textPosition.y = 210;
							SDL_BlitSurface(victoryText, NULL, screenSurface, &textPosition); /* Blit du texte */

							victoryDisplayed = 1;

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

	// Close font and Quit TTF
	TTF_CloseFont(police);
	TTF_Quit();

	return 0;
}