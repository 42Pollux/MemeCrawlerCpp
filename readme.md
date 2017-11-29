# MemeCrawler C++

Grabs memes of reddit and saves them locally (full image or thumbnail) and puts them into a database.
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
   Usage: MemeCrawler [options...]
   -u USER       Username used for mysql login
   -p PASSWD     Password used for mysql login
   --restart     Ignores the initial request data
   --tick SEC    Sets time between requests (refreshrate)
   --thumbnail   Only saves a 200x200 thumbnail of each image
   --local PATH  Saves img/thumb to specified path
```

Default values are:
```
USER=root
PASSWD=root
SEC=600 (10min)
PATH="/where/your/file/is/located"
```

## Errors

TODO

## Issues

TODO

