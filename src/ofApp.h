#pragma once

#include "ofMain.h"
#include "ofxTwitter.h"
#include "Tweets.h"
#include "Cities.h"
#include "Colours.h"
#include <fstream>

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
	bool changeTextBoxColour = false;
	bool changeDayBoxColour = false;
	bool userCanType = false;
	bool userCanTypeDay = false;
	bool userCanTypeWord = false;
	bool resetSearch = false;
	bool stop = false;
	bool changeColourText = true;
	

	//Integers
	int queuedTweets;
	int count = 0;
	int howLongAgo = 0;
	int searchType = 1;
	int citySelected = 0;

	//Doubles
	double tweetIntervalMS;

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
	vector<Cities> myCities;
	vector<string> mySearches;

	//Timers
	double updateTweetWordSearch = 0;
	int updateTweetWordSearchCount = 0;

	//Strings
	string userInput = "";
	string userInputDay = "";
	string userInputSearch = "";

	//Functions
	void searchTweet(bool archive, string text, string city, int date, int type);

	//File handling
	ifstream readFile;
	ofstream writeFile;

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
	ofImage deleteIt;

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
	ofRectangle dayBar;
	ofRectangle dayImage;
	ofRectangle mixedType;
	ofRectangle recentType;
	ofRectangle popularType;
	ofRectangle wordBar;
	ofRectangle wordButton;

	vector<ofRectangle> myCityRects;
	vector<Colours> myColours;

	//Enumerators
	enum tabs {
		HOME = 1,
		SEARCH = 2,
		SETTINGS = 3,
		QUIT = 4
	};
	tabs currentTab = HOME;

};