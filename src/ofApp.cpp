﻿#include "ofApp.h"
#include "Internet.h"
#include "Tweets.h"
#include "Cities.h"
#include "Colours.h"

void ofApp::setup()
{
	//Default colours
	string readString;
	readFile.open("settings.txt");
	if (readFile.good())
	{
		string get;
		getline(readFile, get);
		colourText.r = stoi(get);
		getline(readFile, get);
		colourText.g = stoi(get);
		getline(readFile, get);
		colourText.b = stoi(get);
		getline(readFile, get);
		colourBackground.r = stoi(get);
		getline(readFile, get);
		colourBackground.g = stoi(get);
		getline(readFile, get);
		colourBackground.b = stoi(get);
		
		while (!readFile.eof())
		{
			readFile >> readString;
			if (readString != "")
			{
				cout << readString << endl;
				mySearches.push_back(readString);
			}
		}
		if (!mySearches.empty() && mySearches.size() >= 2)
		{
			mySearches.erase(mySearches.end() - 1);
		}
	}
	else
	{
		colourText.r = 29;
		colourText.g = 161;
		colourText.b = 242;
		colourBackground.r = 250;
		colourBackground.g = 250;
		colourBackground.b = 250;
	}
	readFile.close();

	if (mySearches.empty())
	{
		mySearches.push_back("Hello");
	}

	int r = 255;
	int g = 0;
	int b = 0;
	int state = 0;
	for (int i = 0; i < 50; i++)
	{
		myColours.push_back(Colours(r, g, b, 100 + (45 * (i % 10)), 200 + (50*(i / 10))));
		switch (state)
		{
		case 0:
			g += 36;
			if (g >= 255)
			{
				g = 255;
				state++;
			}
			break;
		case 1:
			r -= 36;
			if (r <= 0)
			{
				r = 0;
				state++;
			}
			break;
		case 2:
			b += 36;
			if (b >= 255)
			{
				b = 255;
				state++;
			}
			break;
		case 3:
			g -= 36;
			if (g <= 0)
			{
				g = 0;
				state++;
			}
			break;
		case 4:
			r += 36;
			if (r >= 255)
			{
				r = 255;
				state++;
			}
			break;
		case 5:
			b -= 36;
			if (b <= 0)
			{
				b = 0;
				state++;
			}
		default:
			break;
		}
	}
	myColours.push_back(Colours(0, 0, 0, 100, 450));
	myColours.push_back(Colours(255, 255, 255, 145, 450));

	//Add cities - MAXIMUM 10 CITIES FOR DESIGN PURPOSES
	myCities.push_back(Cities("World", 0, 0));
	myCities.push_back(Cities("Bath", 51.37497, -2.37297));
	myCities.push_back(Cities("London", 51.51770, -0.11352));
	myCities.push_back(Cities("Amsterdam", 52.36017, 4.89579));
	myCities.push_back(Cities("Porto", 41.15772, -8.61112));

	//Initial settings
	ofSetBackgroundColor(colourBackground);
	ofSetFrameRate(60);

	//Default sliders
	timerBar.set(500, 920, 450, 10);
	timerSlider.set(700, 900, 50, 50);

	//Default rectangles
	searchBar.set(50, 210, 400, 50);
	searchButton.set(450, 210, 50, 50);
	dayBar.set(500, 210, 50, 50);
	dayImage.set(550, 210, 50, 50);
	recentType.set(600, 210, 100, 50); 
	mixedType.set(700, 210, 100, 50);
	popularType.set(800, 210, 100, 50);
	wordBar.set(700, 200, 200, 50);
	wordButton.set(900, 200, 50, 50);
	for (int i = 0; i < myCities.size(); i++)
	{
		myCityRects.push_back(ofRectangle(1350, 300 + (i * 50), 100, 50));
	}

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
	deleteIt.load("images/delete.png");

	//Input words into tweet word vector
	for (int i = 0; i < mySearches.size(); i++)
	{
		myTweetWords.push_back(mySearches[i]);
	}
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
				searchTweet(false, myTweetWords[updateTweetWordSearchCount], "", 0, searchType);
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
	ofSetBackgroundColor(colourBackground);
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
				font.loadFont("fonts/HelveticaNeueUltraLight.ttf", 15);
				font.drawString(userInput, 60, 240);
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

			//Draw day box
			if (!changeDayBoxColour)
			{
				ofSetColor(colourText);
				ofNoFill();
				ofDrawRectangle(dayBar); //Search box
				font.loadFont("fonts/HelveticaNeueUltraLight.ttf", 15);
				font.drawString(userInputDay, 510, 240);
				ofDrawRectangle(dayImage);
				time.draw(550, 220, 50, 30);
			}
			else
			{
				ofSetColor(colourText);
				ofFill();
				ofDrawRectangle(dayBar); //Search box
				ofDrawRectangle(dayImage);
				ofSetColor(colourBackground);
				font.loadFont("fonts/HelveticaNeueUltraLight.ttf", 15);
				font.drawString(userInputDay, 510, 240);
				time.draw(550, 220, 50, 30);
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

			//Display cities
			for (int i = 0; i < myCityRects.size(); i++)
			{
				if (myCityRects[i].inside(xMouseClick, yMouseClick))
				{
					citySelected = i;
				}

				if (citySelected == i)
				{
					ofSetColor(colourText);
					ofFill();
					ofDrawRectangle(myCityRects[i]);
					ofSetColor(colourBackground);
					font.drawString(myCities[i].name, myCityRects[i].getX() + 5, myCityRects[i].getY() + 30);
				}
				else
				{
					ofSetColor(colourText);
					ofNoFill();
					ofDrawRectangle(myCityRects[i]);
					font.drawString(myCities[i].name, myCityRects[i].getX() + 5, myCityRects[i].getY() + 30);
				}
			}

			//Display search types
			font.loadFont("fonts/HelveticaNeueUltraLight.ttf", 12);
			if (searchType == 0)
			{
				ofSetColor(colourText);
				ofFill();
				ofDrawRectangle(recentType);
				ofSetColor(colourBackground);
			}
			else
			{
				ofSetColor(colourText);
				ofNoFill();
				ofDrawRectangle(recentType);
			}
			;
			font.drawString("Recent", recentType.getX() + 5, recentType.getY() + 40);

			if (searchType == 1)
			{
				ofSetColor(colourText);
				ofFill();
				ofDrawRectangle(mixedType);
				ofSetColor(colourBackground);
			}
			else
			{
				ofSetColor(colourText);
				ofNoFill();
				ofDrawRectangle(mixedType);
			}
			font.drawString("Mixed", mixedType.getX() + 5, mixedType.getY() + 40);

			if (searchType == 2)
			{
				ofSetColor(colourText);
				ofFill(); 
				ofDrawRectangle(popularType);
				ofSetColor(colourBackground);
			}
			else
			{
				ofSetColor(colourText);
				ofNoFill();
				ofDrawRectangle(popularType);
			}
			font.drawString("Popular", popularType.getX() + 5, popularType.getY() + 40);
			
			

			break;
		}
		case SETTINGS:
		{
			ofNoFill();
			ofSetColor(colourText);
			font.loadFont("fonts/HelveticaNeueUltraLight.ttf", 12);
			font.drawString("Add or remove more live search terms!", 700, 300);
			ofRectangle text(100, 100, 100, 50);
			ofRectangle background(200, 100, 100, 50);

			if (changeColourText)
			{
				ofSetColor(colourText);
				ofFill();
				ofDrawRectangle(text);
				ofSetColor(colourBackground);
				font.drawString("Text", text.getX() + 5, text.getY() + 40);
				ofNoFill();
				ofSetColor(colourText);
				ofDrawRectangle(background);
				font.drawString("Background", background.getX() + 5, background.getY() + 40);
			}
			else
			{
				ofSetColor(colourText);
				ofNoFill();
				ofDrawRectangle(text);
				font.drawString("Text", text.getX() + 5, text.getY() + 40);
				ofFill();
				ofSetColor(colourText);
				ofDrawRectangle(background);
				ofSetColor(colourBackground);
				font.drawString("Background", background.getX() + 5, background.getY() + 40);
			}

			ofSetColor(colourText);
			ofNoFill();
			array<ofRectangle, 10> myDeletes;
			int whichDelete = 11;
			for (int i = 0; i < myTweetWords.size(); i++)
			{
				ofDrawRectangle(1000, 200 + (i*50), 100, 50);
				font.drawString(myTweetWords[i], 1005, 240 + (i * 50));
				myDeletes[i].setX(1100);
				myDeletes[i].setY(200 + (i * 50));
				myDeletes[i].setWidth(50);
				myDeletes[i].setHeight(50);
				ofDrawRectangle(myDeletes[i]);
				deleteIt.draw(1100, 200 + (i * 50), 50, 50);
				if (myDeletes[i].inside(xMouseClick, yMouseClick))
				{
					whichDelete = i;
					xMouseClick = 0;
					yMouseClick = 0;
				}
			}
			if (whichDelete != 11)
			{
				myTweetWords.erase(myTweetWords.begin() + whichDelete);
				mySearches.erase(mySearches.begin() + whichDelete);
			}

			
			if (userCanTypeWord)
			{
				ofFill();
			}
			else
			{
				ofNoFill();
			}
			ofSetColor(colourText);
			ofDrawRectangle(wordBar);
			if (userCanTypeWord)
			{
				ofSetColor(colourBackground);
			}
			else
			{
				ofSetColor(colourText);
			}
			font.drawString(userInputSearch, wordBar.getX() + 5, wordBar.getY() + 40);
			ofNoFill();
			ofSetColor(colourText);
			ofDrawRectangle(wordButton);
			search.draw(wordButton.getX()+5, wordButton.getY()+5, 40, 40);

			if (text.inside(xMouseClick, yMouseClick))
			{
				changeColourText = true;
			}
			if (background.inside(xMouseClick, yMouseClick))
			{
				changeColourText = false;
			}

			
			for (int i = 0; i < myColours.size(); i++)
			{	
				ofFill();
				ofSetColor(myColours[i].myColour);
				ofDrawRectangle(myColours[i].myShape);
				ofSetColor(0, 0, 0);
				ofNoFill();
				ofDrawRectangle(myColours[i].myShape);
			}

			break;
		}
		case QUIT:
		{
			writeFile.open("settings.txt");
			if (writeFile.good())
			{
				writeFile << to_string(colourText.r) << endl;
				writeFile << to_string(colourText.g) << endl;
				writeFile << to_string(colourText.b) << endl;
				writeFile << to_string(colourBackground.r) << endl;
				writeFile << to_string(colourBackground.g) << endl;
				writeFile << to_string(colourBackground.b) << endl;
				for (int i = 0; i < mySearches.size(); i++)
				{
					writeFile << mySearches[i] << endl;
				}
			}
			writeFile.close();
			
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

void ofApp::searchTweet(bool archive, string text, string city, int date, int type)
{
	if (archive)
	{
		ofxTwitter::SearchQuery query(text);


		//CHECK FOR POPULARITY
		if (type == 0)
		{
			query.setResultType(ofxTwitter::SearchQuery::ResultType::MIXED);
		}
		else if (type == 1)
		{
			query.setResultType(ofxTwitter::SearchQuery::ResultType::RECENT);
		}
		else if (type == 2)
		{
			query.setResultType(ofxTwitter::SearchQuery::ResultType::POPULAR);
		}

		//CHECK FOR DATE
		if (date != 0)
		{
			int day = ofGetDay();
			int month = ofGetMonth();
			int year = ofGetYear();
			for (int i = 0; i < date; i++)
			{
				day -= 1;
				if (day == 0)
				{
					switch (month)
					{
					case 1:
						day = 31;
						month--;
						year--;
						break;
					case 2:
						day = 31;
						month--;
						break;
					case 3:
						day = 28;
						month--;
						break;
					case 4:
						day = 31;
						month--;
						break;
					case 5:
						day = 30;
						month--;
						break;
					case 6:
						day = 31;
						month--;
						break;
					case 7:
						day = 30;
						month--;
						break;
					case 8:
						day = 31;
						month--;
						break;
					case 9:
						day = 31;
						month--;
						break;
					case 10:
						day = 30;
						month--;
						break;
					case 11:
						day = 31;
						month--;
						break;
					case 12:
						day = 30;
						month--;
						break;
					}
				}
			}
			query.setUntil(year, month, day);
		}
		
		//CHECK FOR LOCATION
		for (int i = 1; i < myCities.size(); i++)
		{
			if (city == myCities[i].name)
			{
				query.setGeoCode(myCities[i].lat, myCities[i].lng, 10, ofxTwitter::SearchQuery::UNITS_MILES);
			}
		}

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

	if (currentTab == SEARCH)
	{
		if (recentType.inside(xMouseClick, yMouseClick))
		{
			xMouseClick = 0;
			yMouseClick = 0;
			searchType = 0;
		}
		if (mixedType.inside(xMouseClick, yMouseClick))
		{
			xMouseClick = 0;
			yMouseClick = 0;
			searchType = 1;
		}
		if (popularType.inside(xMouseClick, yMouseClick))
		{
			xMouseClick = 0;
			yMouseClick = 0;
			searchType = 2;
		}

		//Handle search input
		if (searchBar.inside(xMouseClick, yMouseClick))
		{
			changeTextBoxColour = true;
			userCanType = true;
			changeDayBoxColour = false;
			userCanTypeDay = false;
		}
		else if (searchButton.inside(xMouseClick, yMouseClick))
		{
			changeTextBoxColour = false;
			userCanType = false;
			if (userInput != "")
			{
				count = 0;
				if (userInputDay == "")
				{
					userInputDay = "0";
				}
				howLongAgo = stoi(userInputDay);
				cout << "days " << howLongAgo << endl;
				searchTweet(true, userInput, myCities[citySelected].name, howLongAgo, searchType);
				userInputDay = "";
				myTweets.clear();
				userInput = "";
			}
		}

		if (dayBar.inside(xMouseClick, yMouseClick))
		{
			changeDayBoxColour = true;
			userCanTypeDay = true;
			userCanType = false;
			changeTextBoxColour = false;
		}
		else
		{
			changeDayBoxColour = false;
			userCanTypeDay = false;
		}
	}
	

	if (currentTab == SETTINGS)
	{
		for (int i = 0; i < myColours.size(); i++)
		{
			if (myColours[i].myShape.inside(xMouseClick, yMouseClick))
			{
				if (changeColourText)
				{
					colourText = myColours[i].myColour;
				}
				else
				{
					colourBackground = myColours[i].myColour;
				}
			}
		}
		if (wordBar.inside(xMouseClick, yMouseClick))
		{
			userCanTypeWord = true;
		}
		else
		{
			userCanTypeWord = false;
		}
		if (wordButton.inside(xMouseClick, yMouseClick))
		{
			if (userInputSearch != "" && myTweetWords.size() < 10)
			{
				mySearches.push_back(userInputSearch);
				myTweetWords.push_back(userInputSearch);
				userInputSearch = "";
			}
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
	if (userCanType && userInput.size() < 25)
	{
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
	else if (userCanTypeDay && userInputDay.size() < 1)
	{
		if (key >= 48 && key <= 55 && userInputDay == "")
		{
			char convertedNumber = key;
			userInputDay += convertedNumber;
		}
	}
	else if (userCanTypeWord && userInputSearch.size() < 12)
	{
		if (key != 32)
		{
			if (GetKeyState(VK_SHIFT) == 1 || GetKeyState(VK_SHIFT) == 0)
			{
				char convertedKey = key;
				if (key == 8)
				{
					if (userInputSearch.size() != 0)
					{
						userInputSearch.erase(userInputSearch.size() - 1, 1);
					}
				}
				else
				{
					userInputSearch += convertedKey;
				}
			}
			else
			{
				if (key == 34 || key == 64)
				{
					userInputSearch += "\x40";
				}
				if (key == 35 || key == 26)
				{
					userInputSearch += "\x23";
				}
			}
		}
	}
}