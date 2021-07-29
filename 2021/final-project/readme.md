# Gmail Subaddresses

Python script using Gmail API to list subaddresses that are still in your inbox.

## Description

Gmail and other email clients have the ability to use the '+' sign to create different addresses that all lead to the same email.
E.g. myemail@gmail.com and myemail+potentialSpamSite@gmail.com lead to the same email: myemail@gmail.com.
Similarly, any number of '.' characters can be inserted between letters.
E.g. my.email.gmail.com, m.y.e.m.a.i.l@gmail.com both lead to the same email: myemail@gmail.com

Using these are great for creating filters so you can send email directly to trash, or to see who may have sold your information to a third party.
However, if you want to check all of these potential subaddresses for password/email breaches (e.g. using haveibeenpwned.com) it can be difficult unless you keep a list
of all of your subaddresses. Due to limitations with the HIBP API, this script will only print out a list of emails, and you will need to check them manually on the site.

## Getting Started

### Dependencies

* Python 3.0+
  * I have not tested on all versions and use different print methods for learning.
* The [pip](https://pypi.python.org/pypi/pip) package management tool
* A Google Cloud Platform project with the gmail readonly API enabled.
  * You can follow the [Gmail API Python Quickstart](https://developers.google.com/gmail/api/quickstart/python) to create the python app.
* Authorization credentials for a desktop application. To learn how to create credentials for a desktop application, refer to [Create credentials](https://developers.google.com/workspace/guides/create-credentials).
* A Google account with Gmail enabled.

Note: some (most) of these steps are copied directly from the Gmail API Python quickstart page

### Installing

* Download the python files from the github repository via your preferred method.
  * There's only two files at time of this writing
* You will need to create a folder within the directory of the repo install titled '.secrets' i.e. './.secrets'
  * This will be where you store your credentials.json file that is downloaded during [Create credentials](https://developers.google.com/workspace/guides/create-credentials)
  * You may need to change the ```token_fullpath ``` and ```cred_fullpath``` in ```build_gmail_service.py``` variables to ensure they are correct on your system
    * The token file will be created after the Google OAuth flow

### Executing program

* Open a terminal
* Execute the main file:
```
python gmail_subaddresses.py
```

* Command line parameters:
* * Max Messages: search for only this many messages in reverse-chronological order
```
python gmail_subaddresses.py -m [max messages]
```
* * save path: save output of script to a text file
```
python gmail_subaddresses.py -s [save path]
```

## Help

* Sometimes the generated token does not properly allow for a refresh at a later time. You should be able to delete the token.json file to re-trigger the Google OAuth flow authorization
* Due to API limits, the tested rate was ~ 100 emails/sec
```
python gmail_subaddresses.py -h
```

## Authors

Contributors names and contact info

Kedric Daly 

## Version History

* 0.1
    * Initial Release

## License

This project is licensed under the [NAME HERE] License - see the LICENSE.md file for details

## Acknowledgments

Inspiration, code snippets, etc.
* [DomPizzie](https://gist.github.com/DomPizzie/7a5ff55ffa9081f2de27c315f5018afc) (readme)
* [Google Gmail API python quickstart](https://developers.google.com/gmail/api/quickstart/python)
