#include "App.h"
//#include "SDL.h"
#include "Entities.h"

int main(int argc, char *argv[]) {
	App* pcApp = App::Instance();
	pcApp->Init();
	
	pcApp->Run();

	return 0;
}