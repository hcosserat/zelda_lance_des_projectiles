#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main() {
    // Utiliser ofGLFWWindowSettings pour des options (ex. plein écran multi-écrans)
    ofGLWindowSettings settings;
    settings.setSize(1024, 768);
    settings.windowMode = OF_WINDOW; // peut être OF_FULLSCREEN


    auto window = ofCreateWindow(settings);

    ofRunApp(window, std::make_shared<ofApp>());
    ofRunMainLoop();
}
