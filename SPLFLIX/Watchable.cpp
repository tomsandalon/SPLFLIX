#include "Watchable.h"
#include "Session.h"

#include <string>
#include <vector>
#include <iostream>

using namespace std;


	Watchable::Watchable(long id, int length, std::vector<std::string>& tags) : id(id), length(length), tags(tags) {}

	Watchable::Watchable(const Watchable& other) : id(other.id), length(other.length), tags(other.tags) {}

	/*Watchable& operator=(const Watchable& other) {
		_id = other._id;
		_length = other._length;;
		_tags = other._tags;
		return *this;
	}*/

	Watchable::~Watchable() {
		tags.clear();
	}

	/*Watchable Watchable::getNextWatchable(const Session&) {
		return this->recommendation_algoritim;
	}*/



	std::string Watchable::toString() const {
		cout << id << endl;
		cout << length;
		for (int i = 0; i <= tags.size; i++)
		{
			cout << tags[i];
		}
	}

	int const Watchable::get_length() {
		return length;
	}
	long const Watchable::get_id() {
		return id;
	}
	std::vector<std::string> Watchable::get_tags() {
		return tags;
	}


	Movie::Movie(long id, const std::string& name, int length, std::vector<std::string>& tags) : Watchable(id, length, tags), name(name) {}


string Movie::toString() const {
	cout << name << endl;
}

Watchable* Movie::getNextWatchable(Session &s) const {
	std::vector<Watchable*> userHistory = s.getActiveUser().get_history();
	Watchable* lastWatchedMovie = userHistory[userHistory.size - 1];
	//return GetRecommendation;
}


	Episode::Episode(long id, const std::string& seriesName, int length, int season, int episode,  std::vector<std::string>& tags) : Watchable(id, length, tags), seriesName(seriesName), season(season), episode(episode)
	{}

	std::string Episode::toString() const{cout << seriesName << endl; }

	Watchable* Episode::getNextWatchable(Session &s) const {
		for (int i = 0; i <= s.get_content().size; i++) {
			if ((*s.get_content()[i]).get_id() == nextEpisodeId) {
				return s.get_content()[i];
			}
				
		}
		//return getRecommendation;

	}

