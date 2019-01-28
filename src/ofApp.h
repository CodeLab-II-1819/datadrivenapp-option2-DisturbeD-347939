#pragma once

#include "ofMain.h"
#include "ofxTwitter.h"

class Tweets
{
public:
	string displayName; //Get nickname
	string username; //Get account name
	string text; //Get tweet text
	string language;
	string place;
	string url;
	string replies;
	string retweets;
	string likes;
	string date; //Get date
	string type;

	double interval = 10000; //Duration before disappearing
	double time;
	bool selfDestruction = false;
	bool startCountdown = false;
	void tweetTimer(double time);

	Tweets(double current_time, bool startCountdown, string tweet, Poco::DateTime &createdTime, string username, string displayName, string language, string url, int replies, int retweets, int likes, string place);
};

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

	//Bools
	bool updateTweets = false;
	bool internetConnection;
	bool reset = false;

	//Integers
	int queuedTweets;
	int count = 0;

	//Mouse
	int xMouse;
	int yMouse;
	int xMouseClick = 0;
	int yMouseClick = 0;
    
	//Declare twitter search client
    ofxTwitter::SearchClient client;

	//Tweet vector
	vector<Tweets> myTweets;

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

	//Colour defaults
	ofColor colourText;
	ofColor colourBackground = (255, 255, 255);

	//For fonts and sizes
	ofTrueTypeFont font;

	//Enumerators
	enum tabs {
		HOME = 1,
		SEARCH = 2,
		SETTINGS = 3,
		QUIT = 4
	};
	tabs currentTab = HOME;

};