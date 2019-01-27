#include "ofApp.h"
#include "Internet.h"

void ofApp::setup()
{
	//Initial settings
	ofSetBackgroundColor(colourBackground);
	ofSetFrameRate(60);

	//Default colours
	colourText.r = 29;
	colourText.g = 161;
	colourText.b = 242;
	colourBackground.r = 255;
	colourBackground.g = 255;
	colourBackground.b = 255;

	//Twitter settings
	client.registerSearchEvents(this);
	client.setCredentialsFromFile("credentials.json");
	client.setPollingInterval(6000);
	client.search("a");

	//Check internet connection
	if (internetConnection == NULL) 
	{
		cout << "checking!" << endl;
		internetConnection = checkInternetConnection();
		cout << internetConnection << endl;
	}

	//Load images
	quit.load("images/quit.png");
}

void ofApp::update()
{
	//Update twitter times
	for (int i = 0; i < myTweets.size() && i < 15; i++)
	{
		myTweets[i].tweetTimer(ofGetElapsedTimeMillis());
		if (myTweets[i].selfDestruction)
		{
			myTweets.erase(myTweets.begin() + i);
			i--;
		}
	}
	queuedTweets = myTweets.size() - 15;

	//Check if mouse pressed
}

void ofApp::draw()
{
	if (!internetConnection)
	{
		ofSetBackgroundColor(0);
		ofSetColor(colourText);
		ofDrawBitmapString("No internet connection...", ofGetScreenWidth() / 2, ofGetScreenHeight() / 2);
	}
	else
	{
		ofDrawBitmapString(xMouse, xMouse + 15, yMouse - 30); //REMOVEEEEE
		ofDrawBitmapString(yMouse, xMouse + 15, yMouse - 20); //REMOVEEEEE
		drawMenu();

		ofRectangle feedMenu (200, 160, 1100, 40);
		
		switch (currentTab)
		{
		case HOME:
			ofNoFill();
			ofDrawRectangle(200, 200, 1100, 750);
			ofDrawRectangle(feedMenu);
			break;
		case SEARCH:
			break;
		case SETTINGS:
			break;
		case QUIT:
			ofExit();
			break;
		default:
			break;
		}
		
	}
}

//This function is called everytime a new tweet is received
void ofApp::onStatus(const ofxTwitter::Status& status)
{
	count++;
	if (myTweets.size() >= 10)
	{
		myTweets.push_back(Tweets(ofGetElapsedTimeMillis(), status.text(), false));
	}
	else
	{
		myTweets.push_back(Tweets(ofGetElapsedTimeMillis(), status.text(), true));
	}
	//ofLogNotice("ofApp::onStatus") << "Text: " << status.text();
	//ofLogNotice("ofApp::onStatus") << "\tPlace: " << (status.place() ? ofToString(status.place()->fullName()) : "NONE");
}

//DrawMenu
void ofApp::drawMenu()
{
	ofSetColor(colourText);
	font.loadFont("HelveticaNeueUltraLight.ttf", 40);
	ofNoFill();
	ofDrawRectangle(1, 1, 1500, 60);
	ofDrawRectangle(1, 1, 1498, 998);

	//Home tab
	ofRectangle Home(1, 1, 175, 60);
	if (Home.inside(xMouse, yMouse)) //Change colour on mouse hover
	{
		ofFill();
		ofDrawRectangle(Home);
		ofSetColor(colourBackground);
		font.drawString("Home", 20, 50);
		ofSetColor(colourText);
	}
	else
	{
		ofNoFill();
		ofDrawRectangle(Home);
		ofSetColor(colourText);
		font.drawString("Home", 20, 50);
	}

	//Search tab
	ofRectangle Search(176, 1, 200, 60);
	if (Search.inside(xMouse, yMouse)) //Change colour on mouse hover
	{
		ofFill();
		ofDrawRectangle(Search);
		ofSetColor(colourBackground);
		font.drawString("Search", 200, 50);
		ofSetColor(colourText);
	}
	else
	{
		ofNoFill();
		ofDrawRectangle(Search);
		ofSetColor(colourText);
		font.drawString("Search", 200, 50);
	}

	//Settings tab
	ofRectangle Settings(376, 1, 220, 60);
	if (Settings.inside(xMouse, yMouse)) //Change colour on mouse hover
	{
		ofFill();
		ofDrawRectangle(Settings);
		ofSetColor(colourBackground);
		font.drawString("Settings", 400, 50);
		ofSetColor(colourText);
	}
	else
	{
		ofNoFill();
		ofDrawRectangle(Settings);
		ofSetColor(colourText);
		font.drawString("Settings", 400, 50);
	}

	//Quit tab
	ofRectangle Quit(1420, 1, 79, 60);
	if (Quit.inside(xMouse, yMouse)) //Change colour on mouse hover
	{
		ofFill();
		ofDrawRectangle(Quit);
		ofSetColor(colourBackground);
		quit.draw(1440, 11, 39, 40);
		ofSetColor(colourText);
	}
	else
	{
		ofNoFill();
		ofDrawRectangle(Quit);
		ofSetColor(colourText);
		quit.draw(1440, 11, 39, 40);
	}

	if (Home.inside(xMouseClick, yMouseClick))
	{
		cout << "Inside home" << endl;
		currentTab = HOME;
		xMouseClick = 0;
		yMouseClick = 0;
	}
	else if (Search.inside(xMouseClick, yMouseClick))
	{
		cout << "Inside search" << endl;
		currentTab = SEARCH;
		xMouseClick = 0;
		yMouseClick = 0;
	}
	else if (Settings.inside(xMouseClick, yMouseClick))
	{
		cout << "Inside settings" << endl;
		currentTab = SETTINGS;
		xMouseClick = 0;
		yMouseClick = 0;
	}
	else if (Quit.inside(xMouseClick, yMouseClick))
	{
		cout << "Inside quit" << endl;
		currentTab = QUIT;
		xMouseClick = 0;
		yMouseClick = 0;
	}
}

void ofApp::onError(const ofxTwitter::Error& error)
{
	ofLogError("ofApp::onError") << "Error: " << error.code() << " " << error.message();
}
void ofApp::onMessage(const ofJson& json)
{
	// This is the raw message json and is ignored here.
}
void ofApp::onException(const std::exception& notice)
{
	ofLogError("ofApp::onException") << "Exception: " << notice.what();
}

void ofApp::mousePressed(int x, int y, int button)
{
	xMouseClick = x;
	yMouseClick = y;
	cout << xMouseClick << " " << yMouseClick << endl;
}
void ofApp::mouseMoved(int x, int y)
{
	xMouse = x;
	yMouse = y;
}

Tweets::Tweets(double time, string tweet, bool startCountdown)
{
	this->startCountdown = startCountdown;
	this->text = tweet;
	this->time = time;
}
void Tweets::tweetTimer(double current_time)
{
	if (startCountdown)
	{
		if ((current_time - time) > interval)
		{
			selfDestruction = true;
		}
	}
	else
	{
		time = current_time;
		startCountdown = true;
	}
}


