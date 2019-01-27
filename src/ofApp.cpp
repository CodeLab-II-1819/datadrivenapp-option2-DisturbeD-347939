#include "ofApp.h"
#include "Internet.h"

void ofApp::setup()
{
	//Initial settings
	ofSetBackgroundColor(colourBackground);
	ofSetFrameRate(60);

	colourText.r = 29;
	colourText.g = 161;
	colourText.b = 242;

	//Twitter settings
	client.registerSearchEvents(this);
	client.setCredentialsFromFile("credentials.json");
	client.setPollingInterval(6000);
	client.search("hello");

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
		ofSetColor(colourText);
		ofDrawBitmapString(xMouse, xMouse + 15, yMouse - 30);
		ofDrawBitmapString(yMouse, xMouse + 15, yMouse - 20);
		drawMenu();
		
	/*	ofSetColor(0, 255, 0);
		ofDrawBitmapString(ofGetElapsedTimeMillis(), 10, 10);
		ofDrawBitmapString(xMouse, xMouse + 15, yMouse - 30);
		ofDrawBitmapString(yMouse, xMouse + 15, yMouse - 20);
		ofNoFill();
		ofDrawBitmapString("Recents tweets", 100, 100);
		for (int i = 1; i < myTweets.size() + 1 && i < 15; i++)
		{
			ofDrawRectangle(100, 120, 200, 50 * i);
			ofDrawBitmapString(myTweets[i - 1].time, 250, 100 + (50 * i));
			if (myTweets[i - 1].text.size() > 10)
			{
				string firstHalf;
				string secondHalf;
				for (int j = 0; j < myTweets[i - 1].text.size(); j++)
				{
					if (j <= myTweets[i - 1].text.size() / 2)
					{
						firstHalf += myTweets[i - 1].text.at(j);
						if (j == myTweets[i - 1].text.size() / 2)
						{
							if (myTweets[i - 1].text.at(j) != ' ')
							{
								if (myTweets[i - 1].text.at(j + 1) != ' ')
								{
									firstHalf += '-';
								}
							}
						}
					}
					else
					{
						secondHalf += myTweets[i - 1].text.at(j);
					}
				}
				ofDrawBitmapString(firstHalf, 150, 90 + (50 * i));
				ofDrawBitmapString(secondHalf, 150, 110 + (50 * i));
			}
			else
			{
				ofDrawBitmapString(myTweets[i - 1].text, 150, 100 + (50 * i));
			}
		}
		if (queuedTweets > 0)
		{
			int y = 172;
			ofDrawBitmapString("+", 165, 640);
			ofDrawBitmapString(queuedTweets, 172, 640);
			if (queuedTweets <= 9)
			{
				ofDrawBitmapString(" tweets queued!", 172 + 10, 640);
			}
			else if (queuedTweets >= 10 && queuedTweets <= 99)
			{
				ofDrawBitmapString(" tweets queued!", 172 + 12, 640);
			}
			else if (queuedTweets >= 100 && queuedTweets <= 999)
			{
				ofDrawBitmapString(" tweets queued!", 172 + 14, 640);
			}
			else if (queuedTweets >= 1000 && queuedTweets <= 9999)
			{
				ofDrawBitmapString(" tweets queued!", 172 + 16, 640);
			}
		}*/
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
	}
}

void ofApp::onError(const ofxTwitter::Error& error)
{

}
void ofApp::onException(const std::exception& exception)
{

}

void ofApp::mousePressed(int x, int y)
{
	xMouseClick = x;
	yMouseClick = y;
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

