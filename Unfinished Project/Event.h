#ifndef EVENT_H
#define EVENT_H

#include <SDL.h>

class Event {
	public:
		Event();
		virtual ~Event(); 
	
		virtual void vOnEvent(SDL_Event* ptEvent);

		virtual void vOnInputFocus();
		virtual void vOnInputBlur();

		virtual void vOnKeyDown(SDLKey tSym, SDLMod tMod, Uint16 tUnicode);
		virtual void vOnKeyUp(SDLKey tSym, SDLMod tMod, Uint16 tUnicode);

		virtual void vOnMouseFocus();
		virtual void vOnMouseBlur();
		virtual void vOnMouseMove(int nMX, int nMY, int nRelX, int nRelY, bool bLeft, bool bRight, bool bMiddle);
		virtual void vOnMouseWheel(bool bUp, bool bDown); // Not implemented.

		virtual void vOnLButtonDown(int nMX, int nMY);
		virtual void vOnLButtonUp(int nMX, int nMY);
		virtual void vOnRButtonDown(int nMX, int nMY);
		virtual void vOnRButtonUp(int nMX, int nMY);
		virtual void vOnMButtonDown(int nMX, int nMY);
		virtual void vOnMButtonUp(int nMX, int nMY);

		virtual void vOnJoyAxis(Uint8 tWhich, Uint8 tAxis, Sint16 tValue);
		virtual void vOnJoyButtonDown(Uint8 tWhich, Uint8 tButton);
		virtual void vOnJoyButtonUp(Uint8 tWhich, Uint8 tButton);
		virtual void vOnJoyHat(Uint8 tWhich, Uint8 tHat, Uint8 value);
		virtual void vOnJoyBall(Uint8 tWhich, Uint8 tBall, Sint16 tXRel, Sint16 tYRrel);

		virtual void vOnMinimize();
		virtual void vOnRestore();
		virtual void vOnResize(int nW, int nH);
		virtual void vOnExpose();
		virtual void vOnExit();

		virtual void vOnUser(Uint8 tType, int tCode, void* ptData1, void* ptData2);
};

#endif