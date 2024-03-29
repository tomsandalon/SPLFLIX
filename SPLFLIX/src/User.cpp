#include "../include/User.h"
using namespace std;



User::User(const string& _name) :  history(0), name(_name){} //user constructor

User::~User() {
	/*for (size_t i = 0; i < history.size(); i++){
	    if( history[i] != nullptr)
			delete history[i];
	}*/
};


string User::getName() const {
	return name;
};

void User::setName(string newName){
	name = newName;
}

vector<Watchable*> User::get_history() const {
	return history;
};

bool User::already_watched(Watchable* w) const { //we check if the content was already watched by the user
	for (size_t i = 0; i < get_history().size(); i++)
	{
		if (get_history()[i]->get_id() == w->get_id())
			return true;
	}
	return false;
};

LengthRecommenderUser::LengthRecommenderUser(const string& _name) : User(_name) {};

User* LengthRecommenderUser::clone(const Session& other) { //clone, then add the same movies
	LengthRecommenderUser* cloned = new LengthRecommenderUser(getName());
	for (size_t i = 0; i < get_history().size(); i++)
	{
		cloned->addWatched(other.get_content()[get_history()[i]->get_id()-1]);
	}
	return cloned;
}

void LengthRecommenderUser::addWatched(Watchable* w) { //add to the watch list
	history.push_back(w);
};

double LengthRecommenderUser::averageWatchtime() const { //calculate the average watchtime of the user
	double totalLength = 0;
	for (size_t i = 0; i < get_history().size(); i++)
	{
		totalLength = totalLength + get_history()[i]->get_length();
	}
	return totalLength / get_history().size();
};

Watchable* LengthRecommenderUser::getRecommendation(Session& s) {
	double d = -1;
	double average = averageWatchtime();
	Watchable* w = nullptr;
	for (size_t i = 0; i < s.get_content().size(); i++) {
		if (!already_watched(s.get_content()[i])) { //if this content wasn't already watched
			if (d == -1) {//the first content not watched
				w = (s.get_content()[i]);
				d = std::abs(average - w->get_length());
			}
			else {
				if (std::abs(average - s.get_content()[i]->get_length()) < d) { //check if we are closer to the required length. it is worth noting that only the first unwatched episode of a series is recommended, as they are inserted in order
					w = (s.get_content()[i]);
					d = std::abs(average - w->get_length());
				}
			}
		}
	}
	return w; //return the pointer of the content
};

string LengthRecommenderUser::algoType() const { return "len"; };

RerunRecommenderUser::RerunRecommenderUser(const string& _name) : User(_name), currentWatchable(0) {};

User* RerunRecommenderUser::clone(const Session& other) { //clone, then add the watchables to the new user, then set the currentWatchable
	RerunRecommenderUser* cloned = new RerunRecommenderUser(getName());
	for (size_t i = 0; i < get_history().size(); i++)
	{
		cloned->addWatched(other.get_content()[get_history()[i]->get_id()-1]);
	}
	cloned->setCurrentMovie(getCurrentMovie());
	return cloned;
}

void RerunRecommenderUser::addWatched(Watchable* w) { //add to the watchlist
	history.push_back(w);
};

void RerunRecommenderUser::setCurrentMovie(int count) {
	currentWatchable = count;
}

int RerunRecommenderUser::getCurrentMovie() {
	return currentWatchable;
}

Watchable* RerunRecommenderUser::getRecommendation(Session& s) {
	if (get_history().size() == 0)
	{
		return nullptr;
	}
	Watchable* w = (get_history()[currentWatchable % get_history().size()]);
	currentWatchable++;
	return w;
};

string RerunRecommenderUser::algoType() const { return "rer"; }; 

User* GenreRecommenderUser::clone(const Session& other) {
	GenreRecommenderUser* cloned = new GenreRecommenderUser(getName());
	for (size_t i = 0; i < get_history().size(); i++)
	{
		cloned->addWatched(other.get_content()[get_history()[i]->get_id()-1]);
	}
	return cloned;
}

void GenreRecommenderUser::addWatched(Watchable* w) { //add to the watchlist
	history.push_back(w);
	bool found;
	//add the proper tags to the tag list
	for (size_t i = 0; i < w->get_tags().size(); i++)
	{
		found = false;
		for (size_t j = 0; j < tagCounter.size(); j++)
		{
			if (get<1>(tagCounter[j]).compare(w->get_tags()[i]) == 0)
			{
				get<0>(tagCounter[j]) += 1;
				found = true;
				j = tagCounter.size();
			}
		}
		if (!found) //if it is a new tag
		{
			tagCounter.push_back(tuple<int, string>(1, w->get_tags()[i]));
		}
	}
	sortTags(); //sort the tag list
};

void GenreRecommenderUser::sortTags() { //bubble sort with two values
	bool swapped;
	string a;
	string b;
	for (size_t i = 0; i < tagCounter.size() - 1; i++)
	{
		swapped = false;
		for (size_t j = 0; j < tagCounter.size() - i - 1; j++)
		{
			if (get<0>(tagCounter[j]) < get<0>(tagCounter[j + 1])) {
				swap(tagCounter[j], tagCounter[j + 1]);
				swapped = true;
			}
			else if (get<0>(tagCounter[j]) == get<0>(tagCounter[j + 1]))
			{
				string a = get<1>(tagCounter[j]);
				string b = get<1>(tagCounter[j + 1]);
				for_each(a.begin(), a.end(), [](char& c) {	//change a to lower string
					c = ::tolower(c);
					});
				for_each(b.begin(), b.end(), [](char& c) {	//chage b to lower string
					c = ::tolower(c);
					});
				if (a > b)
				{
					swap(tagCounter[j], tagCounter[j + 1]);
					swapped = true;
				}
			}
		}
		if (swapped == false) break;
	}
}

GenreRecommenderUser::GenreRecommenderUser(const std::string& _name) : User(_name), tagCounter(0) {};

Watchable* GenreRecommenderUser::getRecommendation(Session& s) { //get the next recommendation
	for (size_t i = 0; i < tagCounter.size(); i++)
	{
		for (size_t j = 0; j < s.get_content().size(); j++)
		{
			if (!already_watched(s.get_content()[j])) {
				for (size_t k = 0; k < s.get_content()[j]->get_tags().size(); k++)
				{
					if ((get<1>(tagCounter[i]).compare(s.get_content()[j]->get_tags()[k]) == 0))
						return s.get_content()[j];
				}
			}
		}
	}
	return nullptr;
};

string GenreRecommenderUser::algoType() const { return "gen"; };
