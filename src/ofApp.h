#pragma once

#include "ofMain.h"
#include "ofxTwitter.h"
#include "Tweets.h"

class ofApp: public ofBaseApp
{
public:
    //Declare app default functions
    void setup();
	void update();
    void draw();
    void onStatus(const ofxTwitter::Status& status);
    void onError(const ofxTwitter::Error& error);
    void onException(const std::exception& exception);
	void onMessage(const ofJson& json);
	void mouseMoved(int x, int y);
	void mousePressed(int x, int y, int button);
	void mouseDragged(int x, int y, int button);
	void keyPressed(int key);

	//Bools
	bool updateTweets = false;
	bool internetConnection;
	bool reset = false;
	bool changeTextBoxColour = false;
	bool userCanType = false;

	//Integers
	int queuedTweets;
	int count = 0;

	//Doubles
	double tweetIntervalMS = 10000;

	//Mouse
	int xMouse;
	int yMouse;
	int xMouseClick = 0;
	int yMouseClick = 0;
	int xMouseDrag = 0;
	int yMouseDrag = 0;
    
	//Declare twitter search client
    ofxTwitter::SearchClient client;

	//Tweet vectors
	vector<Tweets> myTweets;
	vector<string> myTweetWords;

	//Timers
	double updateTweetWordSearch = 0;
	int updateTweetWordSearchCount = 0;

	//Strings
	string userInput = "";

	/********************************************GUI******************************************/

	//Functions
	void drawMenu();

	//Images
	ofImage quit;
	ofImage reply;
	ofImage retweet;
	ofImage favourite;
	ofImage url;
	ofImage hyperlink;
	ofImage coordinates;
	ofImage language;
	ofImage time;
	ofImage search;

	//Colour defaults
	ofColor colourText;
	ofColor colourBackground = (255, 255, 255);

	//For fonts and sizes
	ofTrueTypeFont font;

	//Sliders
	ofRectangle timerSlider;
	ofRectangle timerBar;
	ofRectangle searchBar;
	ofRectangle searchButton;

	//Enumerators
	enum tabs {
		HOME = 1,
		SEARCH = 2,
		SETTINGS = 3,
		QUIT = 4
	};
	tabs currentTab = HOME;

};