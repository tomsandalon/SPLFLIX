#include "Watchable.h"
#include "Session.h"

#include <string>
#include <vector>
#include <iostream>

class User;

using namespace std;
	Watchable::Watchable(long id, int length, const std::vector<std::string>& tags) : id(id), length(length), tags(tags) {}
    Watchable::Watchable(const Watchable& other) : id(other.id), length(other.length), tags(other.tags) {}

	/*Watchable& operator=(const Watchable& other) {
		_id = other._id;
		_length = other._length;;
		_tags = other._tags;
		return *this;
	}*/

	Watchable::~Watchable() {// CHECK THEAT OU

	}
	int const Watchable::get_length() const{
		return length;
	}
	long const Watchable::get_id() const{
		return id;
	}
	std::vector<std::string> Watchable::get_tags() const {
		return tags;
	}


	Movie::Movie(long id, const std::string& name, int length,  std::vector<std::string>& tags) : Watchable(id, length, tags), name(name) {}


	string Movie::toString() const {
		string tags = "";
		for (int i = 0; i < get_tags().size(); i++) {
			tags = tags + get_tags()[i]  ;
			if (i != (get_tags().size()-1))
			{
			tags = tags + ",";
			}
		}
		return "id:" + to_string(get_id()) + " " +  name+ " "  +  "minutes:" +to_string(get_length())  + " " +"[" + tags + "]";
	}
	


	Watchable* Movie::getNextWatchable(Session &s) const {
		return (*s.getActiveUser()).getRecommendation(s);
	}  



	Episode::Episode(long id, const std::string& seriesName, int length, int season, int episode,  std::vector<std::string>& tags) : Watchable(id, length, tags), seriesName(seriesName), season(season), episode(episode)
	{}

	std::string Episode::toString() const{
		string episodeN;
		string seasonN;
		string tags = "";
		for (int i = 0; i < get_tags().size(); i++) {
			tags = tags + get_tags()[i];
			if (i != (get_tags().size() - 1))
			{
				tags = tags + ",";
			}
		}
		if (episode >= 10)
			episodeN = to_string(episode);
		else
			episodeN = "0" + to_string(episode);
		if (season >= 10)
			seasonN = to_string(season);
		else
			seasonN = "0" + to_string(season);
	




		return "id:" + to_string(get_id()) + " " + seriesName + " " +"S"+ seasonN + "E" + episodeN + " "  + "minutes:" + to_string(get_length()) + " "+"[" + tags + "]";


	}

	Watchable* Episode::getNextWatchable(Session &s) const {
		for (int i = 0; i <= s.get_content().size(); i++) {
			if ((*s.get_content()[i]).get_id() == nextEpisodeId) {
				return s.get_content()[i];
			
			}
	    return (*s.getActiveUser()).getRecommendation(s);
		}
		
	}

