//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
int speed = 4;
bool field [SCREEN_WIDTH][SCREEN_HEIGHT] = { 0 };

// Struct for TRON Player
struct player {
	int x, y, dir;
	SDL_Color color;

	// Constructor
	player(SDL_Color c) {
		x = rand() % SCREEN_WIDTH;
		y = rand() % SCREEN_HEIGHT;
		color = c;
		dir = rand() % 4;
	}

	void tick() {
		if (dir == 0) y += 1;
		if (dir == 1) x -= 1;
		if (dir == 2) x += 1;
		if (dir == 3) y -= 1;

		// Allow to loop outside screen
		if (x >= SCREEN_WIDTH) x = 0;
		if (x < 0) x = SCREEN_WIDTH - 1;
		if (y >= SCREEN_HEIGHT) y = 0;
		if (y < 0) x = SCREEN_HEIGHT - 1;
	
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

	// Init players (1 - red, 2 green)
	player p1({ 255, 0, 0 }), p2({ 0, 255, 0 });

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		//Create window
		window = SDL_CreateWindow("Tile Platformer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else
		{

			/* Loop until an SDL_QUIT event is found */
			while (!quit) {

				/* Poll for events */
				while (SDL_PollEvent(&event)) {

					switch (event.type) {
						/* Keyboard event */
					case SDL_KEYDOWN:
						/* Touche appuyée, changement de statut */
						switch (event.key.keysym.sym) {
						case SDLK_LEFT:
							//alien_xvel = -1;
							break;
						case SDLK_RIGHT:
							//alien_xvel = 1;
							break;
						case SDLK_UP:
							//alien_yvel = -1;
							break;
						case SDLK_DOWN:
							//alien_yvel = 1;
							break;
						case SDLK_x:
							//alien_yvel = 1;
							break;
						case SDLK_c:
							//alien_yvel = 1;
							break;
						case SDLK_ESCAPE:
							quit = 1;
							break;
						default:
							break;
						}


						break;
						/* Lorsque la touche est relachée, on annule le mouvement */
					case SDL_KEYUP:

						/* Touche appuyée, changement de statut */
						switch (event.key.keysym.sym) {
						case SDLK_LEFT:
							//alien_xvel = -1;
							break;
						case SDLK_RIGHT:
							//alien_xvel = 1;
							break;
						case SDLK_UP:
							//alien_yvel = -1;
							break;
						case SDLK_DOWN:
							//alien_yvel = 1;
							break;
						case SDLK_x:
							//alien_yvel = 1;
							break;
						case SDLK_c:
							//alien_yvel = 1;
							break;
						case SDLK_ESCAPE:
							quit = 1;
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

				//Get window surface
				screenSurface = SDL_GetWindowSurface(window);

				//Fill the surface black
				SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0x00));

				//Update the surface
				SDL_UpdateWindowSurface(window);

			}

		}
	}

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}