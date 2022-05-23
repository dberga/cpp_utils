#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"

int main() {

    ofAppGlutWindow window;
	ofSetupOpenGL(&window, 600, 800, OF_WINDOW);
	ofRunApp(new testApp());
    //1024x768
}
