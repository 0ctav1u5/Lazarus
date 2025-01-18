#ifndef LEVEL
#define LEVEL
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

class Level {
private:
	int LevelID;
	std::string LevelName;
	const char* BackgroundImage;
	SDL_Texture* BackgroundTexture;
public:

	Level(int num, std::string levelname, const char* backgroundimage,
		SDL_Texture* backgroundtexture) : LevelID(num), LevelName(levelname), 
		BackgroundImage(backgroundimage), BackgroundTexture(backgroundtexture){}



	void DrawLevel() {

	}


};










#endif // ! 
