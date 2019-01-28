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
	client.search("adsfadshsadhdafh");
	

	//Check internet connection
	if (internetConnection == NULL) 
	{
		cout << "checking!" << endl;
		internetConnection = checkInternetConnection();
		cout << internetConnection << endl;
	}

	//Load images
	quit.load("images/quit.png");
	retweet.load("images/retweet.png");
	favourite.load("images/favourite.png");
	coordinates.load("images/coordinates.png");
	language.load("images/language.png");
	url.load("images/url.png");
	hyperlink.load("images/hyperlink.png");
	reply.load("images/reply.png");
	time.load("images/time.png");
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
		
		switch (currentTab)
		{
		case HOME:
			//Draw text feed
			ofNoFill();
			ofDrawRectangle(100, 200, 1300, 650); //Feed box
			ofDrawRectangle(100, 160, 150, 40); //Username box
			ofDrawRectangle(250, 160, 750, 40); //Text box
			ofDrawRectangle(1000, 160, 50, 40); //Language box
			ofDrawRectangle(1050, 160, 100, 40); //Place box
			ofDrawRectangle(1150, 160, 50, 40); //Url box
			ofDrawRectangle(1200, 160, 50, 40); //Reply box
			ofDrawRectangle(1250, 160, 50, 40); //Retweet box
			ofDrawRectangle(1300, 160, 50, 40); //Favorite box
			ofDrawRectangle(1350, 160, 50, 40); //Timestamp box
			font.loadFont("HelveticaNeueUltraLight.ttf", 20);
			font.drawString("Username", 120, 190);
			font.drawString("Tweet", 605, 190);
			language.draw(1010, 170, 30, 20);
			coordinates.draw(1083, 165, 30, 30);
			url.draw(1155, 165, 40, 30);
			reply.draw(1210, 170, 30, 20);
			retweet.draw(1260, 160, 30, 40);
			favourite.draw(1300, 160, 50, 40);
			time.draw(1350, 160, 50, 40);

			/**********************************************************************************************/

			//Display tweets
			font.loadFont("HelveticaNeueUltraLight.ttf", 12);
			for (int i = 0; i < 10; i++)
			{
				if (i < myTweets.size())
				{
					ofSetColor(colourText);
					font.drawString(myTweets[i].text, 260, 227 + (65 * i));
					ofDrawRectangle(250, 200 + (65 * i), 750, 65);
				}
			}

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


	//Check if mouse pressed inside one of the tabs
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