#include "App.h"
//#include "SDL.h"
#include "Entities.h"

int main(int argc, char *argv[]) {
	App* pcApp = App::Instance(1280, 720);
	pcApp->Init();
	
	pcApp->Run();

	return 0;
}