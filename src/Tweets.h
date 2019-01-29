#pragma once

#include "ofMain.h"

class Tweets
{
public:
	//Information variables
	string displayName; //Get nickname
	string username; //Get account name
	string text; //Get tweet text
	string language; //Get language
	string place; //Get place
	string url; //Get url
	string replies; //Get replies
	string retweets; //Get retweets
	string likes; //Get likes
	string date; //Get date
	string day; //Get day
	string month; //Get month
	string year; //Get year
	string hour; //Get hour
	string minute; //Get minute
	string second; //Get second
	string type; //Get tweet type

	//Functions
	string shortNumber(int number);
	void tweetTimer(double time);

	//Time variables
	double interval = 10000; //Duration before disappearing
	double time;
	bool selfDestruction = false;
	bool startCountdown = false;

	//Constructor
	Tweets(double interval, double current_time, bool startCountdown, string tweet, Poco::DateTime &createdTime, string username, string displayName, string language, string url, int replies, int retweets, int likes, string place);
};