#include "ofApp.h"

int main()
{
    //create small GUI window
    ofSetupOpenGL(1500, 1000, OF_WINDOW);
    //run the app from the ofApp class
    return ofRunApp(std::make_shared<ofApp>());
}