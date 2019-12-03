#include "../include/Action.h"
using namespace std;
BaseAction::~BaseAction() {
	};
BaseAction::BaseAction() : errorMsg(""), status(PENDING) {};

ActionStatus BaseAction::getStatus() const { return status; };
void BaseAction::complete() { status = COMPLETED; }; //change status to complete

void BaseAction::error(const string& _errorMsg) { status = ERROR; errorMsg = _errorMsg; }; //change status to error and log the error

string BaseAction::getErrorMsg() const { return errorMsg; }; 

int BaseAction::countWords(string s) const { //count the number of words in a string
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
BaseAction* BaseAction::clone() {
		 return nullptr;
}

void CreateUser::act(Session& sess) {
	User* u;
	if (countWords(sess.getLastActionInput()) < 3) //if the number of arguments does not match
	{
		error("The number of variables for this command is illegal");
		cout << toString() << endl;
		return;
	}
	string userName = getWord(1, sess.getLastActionInput());
	string algo = getWord(2, sess.getLastActionInput());
	if (sess.userExists(userName)) //if the username is available
	{
		error("Username is already in use");
		cout << toString() << endl;
		return;
	}
	if (algo == "len") //length algo
	{
		u = new LengthRecommenderUser(userName);
		sess.addUserToMap(u);
		complete();
		return;
	}
	if (algo == "rer") //rerun algo
	{
		u = new RerunRecommenderUser(userName);
		sess.addUserToMap(u);
		complete();
		return;
	}
	if (algo == "gen") //genre algo
	{
		u = new GenreRecommenderUser(userName);
		sess.addUserToMap(u);
		complete();
		return;
	}
	error("Recommendation algorithm is invalid");
	cout << toString() << endl;
	return;
}

string CreateUser::toString() const{
	if (getStatus() == ERROR)
	{
		return "Error - " + getErrorMsg();
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
BaseAction* CreateUser::clone() {
		BaseAction *w = new CreateUser(*this);
		return w;
}

void ChangeActiveUser::act(Session& sess) {
	if (countWords(sess.getLastActionInput()) < 2) //if the number of arguments is invalid
	{
		error("Invalid arguments for change active user action");
		cout << toString() << endl;
		return;
	}
	string userName = getWord(1, sess.getLastActionInput());
	if (!sess.userExists(userName)) //if the username exists
	{
		error("username does not exist");
		cout << toString() << endl;
		return;
	}
	else
	{
		sess.changeActiveUser(userName);
		complete();
		return;
	}
}

string ChangeActiveUser::toString() const {
	if (getStatus() == ERROR)
	{
		return "Error - " + getErrorMsg();
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
BaseAction* ChangeActiveUser::clone() {
		BaseAction *w = new ChangeActiveUser(*this);
		return w;
}


void DeleteUser::act(Session& sess) {
	if (countWords(sess.getLastActionInput()) < 2) //if the number of arguments is invalid
	{
		error("Invalid arguments for delete user action");
		cout << toString() << endl;
		return;
	}
	string userName = getWord(1, sess.getLastActionInput());
	if (sess.getActiveUser()->getName().compare(userName) == 0) //if the username to delete is the active username
	{
		error("Cant delete current active user");
		cout << toString() << endl;
		return;
	}
	if (!sess.userExists(userName)) //if the username exists
	{
		error("Username does not exist");
		cout << toString() << endl;
		return;
	}
	else
	{
		sess.deleteUser(userName);
		//cout << "Deleted username " << userName << endl;
		complete();
		return;
	}
}
	
string DeleteUser::toString() const {
	if (getStatus() == ERROR)
	{
		return "Error - " + getErrorMsg();
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

BaseAction* DeleteUser::clone() {
		BaseAction *w = new DeleteUser(*this);
		return w;
}

void DuplicateUser::act(Session& sess) {
	if (countWords(sess.getLastActionInput()) < 3) //if the number of arguments is valid
	{
		error("Invalid arguments for duplicate user action");
		cout << toString() << endl;
		return;
	}
	string oldUser = getWord(1, sess.getLastActionInput());
	string newUser = getWord(2, sess.getLastActionInput());


	if (!sess.userExists(oldUser)) //if the old username exists
	{
		error("Original username does not exist");
		cout << toString() << endl;
		return;
	}
	if (sess.userExists(newUser)) //if the new username exists
	{
		error("username is already in use");
		cout << toString() << endl;
		return;
	}
	if (sess.getUserByName(oldUser) != nullptr) {
		User* u = sess.getUserByName(oldUser)->clone(sess); //clone the user
		u->setName(newUser); //set a new name
		sess.addUserToMap(u); //add the user to the user map
	}
	complete();
	return;
}

string DuplicateUser::toString() const {
	if (getStatus() == ERROR)
	{
		return "Error - " + getErrorMsg();
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

BaseAction* DuplicateUser::clone() {
		BaseAction *w = new DuplicateUser(*this);
		return w;
}


string PrintContentList::tagsToString(vector<string> tags) { //creating a valid string from tags
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
	if (sess.get_content().size() == 0) //check if there is content
	{
		error("No content in SPLFLIX");
		cout << toString() << endl;
		return;
	}
	for (size_t i = 0; i < sess.get_content().size(); i++) //print the content
	{
		cout << (i + 1) << ". " << sess.get_content()[i]->toString(false)  << endl; //print the content list
	}
	complete();
	return;
}

string PrintContentList::toString() const {
	if (getStatus() == ERROR)
	{
		return "Error - " + getErrorMsg();
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
	if (sess.getActiveUser()->get_history().size() == 0) //check if the user has history
	{
		error("Current user has not watched anything yet");
		cout << toString() << endl;
		return;
	}
	cout << "Watch history for " << sess.getActiveUser()->getName() << endl;
	for (size_t i = 0; i < sess.getActiveUser()->get_history().size(); i++) //print the history according to the format
	{
		cout << (i+1) << ". " << sess.getActiveUser()->get_history()[i]->toString(true) << endl;
	}
	complete();
	return;
}

string PrintWatchHistory::toString() const {
	if (getStatus() == ERROR)
	{
		return "Error - " + getErrorMsg();
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
BaseAction* PrintWatchHistory::clone() {
		BaseAction *w = new PrintWatchHistory(*this);
		return w;
}

BaseAction* PrintContentList::clone() {
		BaseAction *w = new PrintContentList(*this);
		return w;
}

int Watch::isInteger(string s) { //check if the string can be cast to string, then cast it
	try {
		return stoi(s);
	}
	catch (const std::exception &) {
		return -1;
	}
}

char Watch::isChar() { //try to cast input to char
	try {
		char c;
		cin >> c;
		return c;
	}
	catch (const std::exception &) {
		return 'a';
	}
}

void Watch::act(Session& sess) {
	if (countWords(sess.getLastActionInput()) < 2) //check if the number of arguments is valid
	{
		error("Invalid arguments for watch user action");
		cout << toString() << endl;
		return;
	}
	int id = Watch::isInteger(getWord(1, sess.getLastActionInput())) - 1; //find the id of the movie - it's location is (printed id-1)
	if (id < 0 || id >= static_cast<int>(sess.get_content().size())) //if the movie id exists
	{
		error("Invalid content ID");
		cout << toString() << endl;
		return;
	}
	cout << "Watching " << sess.get_content()[id]->toString(true) << endl;
	sess.addWatchedToUser(sess.getActiveUser(), sess.get_content()[id]);
	//sess.getActiveUser()->addWatched(sess.get_content()[id]); //watch the watchable
	Watchable* w = sess.get_content()[id]->getNextWatchable(sess); //get the next recommendation
	complete();
	if (w == nullptr)
	{
		cout << "No more content to recommend" << endl;
		return;
	}
	cout << "We recommend watching " << w->toString(true) << ", continue watching? [y/n]" << endl;
	char c = 'a';
	while (c != 'y' && c != 'n') //dont stop until the user inputs a proper char
	{
		c = Watch::isChar();
	}
	if (c == 'n')
	{
		while ((getchar()) != '\n');
		return;
	}
	else
	{
		while ((getchar()) != '\n');
		sess.watchRecommendationFromAction(w); //send the session a command to watch the watchable
		return;
	}
}

string Watch::toString() const {
	if (getStatus() == ERROR)
	{
		return "Error - " + getErrorMsg();
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
BaseAction* Watch::clone() {
		BaseAction *w = new Watch(*this);
		return w;
}


void PrintActionsLog::act(Session& sess) { //print the action log
	for (size_t i = 0; i < sess.getActionLog().size(); i++)
	{
		cout << sess.getActionLog()[i]->toString() << endl; 
		
	}
	complete();
	return;
}
BaseAction* PrintActionsLog::clone() {
		BaseAction *w = new PrintActionsLog(*this);
		return w;
}


string PrintActionsLog::toString() const {
	if (getStatus() == ERROR)
	{
		return "Error - " + getErrorMsg();
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


void Exit::act(Session& sess) {
	try {
		sess.setRunToFalse(); //set the 'continue running' flag in session to false
		complete();
		return;
	}
	catch (exception &)
	{
		error("Failed to exit");
		cout << getErrorMsg() << endl;
		return;
	}
}

string Exit::toString() const {
	if (getStatus() == ERROR)
	{
		return "Error - " + getErrorMsg();
	}
	if (getStatus() == COMPLETED)
	{
		return "Exit successfully";
	}
	else // (getStatus() == PENDING)
	{
		return "Exit is still pending";
	}
}
BaseAction* Exit::clone() {
		BaseAction *w = new Exit(*this);
		return w;
}
