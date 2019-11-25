#include "Action.h"
using namespace std;

BaseAction::BaseAction() : status(PENDING), errorMsg("") {};

ActionStatus BaseAction::getStatus() const { return status; };

void BaseAction::complete() { status = COMPLETED; };

void BaseAction::error(const string& _errorMsg) { status = ERROR; errorMsg = _errorMsg; };

string BaseAction::getErrorMsg() const { return errorMsg; };

int BaseAction::countWords(string s) const {
	int count = 0;
	for (size_t i = 0; i < s.length(); i++)
	{
		if (s[i] == ' ')
			count++;
	}
	return count + 1;
}

string BaseAction::getWord(int num, string s) const { //returns a subword
	int count = 0;
	size_t loc = 0;
	string r = "";
	for (size_t i = 0; i < s.length(); i++)
	{
		if (s[i] == ' ')
			count++;
		if (count == num)
		{
			loc = i + 1;
			i = s.length();
		}
	}
	bool found = false;
	int i = 0;
	while (loc + i < s.length() && !found)
	{
		if (s[loc + i] == ' ')
		{
			found = true;
		}
		else
		{
			r = r + s[loc + i];
			i++;
		}
	}
	return r;
}

void CreateUser::act(Session& sess) {
	User* u;
	if (countWords(sess.getLastActionInput()) != 3)
	{
		error("The number of variables for this command is illegal");
		cout << getErrorMsg() << endl;
		return;
	}
	string userName = getWord(1, sess.getLastActionInput());
	string algo = getWord(2, sess.getLastActionInput());
	if (sess.getUserMap().count(userName) > 0)
	{
		error("This username is already taken");
		cout << getErrorMsg() << endl;
		return;
	}
	if (algo == "len")
	{
		u = new LengthRecommenderUser(userName);
		sess.getUserMap()[userName] = u;
		cout << "Created new user named: " << userName << "\twith Length algorithm" <<endl;
		complete();
		return;
	}
	if (algo == "rer")
	{
		u = new RerunRecommenderUser(userName);
		sess.getUserMap()[userName] = u;
		cout << "Created new user named: " << userName << "\twith Rerun algorithm" << endl;
		complete();
		return;
	}
	if (algo == "gen")
	{
		u = new GenreRecommenderUser(userName);
		sess.getUserMap()[userName] = u;
		cout << "Created new user named: " << userName << "\twith Genre algorithm" << endl;
		complete();
		return;
	}
	error("3-letter code not found");
	cout << getErrorMsg() << endl;
	return;
}

string CreateUser::toString() const{
	if (getStatus() == ERROR)
	{
		return "Create User function resulted in an error:\t" + getErrorMsg();
	}
	if (getStatus() == COMPLETED)
	{
		return "Create User function was completed successfully";
	}
	else // (getStatus() == PENDING)
	{
		return "Create User function is still pending";
	}
}

void ChangeActiveUser::act(Session& sess) {
	if (countWords(sess.getLastActionInput()) != 2)
	{
		error("Invalid arguments for change active user action");
		cout << getErrorMsg() << endl;
		return;
	}
	string userName = getWord(1, sess.getLastActionInput());
	if (sess.getUserMap().count(userName) == 0)
	{
		error("Username not found");
		cout << getErrorMsg() << endl;
		return;
	}
	else
	{
		sess.changeActiveUser(sess.getUserMap()[userName]);
		cout << "Active user was changed to " << userName << endl;
		complete();
		return;
	}
}

string ChangeActiveUser::toString() const {
	if (getStatus() == ERROR)
	{
		return "Change Active User function resulted in an error:\t" + getErrorMsg();
	}
	if (getStatus() == COMPLETED)
	{
		return "Change Active User function was completed successfully";
	}
	else // (getStatus() == PENDING)
	{
		return "Change Active User function is still pending";
	}
}

void DeleteUser::act(Session& sess) {
	if (countWords(sess.getLastActionInput()) != 2)
	{
		error("Invalid arguments for delete user action");
		cout << getErrorMsg() << endl;
		return;
	}
	string userName = getWord(1, sess.getLastActionInput());
	if (sess.getActiveUser()->getName() == userName)
	{
		error("Cant delete current active user");
		cout << getErrorMsg() << endl;
		return;
	}
	if (sess.getUserMap().count(userName) == 0)
	{
		error("Username not found");
		cout << getErrorMsg() << endl;
		return;
	}
	else
	{
		sess.getUserMap()[userName]->~User();
		sess.getUserMap()[userName] = nullptr;
		sess.getUserMap().erase(userName);
		cout << "Deleted username " << userName << endl;
		complete();
		return;
	}
}

string DeleteUser::toString() const {
	if (getStatus() == ERROR)
	{
		return "Delete User function resulted in an error:\t" + getErrorMsg();
	}
	if (getStatus() == COMPLETED)
	{
		return "Delete User function was completed successfully";
	}
	else // (getStatus() == PENDING)
	{
		return "Delete User function is still pending";
	}
}

void DuplicateUser::act(Session& sess) {
	if (countWords(sess.getLastActionInput()) != 3)
	{
		error("Invalid arguments for duplicate user action");
		cout << getErrorMsg() << endl;
		return;
	}
	string oldUser = getWord(1, sess.getLastActionInput());
	string newUser = getWord(2, sess.getLastActionInput());
	if (sess.getUserMap().count(oldUser) == 0)
	{
		error("Original username does not exist");
		cout << getErrorMsg() << endl;
		return;
	}
	if (sess.getUserMap().count(newUser) > 0)
	{
		error("New username already exist");
		cout << getErrorMsg() << endl;
		return;
	}
	User* u;
	if (sess.getUserMap()[oldUser]->algoType() == "len")
	{
		u = new LengthRecommenderUser(newUser);
	}
	else if (sess.getUserMap()[oldUser]->algoType() == "rer")
	{
		u = new RerunRecommenderUser(newUser);
	}
	else
	{
		u = new GenreRecommenderUser(newUser);
	}
	for (size_t i = 0; i < sess.getUserMap()[oldUser]->get_history().size(); i++)
	{
		u->addWatched(sess.getUserMap()[oldUser]->get_history()[i]);
	}
	sess.getUserMap()[newUser] = u;
	cout << "Duplicated " << oldUser << " into " << newUser << " successfully" << endl;
	complete();
	return;
}

string DuplicateUser::toString() const {
	if (getStatus() == ERROR)
	{
		return "Duplicate User function resulted in an error:\t" + getErrorMsg();
	}
	if (getStatus() == COMPLETED)
	{
		return "Duplicate User function was completed successfully";
	}
	else // (getStatus() == PENDING)
	{
		return "Duplicate User function is still pending";
	}
}


string PrintContentList::tagsToString(vector<string> tags) {
	string s = "[";
	for (size_t i = 0; i < tags.size(); i++)
	{
		s = s + tags[i];
		if (i + 1 < tags.size())
		{
			s = s + ", ";
		}
	}
	s = s + "]";
	return s;
}

void PrintContentList::act(Session& sess) {
	if (sess.get_content().size() == 0)
	{
		error("No content in SPLFLIX");
		cout << getErrorMsg() << endl;
		return;
	}
	for (size_t i = 0; i < sess.get_content().size(); i++)
	{
		cout << (i + 1) << ". " << sess.get_content()[i]->toString() << " " << sess.get_content()[i]->get_length() << " minutes " << PrintContentList::tagsToString(sess.get_content()[i]->get_tags()) << endl;
	}
	complete();
	return;
}

string PrintContentList::toString() const {
	if (getStatus() == ERROR)
	{
		return "Print content function resulted in an error:\t" + getErrorMsg();
	}
	if (getStatus() == COMPLETED)
	{
		return "Content was printed successfully";
	}
	else // (getStatus() == PENDING)
	{
		return "Content printing function is still pending";
	}
}

void PrintWatchHistory::act(Session& sess) {
	if (sess.getActiveUser()->get_history().size() == 0)
	{
		error("Current user has not watched anything yet");
		cout << getErrorMsg() << endl;
		return;
	}
	cout << "Watch history for " << sess.getActiveUser()->getName() << endl;
	for (size_t i = 0; i < sess.getActiveUser()->get_history().size(); i++)
	{
		cout << (i+1) << ". " << sess.getActiveUser()->get_history()[i]->toString() << endl;
	}
	complete();
	return;
}

string PrintWatchHistory::toString() const {
	if (getStatus() == ERROR)
	{
		return "Print history function resulted in an error:\t" + getErrorMsg();
	}
	if (getStatus() == COMPLETED)
	{
		return "History was printed successfully";
	}
	else // (getStatus() == PENDING)
	{
		return "History printing function is still pending";
	}
}

int Watch::isInteger(string s) {
	try {
		return stoi(s);
	}
	catch (const std::exception & e) {
		return -1;
	}
}

char Watch::isChar() {
	try {
		char c;
		cin >> c;
		return c;
	}
	catch (const std::exception & e) {
		return 'a';
	}
}

void Watch::act(Session& sess) {
	if (countWords(sess.getLastActionInput()) != 2)
	{
		error("Invalid arguments for watch user action");
		cout << getErrorMsg() << endl;
		return;
	}
	int id = Watch::isInteger(getWord(1, sess.getLastActionInput())) - 1;
	if (id < 0 || id >= sess.get_content().size())
	{
		error("Invalid content ID");
		cout << getErrorMsg() << endl;
		return;
	}
	cout << "Watching " << sess.get_content()[id]->toString() << endl;
	sess.getActiveUser()->addWatched(sess.get_content()[id]);
	Watchable* w = sess.get_content()[id]->getNextWatchable(sess);
	complete();
	if (w == nullptr)
	{
		cout << "No more content to recommend" << endl;
		return;
	}
	cout << "We recommend watching " << w->toString() << ", continue watching? [y/n]" << endl;
	char c = 'a';
	while (c != 'y' || c != 'n')
	{
		c = Watch::isChar();
	}
	if (c == 'n')
	{
		return;
	}
	else
	{
		sess.watchRecommendationFromAction(w);
		return;
	}
}

void Watch::act(Session& sess, Watchable* watch) {
	cout << "Watching " << watch->toString() << endl;
	sess.getActiveUser()->addWatched(watch);
	Watchable* w = watch->getNextWatchable(sess);
	complete();
	if (w == nullptr)
	{
		cout << "No more content to recommend" << endl;
		return;
	}
	cout << "We recommend watching " << w->toString() << ", continue watching? [y/n]" << endl;
	char c = 'a';
	while (c != 'y' || c != 'n')
	{
		c = Watch::isChar();
	}
	if (c == 'n')
	{
		return;
	}
	else
	{
		sess.watchRecommendationFromAction(w);
		return;
	}
}

string Watch::toString() const {
	if (getStatus() == ERROR)
	{
		return "Watch function resulted in an error:\t" + getErrorMsg();
	}
	if (getStatus() == COMPLETED)
	{
		return "Watch performed successfully";
	}
	else // (getStatus() == PENDING)
	{
		return "Watch function is still pending";
	}
}

void PrintActionsLog::act(Session& sess) {
	for (size_t i = 0; i < sess.getActionLog().size(); i++)
	{
		cout << sess.getActionLog()[i]->toString() << endl;
	}
	complete();
	return;
}

string PrintActionsLog::toString() const {
	if (getStatus() == ERROR)
	{
		return "Print log function resulted in an error:\t" + getErrorMsg();
	}
	if (getStatus() == COMPLETED)
	{
		return "Log printed successfully";
	}
	else // (getStatus() == PENDING)
	{
		return "Log printing is still pending";
	}
}