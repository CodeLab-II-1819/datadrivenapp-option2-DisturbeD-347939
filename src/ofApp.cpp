#include "ofApp.h"
#include "Internet.h"
#include <cstdlib>
#include <stdlib.h>

void ofApp::setup()
{
	//Default colours
	colourText.r = 29;
	colourText.g = 161;
	colourText.b = 242;
	colourBackground.r = 250;
	colourBackground.g = 250;
	colourBackground.b = 250;

	//Initial settings
	ofSetBackgroundColor(colourBackground);
	ofSetFrameRate(60);

	//Twitter settings
	client.registerSearchEvents(this);
	client.setCredentialsFromFile("credentials.json");
	client.setPollingInterval(6000);
	client.search("trump");
	

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
		queuedTweets = myTweets.size() - 10;
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
					font.loadFont("fonts/HelveticaNeueUltraLight.ttf", 12);

					//Draw tweet user
					font.drawString(myTweets[i].displayName, 110, 227 + (65 * i));
					font.drawString(myTweets[i].username, 110, 247 + (65 * i));
					ofDrawRectangle(100, 200 + (65 * i), 150, 65);

					//Draw tweet text
					//if(i % 0 == 0)
					font.drawString(myTweets[i].text, 260, 227 + (65 * i));
					ofDrawRectangle(250, 200 + (65 * i), 750, 65);

					//Draw tweet type
					font.drawString(myTweets[i].type, 960, 235 + (65 * i));

					//Draw tweet language
					font.drawString(myTweets[i].language, 1015, 240 + (65 * i));
					ofDrawRectangle(1000, 200 + (65 * i), 50, 65);

					//Draw tweet location
					font.drawString(myTweets[i].place, 1060, 240 + (65 * i));
					ofDrawRectangle(1050, 200 + (65 * i), 100, 65);

					//Draw url icon
					hyperlink.draw(1160, 218 + (65 * i), 30, 30);
					ofDrawRectangle(1150, 200 + (65 * i), 50, 65);
					myURLS[i].setX(1150);
					myURLS[i].setY(200 + (65 * i));
					myURLS[i].setWidth(50);
					myURLS[i].setHeight(65);

					//Draw tweet replies
					font.drawString(myTweets[i].replies, 1210, 240 + (65 * i));
					ofDrawRectangle(1200, 200 + (65 * i), 50, 65);

					//Draw tweet retweets
					font.drawString(myTweets[i].retweets, 1260, 240 + (65 * i));
					ofDrawRectangle(1250, 200 + (65 * i), 50, 65);

					//Draw tweet likes
					font.drawString(myTweets[i].likes, 1310, 240 + (65 * i));
					ofDrawRectangle(1300, 200 + (65 * i), 50, 65);

					//Draw tweet time
					font.loadFont("fonts/HelveticaNeueUltraLight.ttf", 10);
					font.drawString(myTweets[i].hour, 1360, 220 + (65 * i));
					font.drawString("h", 1380, 220 + (65 * i));
					font.drawString(myTweets[i].minute, 1360, 230 + (65 * i));
					font.drawString("m", 1380, 230 + (65 * i));
					font.drawString(myTweets[i].second, 1360, 240 + (65 * i));
					font.drawString("s", 1380, 240 + (65 * i));
					ofDrawRectangle(1350, 200 + (65 * i), 50, 65);
				}
			}
			
			for (int i = 0; i < 10; i++)
			{
				if (myURLS[i].inside(xMouseClick, yMouseClick))
				{
					cout << "Opened tweet number " << i << endl;
					xMouseClick = 0;
					yMouseClick = 0;
					ofLaunchBrowser(myTweets[i].url);
				}
			}

			//Print queued tweets
			if (queuedTweets > 0)
			{
				font.loadFont("fonts/HelveticaNeueUltraLight.ttf", 10);
				font.drawString("+", 105, 865);
				font.drawString(to_string(queuedTweets), 112, 865);
				if (queuedTweets <= 9)
				{
					font.drawString(" tweets queued!", 112 + 10, 865);
				}
				else if (queuedTweets >= 10 && queuedTweets <= 99)
				{
					font.drawString(" tweets queued!", 112 + 20, 865);
				}
				else if (queuedTweets >= 100 && queuedTweets <= 999)
				{
					font.drawString(" tweets queued!", 112 + 30, 865);
				}
				else if (queuedTweets >= 1000 && queuedTweets <= 9999)
				{
					font.drawString(" tweets queued!", 112 + 40, 865);
				}
				else
				{
					font.drawString(" tweets queued!", 112 + 50, 865);
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
	if (text[0] == 'R' && text[1] == 'T') //Check if is a retweet
	{
		type = "RT"; //Retweet
		while (text[0] != ':') //Remove retweeted from text for more text information
		{
			text.erase(0, 1);
		}
		text.erase(0, 1);
	}
	else if(text[0] == '\x40') //Check if is a mention
	{
		type = "MT"; //Mention
	}
	else //If nothing above works it is a normal tweet
	{
		type = "TT"; //Tweet
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
	this->replies = shortNumber(replies);

	//Get retweets
	this->retweets = shortNumber(retweets);

	//Get likes
	this->likes = shortNumber(likes);

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
	this->day = to_string(createdTime.day());
	this->month = to_string(createdTime.month());
	this->year = to_string(createdTime.year());
	this->hour = to_string(createdTime.hour());
	this->minute = to_string(createdTime.minute());
	this->second = to_string(createdTime.second());

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

string Tweets::shortNumber(int number)
{
	char numberSize = 'o';
	if (number <= 0)
	{
		number = 0;
	}
	else if (number >= 10000000) //Larger than 10M
	{
		number /= 1000000;
		numberSize = 'M';
	}
	else if (number >= 1000000) //Largen than 1M
	{
		number /= 1000000;
		numberSize = 'M';
	}
	else if (number >= 100000) //Larger than 100mil
	{
		number /= 1000;
		numberSize = 'k';
	}
	else if (number >= 10000) //Larger than 10mil
	{
		number /= 1000;
		numberSize = 'k';
	}
	else if (number >= 1000) //Larger than 1k
	{
		number /= 1000;
		numberSize = 'k';
	}

	string finalizedNumber;
	finalizedNumber.append(to_string(number));
	//while (finalizedNumber[finalizedNumber.size() - 1] == '0')
	//{
	//	finalizedNumber.erase(finalizedNumber.size() - 1, 1);
	//}
	//if (finalizedNumber[finalizedNumber.size() - 1] == '.')
	//{
	//	finalizedNumber.erase(finalizedNumber.size() - 1, 1);
	//}
	//else
	//{
	//	if (numberSize == 'M' || numberSize == 'm' || numberSize == 'k')
	//	{
	//		while(finalizedNumber[finalizedNumber.size() - 1] != '.')
	//		{
	//			finalizedNumber.erase(finalizedNumber.size() - 1, 1);
	//		}
	//	}
	//	size_t foundDecimal;
	//	foundDecimal = finalizedNumber.find("."); //Find new lines on tweets
	//	if (foundDecimal != std::string::npos) //If found
	//	{
	//		int howManyDecimals;
	//		cout << finalizedNumber;
	//		howManyDecimals = finalizedNumber.size() - foundDecimal;
	//		finalizedNumber.erase(finalizedNumber.size() - 1, howManyDecimals);
	//		cout << " " << finalizedNumber << endl << endl;
	//			
	//	}
	//}
	switch (numberSize)
	{
	case 'M':
		finalizedNumber += numberSize;
		break;
	case 'm':
		finalizedNumber += numberSize;
		break;
	case 'k':
		finalizedNumber += numberSize;
		break;
	default:
		break;
	}
	return finalizedNumber;
}