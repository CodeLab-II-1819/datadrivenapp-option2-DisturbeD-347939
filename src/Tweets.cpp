#include "Tweets.h"
#include <Poco/DateTime.h>

Tweets::Tweets(double interval, double current_time, bool startCountdown, string tweet, Poco::DateTime &createdTime, string username, string displayName, string language, string url, int replies, int retweets, int likes, string place)
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
	else if (text[0] == '\x40') //Check if is a mention
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

	this->interval = interval;
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