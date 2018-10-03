#include "Event.h"

Event::Event() {
}

Event::~Event() {
}

void Event::vOnEvent(SDL_Event* ptEvent) {
	switch (ptEvent->type) 
	{
		case SDL_ACTIVEEVENT: 
			switch (ptEvent->active.state) 
			{
				case SDL_APPMOUSEFOCUS: 
					if (ptEvent->active.gain) 
						vOnMouseFocus();
					else 
						vOnMouseBlur();
					break;
				case SDL_APPINPUTFOCUS:
					if (ptEvent->active.gain)
						vOnInputFocus();
					else
						vOnInputBlur();
					break;
				case SDL_APPACTIVE:
					if (ptEvent->active.gain)
						vOnRestore();
					else
						vOnMinimize();
					break;
			}
			break;

		case SDL_KEYDOWN:
			vOnKeyDown(ptEvent->key.keysym.sym, ptEvent->key.keysym.mod, ptEvent->key.keysym.unicode);
			break;

		case SDL_KEYUP:
			vOnKeyUp(ptEvent->key.keysym.sym, ptEvent->key.keysym.mod, ptEvent->key.keysym.unicode);
			break;

		case SDL_MOUSEMOTION:
			vOnMouseMove(ptEvent->motion.x, ptEvent->motion.y, ptEvent->motion.xrel, ptEvent->motion.yrel, (ptEvent->motion.state&SDL_BUTTON(SDL_BUTTON_LEFT)) != 0, (ptEvent->motion.state&SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0, (ptEvent->motion.state&SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0);
			break;

		case SDL_MOUSEBUTTONDOWN:
			switch (ptEvent->button.button)
			{
				case SDL_BUTTON_LEFT:
					vOnLButtonDown(ptEvent->button.x, ptEvent->button.y);
					break;

				case SDL_BUTTON_RIGHT:
					vOnRButtonDown(ptEvent->button.x, ptEvent->button.y);
					break;

				case SDL_BUTTON_MIDDLE:
					vOnMButtonDown(ptEvent->button.x, ptEvent->button.y);
					break;
			}
			break;

		case SDL_MOUSEBUTTONUP:
			switch (ptEvent->button.button)
			{
				case SDL_BUTTON_LEFT:
					vOnLButtonUp(ptEvent->button.x, ptEvent->button.y);
					break;

				case SDL_BUTTON_RIGHT:
					vOnRButtonUp(ptEvent->button.x, ptEvent->button.y);
					break;

				case SDL_BUTTON_MIDDLE:
					vOnMButtonUp(ptEvent->button.x, ptEvent->button.y);
					break;
			}
			break;

		case SDL_JOYAXISMOTION:
			vOnJoyAxis(ptEvent->jaxis.which, ptEvent->jaxis.axis, ptEvent->jaxis.value);
			break;

		case SDL_JOYBALLMOTION:
			vOnJoyBall(ptEvent->jball.which, ptEvent->jball.ball, ptEvent->jball.xrel, ptEvent->jball.yrel);
			break;

		case SDL_JOYHATMOTION:
			vOnJoyHat(ptEvent->jhat.which, ptEvent->jhat.hat, ptEvent->jhat.value);
			break;

		case SDL_JOYBUTTONDOWN:
			vOnJoyButtonDown(ptEvent->jbutton.which, ptEvent->jbutton.button);
			break;

		case SDL_JOYBUTTONUP:
			vOnJoyButtonUp(ptEvent->jbutton.which, ptEvent->jbutton.button);
			break;

		case SDL_QUIT:
			vOnExit();
			break;

		case SDL_SYSWMEVENT:
			//ignore
			break;

		case SDL_VIDEORESIZE:
			vOnResize(ptEvent->resize.w, ptEvent->resize.h);
			break;

		case SDL_VIDEOEXPOSE:
			vOnExpose();
			break;

		default:
			vOnUser(ptEvent->user.type, ptEvent->user.code, ptEvent->user.data1, ptEvent->user.data2);
			break;
	}		
}

void Event::vOnInputFocus() { /*Pure virtual, do nothing*/ }

void Event::vOnInputBlur() { /*Pure virtual, do nothing*/ }

void Event::vOnKeyDown(SDLKey tSym, SDLMod tMod, Uint16 tUnicode) { /*Pure virtual, do nothing*/ }

void Event::vOnKeyUp(SDLKey tSym, SDLMod tMod, Uint16 tUnicode) { /*Pure virtual, do nothing*/ }

void Event::vOnMouseFocus() { /*Pure virtual, do nothing*/ }

void Event::vOnMouseBlur() { /*Pure virtual, do nothing*/ }

void Event::vOnMouseMove(int nMX, int nMY, int nRelX, int nRelY, bool bLeft, bool bRight, bool bMiddle) { /*Pure virtual, do nothing*/ }

void Event::vOnMouseWheel(bool Up, bool Down) { /*Pure virtual, do nothing*/ }

void Event::vOnLButtonDown(int nMX, int nMY) { /*Pure virtual, do nothing*/ }

void Event::vOnLButtonUp(int nMX, int nMY) { /*Pure virtual, do nothing*/ }

void Event::vOnRButtonDown(int nMX, int nMY) { /*Pure virtual, do nothing*/ }

void Event::vOnRButtonUp(int nMX, int nMY) { /*Pure virtual, do nothing*/ }

void Event::vOnMButtonDown(int nMX, int nMY) { /*Pure virtual, do nothing*/ }

void Event::vOnMButtonUp(int nMX, int nMY) { /*Pure virtual, do nothing*/ }

void Event::vOnJoyAxis(Uint8 tWhich, Uint8 tAxis, Sint16 tValue) { /*Pure virtual, do nothing*/ }

void Event::vOnJoyButtonDown(Uint8 tWhich, Uint8 tButton) { /*Pure virtual, do nothing*/ }

void Event::vOnJoyButtonUp(Uint8 tWhich, Uint8 tButton) { /*Pure virtual, do nothing*/ }

void Event::vOnJoyHat(Uint8 tWhich, Uint8 tHat, Uint8 value) { /*Pure virtual, do nothing*/ }

void Event::vOnJoyBall(Uint8 tWhich, Uint8 tBall, Sint16 tXRel, Sint16 tYRel) { /*Pure virtual, do nothing*/ }

void Event::vOnMinimize() { /*Pure virtual, do nothing*/ }

void Event::vOnRestore() { /*Pure virtual, do nothing*/ }

void Event::vOnResize(int nW, int nH) { /*Pure virtual, do nothing*/ }

void Event::vOnExpose() { /*Pure virtual, do nothing*/ }

void Event::vOnExit() { /*Pure virtual, do nothing*/ }

void Event::vOnUser(Uint8 tType, int nCode, void* pvData1, void* pvData2) { /*Pure virtual, do nothing*/ }

