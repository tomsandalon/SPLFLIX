
#include "Session.h"
#include "User.h"
#include "Watchable.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "json.hpp"
#include "Action.h"


using json = nlohmann::json;

using namespace std;

Session::Session(const std::string& configFilePath) {
	getJsonData(configFilePath);
}



Session::~Session() {// CHECK THEAT OU
	//delete activeUser;
}


void Session::start() {
	cout << "SPLFLIX is now on!";

	//and initializing a default watching user
	activeUser = &LengthRecommenderUser("default");

	/* wait for the user to enter an action to execute.
	After each executed action, it should wait for the next action.
		The loop ends when the user enters the action "exit". (See actions in part 3.4)*/

	string userInput;

	while(true) {

		DisplayMenu();

		cout << endl << "\t\t\t\tPlease select an option: ";
		

		getline(cin, userInput);

		if (userInput.compare("content") == 0) {
			PrintContentList* a = new PrintContentList();
			a->act(*this);
			a->toString();
			actionsLog.push_back(a);
			//cout << "jjjj" << actionsLog[0]->toString();
			//delete a;
			system("PAUSE");
			system("CLS");
		}
		else if (userInput.length() >= 7 && userInput.substr(0, 5).compare("watch") == 0) {
			lastActionInput = userInput;
			Watch* a = new Watch();
			a->act(*this);
			a->toString();
			//TODO: add it actionsLog
			system("PAUSE");
			system("CLS");
		}
		else if (userInput.compare("log") == 0) {
			//lastActionInput = userInput;
			//PrintActionsLog* a = new PrintActionsLog();
			//a->act(*this);
			//a->toString();
			system("PAUSE");
			system("CLS");
			
			
			
			
			;
			break;
		}
		else if (userInput.length() == 4 && userInput.substr(0, 4).compare("exit") == 0) {
			cout << "\nexit\n" << endl;
			break;
		}
		else {
			cout << "Please choose a valid option:" << endl
				<< "=============================" << endl;
			cin.ignore();
		}
	}

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


User* Session::getActiveUser() const {
	return activeUser;
}

void Session::watchRecommendationFromAction(Watchable* w) {
	lastActionInput = "watch " + to_string(w->get_id());
	Watch* a = new Watch();
	a->act(*this);
}

std::vector<BaseAction*> Session::getActionLog() const {
	return actionsLog;
}



json Session::getJsonData(const std::string& configFilePath) {


	json aaa;

	std::ifstream watchableFile(configFilePath);
	watchableFile >> aaa;



	int id_count=1;
	for (int i = 0; i < aaa["movies"].size(); i++) {
		vector<string> vect = aaa["movies"][i]["tags"];
		Movie* watchC = new Movie(id_count, aaa["movies"][i]["name"], aaa["movies"][i]["length"], vect);
		id_count++;
		content.push_back(watchC);
		cout << vect[0] + "kkkk" << endl;
	}


	for (int j = 0; j < aaa["tv_series"].size(); j++) {
		cout << "jjjj";
		vector<string> vect2 = aaa["tv_series"][j]["tags"];
		for (int i = 0; i < aaa["tv_series"][j]["seasons"].size(); i++) {
			for (int k = 0; k < aaa["tv_series"][j]["seasons"][i]; k++) {

				Episode* watchS = new Episode(id_count, aaa["tv_series"][j]["name"], aaa["tv_series"][j]["episode_length"], i + 1, k + 1, vect2);
				id_count++;
				content.push_back(watchS);

				if ((aaa["tv_series"][j]["seasons"][i]  == k +1) && (i == aaa["tv_series"][j]["seasons"].size() - 1)) {
					
						watchS->set_Next_Episode(-1);
				}
				else
						watchS->set_Next_Episode(id_count);
		
			}
		}
	}


	cout << (*content[0]).toString() << endl;
	cout << (*content[1]).toString() << endl;
	cout << (*content[2]).toString() << endl;
	cout << (*content[3]).toString() << endl;

	cout << (*content[4]).toString() << endl;
	cout << (*content[5]).toString() << endl;
	cout << (*content[6]).toString() << endl;




	return {};
};

void Session::DisplayMenu()
{


	cout << endl << "\t\t\t\t  ***********************  " << endl
		<< "\t\t\t\t||        SPLFLIX          ||" << endl
		<< "\t\t\t\t||       is now on!        ||" << endl
		<< "\t\t\t\t  ***********************  " << endl << endl
		<< "\t\t\t\t                           " << endl
		<< "\t\t\t\t===========================" << endl
		<< "\t\t\t\t\tMAIN MENU: " << endl
		<< "\t\t\t\t1--> content" << endl
		<< "\t\t\t\t2--> watch" << endl
		<< "\t\t\t\t3--> delete user" << endl
		<< "\t\t\t\t4--> exit" << endl;

}


