#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"

#include "json.hpp"

using json = nlohmann::json;


class User;
class Watchable;
class BaseAction;
class PrintActionsLog;
class Session {
public:
	Session(const std::string& configFilePath);
	virtual ~Session();
	Session& operator=(const Session& other);//new
	Session(const Session& other);// new
	Session& operator=(Session&& other);// new
	Session(Session&& other);// new
	void start();
	std::vector<Watchable*> get_content() const; //new
	std::string getLastActionInput() const; //new
	//std::unordered_map<std::string, User*>& getUserMap(); //new
	void changeActiveUser(std::string); //new 
	User* getActiveUser() const; //new
	void watchRecommendationFromAction(Watchable*); //new
	std::vector<BaseAction*> getActionLog() const; //new
	void setRunToFalse(); //new
	bool userExists(std::string) const;
	User* getUserByName(std::string) const;
	void addUserToMap(User*);
	void deleteUser(std::string);
	void addWatchedToUser(User*, Watchable*);
	std::string getUserAlgoType(string) const;
	std::vector<Watchable*> getUserHistory(string) const;
private:
	void clean();// new
	void copy(const Session& other);// new
	void steal(Session& other);// new
	void setJsonData(const std::string& configFilePath);
	std::vector<Watchable*> content;
	std::vector<BaseAction*> actionsLog;
	std::unordered_map<std::string, User*> userMap;
	User* activeUser;
	std::string lastActionInput;
	bool run;
};
#endif