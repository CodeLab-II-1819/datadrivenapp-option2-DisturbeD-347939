#include "ofApp.h"
#include "Internet.h"
#include <cstdlib>
#include <stdlib.h>

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
	client.search("location");
	

	//Check internet connection
	if (internetConnection == NULL) 
	{
		internetConnection = checkInternetConnection();
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
	currentTab == HOME ? updateTweets = true : updateTweets = false;

	//Update twitter times
	if (updateTweets)
	{
		for (int i = 0; i < myTweets.size() && i < 10; i++)
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
}

void ofApp::draw()
{
	array<ofRectangle, 10> myURLS;
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
			font.loadFont("fonts/HelveticaNeueUltraLight.ttf", 20);
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
			font.loadFont("fonts/HelveticaNeueUltraLight.ttf", 12);
			for (int i = 0; i < 10; i++)
			{
				if (i < myTweets.size())
				{
					ofSetColor(colourText);

					//Draw tweet user
					font.drawString(myTweets[i].displayName, 110, 227 + (65 * i));
					font.drawString(myTweets[i].username, 110, 247 + (65 * i));
					ofDrawRectangle(100, 200 + (65 * i), 150, 65);

					//Draw tweet text
					font.drawString(myTweets[i].text, 260, 227 + (65 * i));
					ofDrawRectangle(250, 200 + (65 * i), 750, 65);

					//Draw tweet type
					font.drawString(myTweets[i].type, 960, 235 + (65 * i));

					//Draw tweet language
					font.drawString(myTweets[i].language, 1015, 240 + (65 * i));
					ofDrawRectangle(1000, 200 + (65 * i), 50, 65);

					//Draw tweet language
					font.drawString(myTweets[i].place, 1060, 240 + (65 * i));
					ofDrawRectangle(1050, 200 + (65 * i), 100, 65);

					//Draw url icon
					hyperlink.draw(1160, 218 + (65 * i), 30, 30);
					ofDrawRectangle(1150, 200 + (65 * i), 50, 65);
					myURLS[i].setX(1150);
					myURLS[i].setY(200 + (65 * i));
					myURLS[i].setWidth(50);
					myURLS[i].setHeight(65);
				}
			}
			
			for (int i = 0; i < 10; i++)
			{
				if (myURLS[i].inside(xMouseClick, yMouseClick))
				{
					cout << "inside" << endl;
					xMouseClick = 0;
					yMouseClick = 0;
					

					system("start");
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

	string placeName = "N/A";
	string countryCode = "";
	if (status.place() != 0) //Check if there is any location available before passing parameters
	{
		placeName = status.place()->name();
		countryCode = status.place()->countryCode();
		if (placeName.size() > 6) //If it's bigger than 6 characters cut it down to have space for the country too.
		{
			placeName.erase(6, placeName.size() - 6);
			placeName.append("..");
			placeName.append("" + countryCode);
		}
		else
		{
			placeName.append("," + countryCode);
		}
		cout << placeName << endl;
	}

	myTweets.size() >= 10 ? myTweets.push_back(Tweets(ofGetElapsedTimeMillis(), false, status.text(), status.createdAt(), status.user()->screenName(), status.user()->name(), status.language(), status.url(), status.replyCount(), status.retweetCount(), status.favoriteCount(), placeName)) : myTweets.push_back(Tweets(ofGetElapsedTimeMillis(), true, status.text(), status.createdAt(), status.user()->screenName(), status.user()->name(), status.language(), status.url(), status.replyCount(), status.retweetCount(), status.favoriteCount(), placeName));

}

//Draw navigation bar
void ofApp::drawMenu()
{
	ofSetColor(colourText);
	font.loadFont("fonts/HelveticaNeueUltraLight.ttf", 40);
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
		currentTab = HOME;
		xMouseClick = 0;
		yMouseClick = 0;
	}
	else if (Search.inside(xMouseClick, yMouseClick))
	{
		currentTab = SEARCH;
		xMouseClick = 0;
		yMouseClick = 0;
	}
	else if (Settings.inside(xMouseClick, yMouseClick))
	{
		currentTab = SETTINGS;
		xMouseClick = 0;
		yMouseClick = 0;
	}
	else if (Quit.inside(xMouseClick, yMouseClick))
	{
		currentTab = QUIT;
		xMouseClick = 0;
		yMouseClick = 0;
	}
}
//COMPLETE

//Handle errors and exceptions
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
//COMPLETE

//Mouse related functions
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
//COMPLETE

Tweets::Tweets(double current_time, bool startCountdown, string tweet, Poco::DateTime &createdTime, string username, string displayName, string language, string url, int replies, int retweets, int likes, string place)
{
	//Get username and nickname
	this->username = username;
	this->username.insert(0, "@");
	if (displayName.size() > 15) //Cut the display name to 15 characters
	{
		displayName.erase(15, displayName.size() - 15);
		displayName.append("...");
	}
	this->displayName = displayName;

	//Handle text
	this->text = tweet;
	cout << text[0] << endl;
	if (text[0] == 'R' && text[1] == 'T') //Check if is a retweet
	{
		type = "RT";
		while (text[0] != ':') //Remove retweeted from text for more text information
		{
			text.erase(0, 1);
		}
		text.erase(0, 1);
	}
	else if(text[0] == '\x40') //Check if is a mention
	{
		type = "MT";
	}
	else //If nothing above works it is a normal tweet
	{
		type = "TT";
	}
	
	bool biggerThanItShould = false; 
	if (text.size() > 85) //If tweet is bigger than it should (85 characters) turn this bool true
	{
		biggerThanItShould = true;
	}
	this->text.resize(85); //Cut the tweet to 85 characters

	size_t found;
	found = text.find("\n"); //Find new lines on tweets
	if (found != std::string::npos) //If found
	{
		while (text.size() != found)
		{
			text.erase(found, text.size() - found); //Erase everything below a new line and add warning
		}
		text.append("\n Check url to see more...");
	}

	if (text[text.size() - 2] != '.' && biggerThanItShould) //Check if has "Click url to see more", if not and text was bigger than it should, add it
	{
		text.append("...\n Check url to see more...");
	}

	//Get language
	for (int i = 0; i < language.size(); i++) //Upper case the language letters
	{
		char tempChar = toupper(language[i]);
		string tempString(1, tempChar);
		language.replace(i, 1, tempString);
	}
	this->language = language;

	//Get place
	this->place = place;

	//Get URL
	this->url = url;

	//Get replies
	this->replies = replies;

	//Get retweets
	this->retweets = retweets;

	//Get likes
	this->likes = likes;

	//Get date and time
	this->date = to_string(createdTime.day());
	this->date.append("/");
	this->date.append(to_string(createdTime.month()));
	this->date.append("/");
	this->date.append(to_string(createdTime.year()));
	this->date.append(" ");
	this->date.append(to_string(createdTime.hour()));
	this->date.append(":");
	this->date.append(to_string(createdTime.minute()));
	this->date.append(":");
	this->date.append(to_string(createdTime.second()));

	//Get time data
	this->startCountdown = startCountdown;
	this->time = current_time;
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