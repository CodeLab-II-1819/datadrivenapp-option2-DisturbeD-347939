#include "ofApp.h"
#include "Internet.h"
#include "Tweets.h"

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

	//Default sliders
	timerBar.set(500, 920, 450, 10);
	timerSlider.set(700, 900, 50, 50);
	searchBar.set(50, 210, 400, 50);
	searchButton.set(450, 210, 50, 50);

	//Twitter settings
	client.registerSearchEvents(this);
	client.setCredentialsFromFile("credentials.json");
	client.setPollingInterval(6000);

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
	search.load("images/magnifying glass.png");

	//Input words into tweet word vector
	myTweetWords.push_back("die");
	myTweetWords.push_back("live");
}

void ofApp::update()
{
	currentTab == HOME ? updateTweets = true : updateTweets = false;

	//Update twitter times
	if (updateTweets)
	{
		if (ofGetElapsedTimeMillis() >= updateTweetWordSearch)
		{
			updateTweetWordSearch += 6000;
			if (client.isRunning() == 0)
			{
				searchTweet(false, myTweetWords[updateTweetWordSearchCount]);
			}
			else
			{
				resetSearch = true;
				client.stop();
			}
			
			if (updateTweetWordSearchCount < myTweetWords.size() - 1)
			{
				updateTweetWordSearchCount++;
			}
			else
			{
				updateTweetWordSearchCount = 0;
			}
		}
		for (int i = 0; i < myTweets.size() && i < 10; i++)
		{
			myTweets[i].tweetTimer(ofGetElapsedTimeMillis());
			if (myTweets[i].selfDestruction) //If tweets run out of time destroy them
			{
				myTweets.erase(myTweets.begin() + i); //Erase tweet
				i--;
			}
		}
	}
	queuedTweets = myTweets.size() - 10; //Update the number of queued tweets

	float oldTweetIntervalMS = tweetIntervalMS; //Check if position changed
	tweetIntervalMS = 1000 + (42 * ((timerSlider.x + 25) - 500)); //Convert slider position into seconds.

	if (oldTweetIntervalMS != tweetIntervalMS) //If the position has changed
	{
		for (int i = 0; i < myTweets.size(); i++) //Update all the timers
		{
			myTweets[i].interval = tweetIntervalMS;
		}
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
		case HOME: //Home tab
		{

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
							font.drawString(myTweets[i].minute, 1360, 232 + (65 * i));
							font.drawString("m", 1380, 232 + (65 * i));
							font.drawString(myTweets[i].second, 1360, 244 + (65 * i));
							font.drawString("s", 1380, 244 + (65 * i));
							ofDrawRectangle(1350, 200 + (65 * i), 50, 65);
						}

						if (myURLS[i].inside(xMouseClick, yMouseClick))
						{
							xMouseClick = 0;
							yMouseClick = 0;
							ofLaunchBrowser(myTweets[i].url); //Open tweet URL
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

					//Add timer slider
					ofNoFill();
					ofDrawRectangle(timerBar);
					font.drawString(to_string(int(tweetIntervalMS)), timerSlider.x, timerSlider.y - 15);
					ofFill();
					ofDrawRectangle(timerSlider);
					if (timerSlider.inside(xMouseDrag, yMouseDrag))
					{
						if (xMouseDrag >= 500 && xMouseDrag <= 950)
						{
							timerSlider.setX(xMouseDrag - 25);
							xMouseDrag = 0;
							yMouseDrag = 0;
						}
					}
					
					break;
				}
		case SEARCH: //Search tab
		{
			
			//Draw search box
			if (!changeTextBoxColour)
			{
				ofNoFill();
				ofDrawRectangle(searchBar); //Search box
				ofDrawRectangle(searchButton); //Search button
				search.draw(450, 220, 50, 30);
			}
			else
			{
				ofFill();
				ofDrawRectangle(searchBar); //Search box
				ofDrawRectangle(searchButton); //Search button
				ofSetColor(colourBackground);
				font.loadFont("fonts/HelveticaNeueUltraLight.ttf", 15);
				font.drawString(userInput, 60, 240);
				search.draw(450, 220, 50, 30);
			}

			//Draw text feed
			ofNoFill();
			ofSetColor(colourText);
			ofDrawRectangle(50, 300, 1300, 650); //Feed box
			ofDrawRectangle(50, 260, 150, 40); //Username box
			ofDrawRectangle(200, 260, 750, 40); //Text box
			ofDrawRectangle(950, 260, 50, 40); //Language box
			ofDrawRectangle(1000, 260, 100, 40); //Place box
			ofDrawRectangle(1100, 260, 50, 40); //Url box
			ofDrawRectangle(1150, 260, 50, 40); //Reply box
			ofDrawRectangle(1200, 260, 50, 40); //Retweet box
			ofDrawRectangle(1250, 260, 50, 40); //Favorite box
			ofDrawRectangle(1300, 260, 50, 40); //Timestamp box
			font.loadFont("fonts/HelveticaNeueUltraLight.ttf", 20);
			font.drawString("Username", 70, 290);
			font.drawString("Tweet", 555, 290);
			language.draw(960, 270, 30, 20);
			coordinates.draw(1033, 265, 30, 30);
			url.draw(1105, 265, 40, 30);
			reply.draw(1160, 270, 30, 20);
			retweet.draw(1210, 260, 30, 40);
			favourite.draw(1250, 260, 50, 40);
			time.draw(1300, 260, 50, 40);

			//Display tweets
			font.loadFont("fonts/HelveticaNeueUltraLight.ttf", 12);
			for (int i = 0; i < 10; i++)
			{
				if (i < myTweets.size())
				{
					ofSetColor(colourText);
					font.loadFont("fonts/HelveticaNeueUltraLight.ttf", 12);

					//Draw tweet user
					font.drawString(myTweets[i].displayName, 60, 327 + (65 * i));
					font.drawString(myTweets[i].username, 60, 347 + (65 * i));
					ofDrawRectangle(50, 300 + (65 * i), 150, 65);

					//Draw tweet text
					//if(i % 0 == 0)
					font.drawString(myTweets[i].text, 210, 327 + (65 * i));
					ofDrawRectangle(200, 300 + (65 * i), 750, 65);

					//Draw tweet type
					font.drawString(myTweets[i].type, 910, 335 + (65 * i));

					//Draw tweet language
					font.drawString(myTweets[i].language, 965, 340 + (65 * i));
					ofDrawRectangle(950, 300 + (65 * i), 50, 65);

					//Draw tweet location
					font.drawString(myTweets[i].place, 1010, 340 + (65 * i));
					ofDrawRectangle(1000, 300 + (65 * i), 100, 65);

					//Draw url icon
					hyperlink.draw(1110, 318 + (65 * i), 30, 30);
					ofDrawRectangle(1100, 300 + (65 * i), 50, 65);
					myURLS[i].setX(1100);
					myURLS[i].setY(300 + (65 * i));
					myURLS[i].setWidth(50);
					myURLS[i].setHeight(65);

					//Draw tweet replies
					font.drawString(myTweets[i].replies, 1160, 340 + (65 * i));
					ofDrawRectangle(1150, 300 + (65 * i), 50, 65);

					//Draw tweet retweets
					font.drawString(myTweets[i].retweets, 1210, 340 + (65 * i));
					ofDrawRectangle(1200, 300 + (65 * i), 50, 65);

					//Draw tweet likes
					font.drawString(myTweets[i].likes, 1260, 340 + (65 * i));
					ofDrawRectangle(1250, 300 + (65 * i), 50, 65);

					//Draw tweet time
					font.loadFont("fonts/HelveticaNeueUltraLight.ttf", 10);
					font.drawString(myTweets[i].hour, 1310, 320 + (65 * i));
					font.drawString("h", 1330, 320 + (65 * i));
					font.drawString(myTweets[i].minute, 1310, 332 + (65 * i));
					font.drawString("m", 1330, 332 + (65 * i));
					font.drawString(myTweets[i].second, 1310, 344 + (65 * i));
					font.drawString("s", 1330, 344 + (65 * i));
					ofDrawRectangle(1300, 300 + (65 * i), 50, 65);

					//Print queued tweets
					if (queuedTweets > 0)
					{
						font.loadFont("fonts/HelveticaNeueUltraLight.ttf", 10);
						font.drawString("+", 55, 965);
						font.drawString(to_string(queuedTweets), 62, 965);
						if (queuedTweets <= 9)
						{
							font.drawString(" tweets queued!", 62 + 10, 965);
						}
						else if (queuedTweets >= 10 && queuedTweets <= 99)
						{
							font.drawString(" tweets queued!", 62 + 20, 965);
						}
						else if (queuedTweets >= 100 && queuedTweets <= 999)
						{
							font.drawString(" tweets queued!", 62 + 30, 965);
						}
						else if (queuedTweets >= 1000 && queuedTweets <= 9999)
						{
							font.drawString(" tweets queued!", 62 + 40, 965);
						}
						else
						{
							font.drawString(" tweets queued!", 62 + 50, 965);
						}
					}

					if (myURLS[i].inside(xMouseClick, yMouseClick))
					{
						xMouseClick = 0;
						yMouseClick = 0;
						ofLaunchBrowser(myTweets[i].url); //Open tweet URL
					}

				}
			}


			break;
		}
		case SETTINGS:
		{
			break;
		}
		case QUIT:
		{
			ofExit();
			break;
		}
		default:
			break;
		}
		
	}
}

//This function is called everytime a new tweet is received
void ofApp::onStatus(const ofxTwitter::Status& status)
{
	if (stop)
	{
		client.reset();
		stop = false;
	}

	if (resetSearch)
	{
		client.stop();
		resetSearch = false;
	}

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
	myTweets.size() >= 10 ? myTweets.push_back(Tweets(tweetIntervalMS, ofGetElapsedTimeMillis(), false, status.text(), status.createdAt(), status.user()->screenName(), status.user()->name(), status.language(), status.url(), status.replyCount(), status.retweetCount(), status.favoriteCount(), placeName)) : myTweets.push_back(Tweets(tweetIntervalMS, ofGetElapsedTimeMillis(), true, status.text(), status.createdAt(), status.user()->screenName(), status.user()->name(), status.language(), status.url(), status.replyCount(), status.retweetCount(), status.favoriteCount(), placeName));
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
		myTweets.clear();
		client.search("");
		resetSearch = true;
		stop = true;
		xMouseClick = 0;
		yMouseClick = 0;
	}
	else if (Search.inside(xMouseClick, yMouseClick))
	{
		currentTab = SEARCH;
		myTweets.clear();
		queuedTweets = 0;
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

void ofApp::searchTweet(bool archive, string text, string city)
{
	if (archive)
	{
		ofxTwitter::SearchQuery query(text);
		//query.setUntil(2019, 1, 29);
		if()
		query.setGeoCode(51.3811, -2.3590, 10, ofxTwitter::SearchQuery::UNITS_MILES);
		query.setCount(10);
		client.search(query);
		resetSearch = true;
	}
	else
	{
		client.search(text);
		resetSearch = true;
		
	}
}

//Handle errors and exceptions
void ofApp::onError(const ofxTwitter::Error& error)
{
	//cout << "onError" << endl;
}
void ofApp::onMessage(const ofJson& json)
{
	// This is the raw message json and is ignored here.
	//cout << "onMessage" << endl;
}
void ofApp::onException(const std::exception& notice)
{
	//cout << "Exception" << endl;
}
//COMPLETE

//Mouse related functions
void ofApp::mouseDragged(int x, int y, int button)
{
	xMouseDrag = x;
	yMouseDrag = y;
	cout << "D - " << x << " " << y << endl;
}
void ofApp::mousePressed(int x, int y, int button)
{
	xMouseClick = x;
	yMouseClick = y;
	cout << xMouseClick << " " << yMouseClick << endl;

	//Handle search input
	if (searchBar.inside(xMouseClick, yMouseClick))
	{
		changeTextBoxColour = true;
		userCanType = true;
	}
	else if (searchButton.inside(xMouseClick, yMouseClick))
	{
		changeTextBoxColour = false;
		userCanType = false;
		if (userInput != "")
		{
			count = 0;
			searchTweet(true, userInput);
			myTweets.clear();
			userInput = "";
		}
	}
}
void ofApp::mouseMoved(int x, int y)
{
	xMouse = x;
	yMouse = y;
}
//COMPLETE

//Key handling
void ofApp::keyPressed(int key)
{
	if (userCanType)
	{
		cout << key << endl;
		if(GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0)
		{
			char convertedKey = key;
			if (key == 8)
			{
				if (userInput.size() != 0)
				{
					userInput.erase(userInput.size() - 1, 1);
				}
			}
			else
			{
				userInput += convertedKey;
			}
		}
		else
		{
			if (key == 34 || key == 64)
			{
				userInput += "\x40";
			}
			if (key == 35 || key == 26)
			{
				userInput += "\x23";
			}
		}
	}
}