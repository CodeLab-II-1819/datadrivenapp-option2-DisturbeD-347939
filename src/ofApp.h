/*
 Starter code for Data Driven App assignment CodeLab II
 Based on basic example of ofxTwitter by Christopher Baker
 <https://christopherbaker.net>
 SPDX-License-Identifier:    MIT
 */


#pragma once


#include "ofMain.h"
#include "ofxTwitter.h"


class ofApp: public ofBaseApp
{
public:
    //declare app functions
    void setup();
	void update();
    void draw();
    void onStatus(const ofxTwitter::Status& status);
    void onError(const ofxTwitter::Error& error);
    void onException(const std::exception& exception);
	void onMessage(const ofJson& json);
	void mouseMoved(int x, int y);

	void varSetup();
    
	//declare twitter search client
    ofxTwitter::SearchClient client;
    
    //declare variables
	bool internetConnection;
	bool reset = false;

};
