# jadeocr-next

> Elegant spaced-repetition flashcards with handwriting for learning languages

An improved version of [jadeocr](https://jadeocr.com), built with Vue3 and TypeScript


## Stack
* Vue3 + TypeScript
* Node.js + Express


## Quickstart
First clone this repo, then run the following to quickly spin up a dev instance
### Client
```bash
cd client
yarn && yarn twbuild
yarn serve # Starts dev server on http://localhost:8080
```
### Server
First set up a MongoDB server (locally or on [MongoDB Atlas](https://www.mongodb.com/cloud/atlas)) then add the following to **server/.env**
```bash
MONGOOSEURL="my_mongoose_url_with_key"
```
On the server, make sure you have Python3 and [TensorFlow Lite](https://www.tensorflow.org/lite/guide/python) installed. Also make sure that you have installed `virtualenv` (`pip install virtualenv`) to isolate dependencies from the rest of your system.
Then run the following
```bash
cd server

# Python setup
virtualenv venv # Creates python virtual environment
source venv/bin/activate # Activates virtual environment (run each time before working with the server)
pip install --upgrade pip # Updates pip
pip install -r requirements.txt # Installs dependencies

# Start up node.js server
cd server
yarn && yarn dev # Starts dev server on http://localhost:3000

deactivate # Deactivates virtual environment (run this when you're done)
```


## Contribute
Thank you for reading this far and considering contributing! Contributions are very much appreciated, and they are what makes this project what it is! We are looking for contributions of features/feature requests, bug fixes, documentation fixes, and anything else you can think of! The best contributions are those that will add value to the user experience. If you need to get in touch, please see the [contact/help](#Contact/Help) section.

### Issues
Browse through the [issues](https://github.com/TanayB11/jadeocr-next/issues) or submit one. Here are a couple guidelines to follow:
* Make sure all of your dependencies are up to date
* Include steps to reproduce the issue
* Expected behavior and what went wrong
* Screenshots/terminal output if necessary
### Pull Requests
Pull requests are also always welcome. Here are a couple simple guidelines:
* Make sure your code is readable and commented when necessary
* Document your changes adequately when opening a pull request

Thank you so much for taking the time to contribute!


## Contact/Help
If you would like to get in touch with me for any legitimate reason, please send me an <a href='mailto: devs@jadeocr.com'>email</a>! 


## License
This repository is licensed under the MIT License.

[Tanay Biradar](https://github.com/TanayB11)  
[Jeremy](https://github.com/wizard1238)
