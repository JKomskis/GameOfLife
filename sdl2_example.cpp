/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void setPixel(SDL_Surface* screenSurface, int x, int y, Uint32 color)
{
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = 1;
	dst.h = 1;
	SDL_FillRect(screenSurface, &dst, color);
}

int main( int argc, char* args[] )
{
	//The window we'll be rendering to
	SDL_Window* window = NULL;
	
	//The surface contained by the window
	SDL_Surface* screenSurface = NULL;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	}
	else
	{
		//Create window
		window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( window == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get window surface
			screenSurface = SDL_GetWindowSurface( window );

			//Fill the surface white
			SDL_FillRect( screenSurface, NULL, 0 );
			
			//Update the surface
			SDL_UpdateWindowSurface( window );
			for(int i=0; i < 4 * 480; i++)
			{
				setPixel(screenSurface, i % SCREEN_WIDTH, i % SCREEN_HEIGHT, 0xFFFFFF);
				SDL_UpdateWindowSurface( window );
				SDL_Delay( 0 );
				
			}
			//Wait two seconds
			SDL_Delay( 100 );
		}
	}

	//Destroy window
	SDL_DestroyWindow( window );

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}
