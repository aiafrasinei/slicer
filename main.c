#define SDL_MAIN_HANDLED

#include <stdlib.h>
#include <SDL3/SDL.h>

#include "stk.h"


SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;

char *getCfg(const char *name) {
	struct STK_Result *result = STK_GetData();
	char *ret = "";

	for (int i = 0; i < result->nrLines; i++) {
		if (strcmp(name, result->lines[i].tokens[0].data) == 0) {
			ret = result->lines[i].tokens[2].data;
		}
	}

	return ret;
}

bool start()
{
    bool success = true;

	STK_Create("settings.ini");
	STK_Parse();

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDL_Log("SDL3 could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		char *convResult = nullptr;
		const int WIDTH = (int)strtol(getCfg("Width"), &convResult, 10);
		const int HEIGHT = (int)strtol(getCfg("Height"), &convResult, 10);

		gWindow = SDL_CreateWindow(getCfg("Title"), WIDTH , HEIGHT, 0);
		if( gWindow == nullptr )
		{
			SDL_Log("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, nullptr);
			if(gRenderer == nullptr)
			{
				SDL_Log("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
			}
		}
	}

	return success;
}

bool load() {

	return true;
}

void end()
{
	STK_Destroy();

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	gRenderer = nullptr;

	SDL_Quit();
}

int main(int argc, const char *argv[])
{
	bool loop = true;

    start();
	load();

	SDL_Event e;
	while(loop)
	{
		while(SDL_PollEvent(&e) != 0)
		{
			if(e.type == SDL_EVENT_QUIT)
			{
				loop = false;
				break;
			}
		}

		SDL_SetRenderDrawColor(gRenderer, 100, 0, 30, 255);
		SDL_RenderClear(gRenderer);


		SDL_RenderPresent(gRenderer);
	}

    end();

    return 0;
}