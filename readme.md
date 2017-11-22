# MemeCrawler C++
*Note: Some features are already described but yet to be implemented*
Grabs memes of reddit and saves them locally or puts them into a database.
For each meme we download, we save their ColorMomentHash. Before a meme is 
added to our local files/database we check for reposts by comparing each 
ColorMomentHash.

Currently sources are subreddits of [reddit](https://reddit.com/) and their links are hardcoded.
Sources:
 * /r/memes
 * /r/dankmemes
 * /r/meirl
 * /r/dankchristianmemes
 * /r/bikinibottomtwitter

The crawler only grabs images from the frontpage which are usually 22-25 
images each. A filter is created for each subreddit to prevent a post from 
being added multiple times if they re-enter the frontpage multiple times.

## Getting Started

### Dependencies

Required libraries are:
 * [curlpp](http://www.curlpp.org/)
 * [openvc](https://opencv.org/)
 * [mysql](https://dev.mysql.com/downloads/connector/cpp/) 
 * [boost](http://www.boost.org/) (mysql req.)

### Start

To start the application you need to specify at least one argument.
Arguments are: 
```
   -u USER       Username used for mysql login
   -p PASSWD     Password used for mysql login
   --restart     Ignores the initial request data
   --tick SEC    sets time between requests (refreshrate)
   --local PATH  Saves memes only locally at the specified path
```

Default values are:
```
USER=root
PASSWD=root
SEC=600 (10min)
PATH=""
```

## Errors

TODO

## Issues

TODO

