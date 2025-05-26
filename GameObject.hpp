#ifndef GAME_OBJECT
#define GAME_OBJECT
#include <iostream>
#include <SDL.h>

// TODO: Add unique ID for each object so we can destroy after collecting 
class GameObject {
private:
	std::string NAME = "";
	SDL_Rect rect = {};
	SDL_Surface* SURFACE = nullptr;
	SDL_Texture* texture = nullptr;
	const char* IMAGEPATH = "";
	int X, Y, WIDTH, HEIGHT;
	int XUrange, XLrange;
	int YUrange, YLrange;
	bool CanCollide;
	bool CanDamage; // damage zone is in the centre point line and right of the rect
	bool CanCollect;
	bool Visible;
	bool HasTexture = false;
public:

	GameObject(std::string name, int x, int y, int width, int height, bool cancollide, bool candamage, bool cancollect,
		bool visible, int xurange, int xlrange, int yurange, int ylrange)
		: NAME(name), X(x), Y(y), WIDTH(width), HEIGHT(height), CanCollide(cancollide), CanDamage(candamage), 
		CanCollect(cancollect), Visible(visible), XUrange(xurange), XLrange(xlrange), YUrange(yurange), YLrange(ylrange)
	{
		rect = { X, Y, WIDTH, HEIGHT };
	}

	~GameObject() {
		if (SURFACE) {
			SDL_FreeSurface(SURFACE);
		}
		if (texture) {
			SDL_DestroyTexture(texture);
		}
	}

	void FlipTexture();
	void SetTexture(const char* imagepath);
	void RenderGameObject(SDL_Renderer* renderer);
	std::string GetName();
	bool GetCanDamage();
	bool GetCanCollect();
	int GetX();
	int GetY();
	int GetGameObjectWidth();
	int GetGameObjectHeight();
	SDL_Rect& GetRect();
	int GetXU();
	int GetXL();
	int GetYU();
	int GetYL();
	bool CheckCollidable();
	std::string CheckBoundary(int PlayerY, int PlayerX, int PlayerWidth, int PlayerHeight);
};
#endif 
