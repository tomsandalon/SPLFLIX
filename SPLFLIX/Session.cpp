
#include "Session.h"
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


		json aaa;

		std::ifstream watchableFile(configFilePath);
		watchableFile >> aaa;

		cout << aaa;

		return {};
	}