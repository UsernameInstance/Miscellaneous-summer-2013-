#ifndef TEXT_H
#define TEXT_H

#include <SDL.h>
#include <iostream>
#include <sstream>
#include <string>

class Text {
private:
	Text();
	int iSize;

	static Text* s_pcText;
	static bool s_bInstance;

	SDL_Surface* psFont;

public:
	static Text* Instance();
	~Text();

	std::string tString;
	template <typename T>
	inline std::string ToString(T tX) {
		std::ostringstream oStream;
		oStream << tX;
		return oStream.str();
	}

	void RenderChar(SDL_Surface* psTarg, int iX, int iY, int iCh);
	void RenderString(SDL_Surface* psTarg, int iX, int iY, const char* pchS);

};





#endif