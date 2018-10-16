# CodeLab II (CCO5000-20)
## Data Driven Application Option 2 - Assignment 2

### Task

For this assignment you are asked to develop an application that interrogates tweets. The application should query realtime tweets using the ofxTwitter addon for the openFrameworks  C++ toolkit. The application should be designed to use functions, and pass arguments between these as appropriate. Where possible the application should also implement object oriented programming techniques. Your application should provide a minimum of 10 different queries via a menu. Sample queries may include but are not limited to: 

* Display tweets from a certain date (limited to last 7 days with standard search API)
* Display series of tweets from a certain hashtag
* Display series of tweets mentioning a specific user
* Display tweets from a certain location
* Display tweets mentioning Mentioning “Bath Spa University” from within 10 miles of Bath
* Display most popular tweets mentioning “Donald Trump”
* Allow free search queries

You should aim to include variety in your search queries, by refining the searches with the parameters available via the twitter <a href="https://developer.twitter.com/en/docs/tweets/search/api-reference/get-search-tweets">search API</a> and <a href="https://developer.twitter.com/en/docs/tweets/rules-and-filtering/overview/standard-operators">tweet filters</a>. Exploration of the API, the sample files included in the Twitter Addon and the the header files of the ofxTwitter addon will aid your in enhancing your app.

The header files can be found in: *openFrameworks/addons/ofxTwitter/libs/ofxTwitter/include/ofx/Twitter*

The program should be designed to use functions and pass arguments between these as appropriate. You should also aim to make use of Object Oriented Programming techniques. 

The final application can either query and display tweets via the console or via a GUI window. 

### Guides

A series of guides are provided in the guides folder. These include guides on setting up a Twitter App to get your API credentials, information on setting up openFrameworks for the Data Driven Application and creating a base application from the starter code provided. The full assessment brief is also provided here for reference. 

### Starting Code

Your github repository for the assignment contains starter code to begin the task. This code should provide the initial setup in terms of connecting to Twitter. In order to get this starter code up and running clone this repository to the *myApps* folder of your openFrameworks installation. Then import this folder via the project generator (ensuring you include the required addons) to create the required files for your platform (Windows/Mac). 

You will then need a Twitter account and a registered Twitter application to generate the credentials keys to access the Twitter API:

* Consumer Key (API Key)
* Consumer Secret (API Secret)
* Access Token
* Access Token Secret

This information can be given to ofxTwitter via a JSON file stored in your projects *bin/data* folder. A template credentials file is already located in this folder and you should replace the keys here with your own keys. To create a Twitter app:

* Create an account at [twitter.com](https://twitter.com) if do not already have one
* Create a developer account and Twitter application at [developer.twitter.com](https://developer.twitter.com)
* The required keys will be listed under the *Keys and Access Tokens* tab once generated

*For a more detailed guide on setting up your Data Driven App project refer to the guides folder.*

#### Required Addons

Your project will require the following addons:

* [ofxGeo](https://github.com/bakercp/ofxGeo) 
* [ofxHTTP](https://github.com/bakercp/ofxHTTP)
* [ofxIO](ofxIO)
* [ofxMediaType](https://github.com/bakercp/ofxMediaType)
* [ofxNetworkUtils](https://github.com/bakercp/ofxNetworkUtils)
* ofxPoco - *included with openFrameworks*
* [ofxSSLManager](https://github.com/bakercp/ofxSSLManager)
* [ofxTwitter](https://github.com/bakercp/ofxTwitter)

### Deliverables

The deliverables for the Data Driven Application are as follows:

#### The Application

The C++ code (e.g. main.cpp file) and any supplementary assets (images, sounds, fonts) required to run your data driven application. In most cases this is will be the src and data (located inside the bin) folders of your openFrameworks project. These files should be committed to your unique github repository for this assignment. 

*If you are unsure - ask!*

#### The Development Document 

Your data driven application must be accompanied by a Development Document of a minimum of 1000 words (there is no maximum word count). This development document should consist of the following elements: Abstract, Project Plan, Evidence of design, Technical Description, Testing, Critical Reflection, Appendix (see brief for full details).

The development document should be submitted via Turnitin and include the assignement coversheet as the first page. The coversheet should include a link to your Github repository. 

*Incorrect submission will result in marks being deducted.* 


### Deadline

**Thursday 31st January - 23:59** 
*Only code submitted ahead of this deadline will be marked*