#include "EventApp.h"

int main(int argc, char** argv) {

	App::vMakeInstance();

	try {
		App::s_pcApp->nOnRun();
	}
	catch (char* pchException) {
		std::cerr << pchException;
		assert(!pchException);
	}

	return 0;
}