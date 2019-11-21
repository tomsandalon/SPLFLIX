#include "User.h"
using namespace std;

User::User(const string& _name) : name(_name), history(0) {} //user constructor

string User::getName() const {
	return name;
};

vector<Watchable*> User::get_history() const {
	return history;
};

bool User::already_watched(Watchable* w) const { //we check if the content was already watched by the user
	for (int i = 0; i < get_history().size(); i++)
	{
		if (get_history()[i]->get_id() == w->get_id())
			return true;
	}
	return false;
};

LengthRecommenderUser::LengthRecommenderUser(const string& _name) : User(_name) {};

void LengthRecommenderUser::addWatched(Watchable* w) {
	history.push_back(w);
};

double LengthRecommenderUser::averageWatchtime() const { //calculate the average watchtime of the user
	double totalLength = 0;
	for (int i = 0; i < get_history().size(); i++)
	{
		totalLength = totalLength + get_history()[i]->get_length();
	}
	return totalLength / get_history().size();
};

Watchable* LengthRecommenderUser::getRecommendation(Session& s) {
	double d = -1;
	double average = averageWatchtime();
	Watchable *w;
	for (int i = 0; i < s.get_content().size(); i++) {
		if (!already_watched(s.get_content()[i])) { //if this content wasn't already watched
			if (d = -1) {//the first content not watched
				w = (s.get_content()[i]);
				d = std::abs(average - w->get_length());
			}
			else {
				if (std::abs(average - s.get_content()[i].get_length() < d) { //check if we are closer to the required length. it is worth noting that only the first unwatched episode of a series is recommended, as they are inserted in order			
					w = (s.get_content()[i]);
					d = std::abs(average - w->get_length());
				}
			}
		}
	}
	return w; //return the pointer of the content
};

RerunRecommenderUser::RerunRecommenderUser(const string& _name) : User(_name), currentMovie(0) {};

	void RerunRecommenderUser::addWatched(Watchable* w) {
		history.push_back(w);
	};

	Watchable* RerunRecommenderUser::getRecommendation(Session& s) {
		if (get_history().size() == 0)
		{
			return nullptr;
		}
		Watchable* w = (get_history()[currentMovie % get_history().size()]);
		currentMovie++;
		return w;
	};

	bool GenreRecommenderUser::genreSort(const tuple<int, string> a, const tuple<int, string> b) {
		if (get<0>(a) == get<0>(b))
		{
			return (get<1>(a) < get<1>(b))); //compare lexicographicly
		}
		else
		{
			return (get<0>(a) > get<0>(b));
		}
	};
	void GenreRecommenderUser::addWatched(Watchable* w) {
		history.push_back(w);
		bool found;
		for (int i = 0; i < w.get_tags().size(); i++)
		{
			found = false;
			for (int j = 0; j < tagCounter.size(); j++)
				if (get<1>(tagCounter[j]).compare(w.get_tags()[i]) == 0)
				{
					get<0>(tagCounter[j]) += 1;
					found = true;
					j = INT_MAX;
				}
			if (!found)
			{
				tagCounter.push_back(make_tuple<string, int>(1, w.get_tags()[i]));
			}
		}
		sort(tagCounter.begin(), tagCounter.end(), genreSort);
	};

	GenreRecommenderUser::GenreRecommenderUser(const std::string& _name) : User(_name), tagCounter(0) {};

Watchable* GenreRecommenderUser::getRecommendation(Session& s) {
	for (int i = 0; i < tagCounter.size(); i++)
	{
		for (int j = 0; j < s.get_content().size(); j++)
		{
			for (int k = 0; k < s.get_content()[i].get_tags()[k]; k++)
			{
				if ((get<1>(tagCounter[i]).compare(s.get_content()[j].get_tags[k]) == 0) && (!already_watched(s.get_content()[i])))
					return s.get_content()[j];
			}
		}
	}
	return nullptr;
};

