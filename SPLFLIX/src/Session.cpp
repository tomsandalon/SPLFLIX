#include "../include/Session.h"
#include "../include/User.h"
#include "../include/Watchable.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "../include/json.hpp"
#include "../include/Action.h"


using namespace std;

Session::Session(const std::string& configFilePath):content({}), actionsLog({}), userMap({}), activeUser(nullptr) ,lastActionInput(""), run(false)   {
	activeUser = new LengthRecommenderUser("default");
	userMap[activeUser->getName()] = activeUser;
	setJsonData(configFilePath);
}



Session::~Session() {
	cout << "session deleteeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee" << endl;
	clean();
}
void Session::clean() {
	cout << "session delete" << endl;
	for (size_t i = 0; i < content.size(); i++){
		if (content[i] != nullptr)
			delete content[i];
	}
	for (auto it = userMap.begin(); it != userMap.end(); ++it){
		if (it->second != nullptr)
			delete it->second;
	}
	for (size_t i = 0; i < actionsLog.size(); i++){
	    if( actionsLog[i] != nullptr)
			delete actionsLog[i];
	}
}

Session::Session(const Session& other): content(other.content), actionsLog(other.actionsLog), userMap(other.userMap) , activeUser(other.activeUser), lastActionInput(other.lastActionInput),  run(other.run)   {
	copy(other);
}
Session& Session::operator=(const Session& other){
		if (&other != this) {
			clean();
				copy(other);
		}
		return *this;
	}
Session::Session(Session&& other): content(other.content), actionsLog(other.actionsLog), userMap(other.userMap) , activeUser(other.activeUser), lastActionInput(other.lastActionInput),  run(other.run)  {
	steal(other);
}
Session& Session :: operator=( Session&& other) {
	if (&other != this) {
		clean();
		steal(other);
	}
	return *this;
}
 void Session::copy(const Session& other) {	
	content =other.content;
	//for (size_t i = 0; i < content.size(); i++) {
	//	if (other.content[i]->isMovie()) {
	//		content[i] = new Movie(other.content[i]);
	//	}
	//	else {
	//		content[i] = new Episode(other.content[i]);
	//	}
	//}
	actionsLog = other.actionsLog;
	userMap = other.userMap;
	lastActionInput = other.lastActionInput;
	//activeUser = User(*other.activeUser);
}

void Session::steal(Session &other){
	activeUser = other.activeUser;
	other.activeUser = nullptr;
	content = other.content;
	actionsLog = other.actionsLog;
	userMap = other.userMap;
	lastActionInput = other.lastActionInput;
	other.content.clear();
	other.actionsLog.clear();
	other.userMap.clear();
	other.lastActionInput = "";
}











void Session::start() {
	cout << "SPLFLIX is now on!"<< endl;
	run = true;


	string userInput;

	while (run) {
		getline(cin, userInput);

		if (userInput.compare("content") == 0) {
			BaseAction* action = new PrintContentList();
			action->act(*this);
			actionsLog.push_back(action);
		}
		else if (userInput.length() >= 7 && userInput.substr(0, 6).compare("watch ") == 0) {
			lastActionInput = userInput;
			BaseAction* action = new Watch();
			action->act(*this);
			actionsLog.push_back(action);
		}
		else if (userInput.compare("log") == 0) {
			lastActionInput = userInput;
			BaseAction* action = new PrintActionsLog();
			action->act(*this);
			actionsLog.push_back(action);

		}
		else if (userInput.compare("watchhist") == 0) {
			lastActionInput = userInput;
			BaseAction* action = new PrintWatchHistory();
			action->act(*this);
			actionsLog.push_back(action);

		}
		else if (userInput.length() > 9 && userInput.substr(0, 8).compare("dupuser ") == 0) {
			lastActionInput = userInput;
			BaseAction* action = new DuplicateUser();
			actionsLog.push_back(action);
			action->act(*this);
		}
		else if (userInput.length() > 9 && userInput.substr(0, 11).compare("createuser ") == 0) {
			lastActionInput = userInput;
			BaseAction* action = new CreateUser();
			actionsLog.push_back(action);
			action->act(*this);
		}
		else if (userInput.length() > 9 && userInput.substr(0, 11).compare("changeuser ") == 0) {
			lastActionInput = userInput;
			BaseAction* action = new ChangeActiveUser();
			actionsLog.push_back(action);
			action->act(*this);
		}
		else if (userInput.length() > 9 && userInput.substr(0, 11).compare("deleteuser ") == 0) {
			lastActionInput = userInput;
			BaseAction* action = new DeleteUser();
			actionsLog.push_back(action);
			action->act(*this);
		}
		else if (userInput.length() == 4 && userInput.substr(0, 4).compare("exit") == 0) {
			lastActionInput = userInput;
			BaseAction* action = new Exit();
			action->act(*this);
			actionsLog.push_back(action);

		}
	}

}

std::vector<Watchable*> Session::get_content() const {
	return content;
}

std::string Session::getLastActionInput() const {
	return lastActionInput;
}


void Session::changeActiveUser(string user) {
	activeUser = userMap.at(user);
}

User* Session::getActiveUser() const {
	return activeUser;
}

void Session::watchRecommendationFromAction(Watchable* w) {
	lastActionInput = "watch " + to_string(w->get_id());
	Watch* nextWat = new Watch();
	nextWat->act(*this);
	actionsLog.push_back(nextWat);
}

std::vector<BaseAction*> Session::getActionLog() const {
	return actionsLog;
}



void Session::setJsonData(const std::string& configFilePath) {
	json jsonData;

	std::ifstream watchableFile(configFilePath);
	watchableFile >> jsonData;



	int id_count = 1;
	for (size_t i = 0; i < jsonData["movies"].size(); i++) {

		vector<string> vect = jsonData["movies"][i]["tags"];
		Movie* watchC = new Movie(id_count, jsonData["movies"][i]["name"], jsonData["movies"][i]["length"], vect);
		id_count++;
		content.push_back(watchC);
	}


	for (size_t j = 0; j < jsonData["tv_series"].size(); j++) {
		vector<string> vect2 = jsonData["tv_series"][j]["tags"];
		for (size_t i = 0; i < jsonData["tv_series"][j]["seasons"].size(); i++) {
			for (size_t k = 0; k < jsonData["tv_series"][j]["seasons"][i]; k++) {

				Episode* watchS = new Episode(id_count, jsonData["tv_series"][j]["name"], jsonData["tv_series"][j]["episode_length"], i + 1, k + 1, vect2);
				id_count++;
				content.push_back(watchS);

				if ((jsonData["tv_series"][j]["seasons"][i] == k + 1) && (i == jsonData["tv_series"][j]["seasons"].size() - 1)) {

					watchS->set_Next_Episode(-1);
				}
				else
					watchS->set_Next_Episode(id_count);

			}
		}
	}
};


void Session::setRunToFalse() {
	run = false;
}

bool Session::userExists(string userName) const {
	return userMap.count(userName) > 0;
}

User* Session::getUserByName(string userName) const {
	if (userExists(userName))
		return nullptr;
	else
		return userMap.at(userName);
}

void Session::addUserToMap(User* u) {
	userMap[u->getName()] = u;
}

void Session::deleteUser(string s) {
	userMap[s]->~User();
	userMap[s] = nullptr; //apply null pointer
	userMap.erase(s); //delete the slot in the hash table
}

void Session::addWatchedToUser(User* u, Watchable* w) {
	u->addWatched(w);
}

std::string Session::getUserAlgoType(string s) const {
	return userMap.at(s)->algoType();
}

std::vector<Watchable*> Session::getUserHistory(string s) const {
	return userMap.at(s)->get_history();
}