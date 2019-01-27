/*
 Starter code for Data Driven App assignment CodeLab II
 Based on basic example of ofxTwitter by Christopher Baker
 <https://christopherbaker.net>
 SPDX-License-Identifier:    MIT
 */


#pragma once


#include "ofMain.h"
#include "ofxTwitter.h"

class Tweets
{
public:
	string text;
	double interval = 5000;
	double time;
	bool selfDestruction = false;
	bool startCountdown = false;
	void tweetTimer(double time);

	Tweets(double current_time, string tweet, bool startCountdown);
};

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
	void mousePressed(int x, int y);

	//Functions
	void drawMenu();

	//Images
	ofImage quit;

	int queuedTweets;
	int count = 0;

	//Mouse
	int xMouse;
	int yMouse;
	int xMouseClick = 0;
	int yMouseClick = 0;
    
	//declare twitter search client
    ofxTwitter::SearchClient client;
    
    //declare variables
	bool internetConnection;
	bool reset = false;

	//Colour defaults
	ofColor colourText;
	ofColor colourBackground = (255, 255, 255);

	//For fonts and sizes
	ofTrueTypeFont font;

	//Tweet vector
	vector<Tweets> myTweets;

};
