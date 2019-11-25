#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"

class User;
class Watchable;

class Session {
public:
	Session(const std::string& configFilePath);
	~Session();
	void start();

	std::vector<Watchable*> get_content() const; //new
	std::string getLastActionInput() const; //new
	std::unordered_map<std::string, User*> getUserMap() const; //new
	void changeActiveUser(User*); //new 
	User* getActiveUser() const; //new
	void watchRecommendationFromAction(Watchable*); //new
	std::vector<BaseAction*> getActionLog() const; //new
private:
	std::vector<Watchable*> content;
	std::vector<BaseAction*> actionsLog;
	std::unordered_map<std::string, User*> userMap;
	User* activeUser;
	std::string lastActionInput();
};
#endif