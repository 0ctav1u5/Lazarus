#ifndef OVERLAYOBJECT
#define OVERLAYOBJECT



class OverlayObject {
private:
	SDL_Rect rect = {};
	SDL_Surface* SURFACE = nullptr;
	SDL_Texture* texture = nullptr;
	const char* IMAGEPATH = "";
	int X, Y, WIDTH, HEIGHT;
	bool exists = true;
	int LevelID = 0;

public:


	OverlayObject(int x, int y, int width, int height) : X(x), Y(y), WIDTH(width), HEIGHT(height) {

		rect = { X, Y, WIDTH, HEIGHT };
	
	}

	~OverlayObject() {
		if (SURFACE) {
			SDL_FreeSurface(SURFACE);
		}
		if (texture) {
			SDL_DestroyTexture(texture);
		}
	}

	void SetTexture(const char* imagepath);
	void RenderOverlayObject(SDL_Renderer* renderer);
	void Remove();

};


#endif 