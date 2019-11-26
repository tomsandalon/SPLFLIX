
#include "Session.h"
#include "User.h"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

using json = nlohmann::json;

using namespace std;

Session::Session(const std::string& configFilePath)  {
	getJsonData(configFilePath);
}



Session::~Session() {// CHECK THEAT OU
	delete activeUser;
}


void Session::start() {
	cout << "SPLFLIX is now on!";

	//and initializing a default watching user
	activeUser = &LengthRecommenderUser("default");


	/* wait for the user to enter an action to execute.
	After each executed action, it should wait for the next action.
		The loop ends when the user enters the action "exit". (See actions in part 3.4)*/
}

	std::vector<Watchable*> Session::get_content() const {
		return content; 
	}

	std::string Session::getLastActionInput() const {
		return lastActionInput;
	}

	std::unordered_map<std::string, User*> Session::getUserMap() const {
		return userMap;
	}

	void Session::changeActiveUser(User* user) {
		activeUser = user;
	}


	User* Session::getActiveUser() const{
			return activeUser;
	}

	void Session::watchRecommendationFromAction(Watchable* w) {

	}

	std::vector<BaseAction*> Session::getActionLog() const {
		return actionsLog;
	}


	json Session::getJsonData(const std::string& configFilePath) {

		
		string aaa;

		std::ifstream watchableFile(configFilePath);
		watchableFile >> aaa;

		cout << aaa;

		return {};
	}