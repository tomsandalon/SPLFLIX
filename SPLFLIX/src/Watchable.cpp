#include "../include/Watchable.h"
#include "../include/Session.h"

#include <string>
#include <vector>
#include <iostream>

class User;

using namespace std;
	Watchable::Watchable(long id, int length, const std::vector<std::string>& tags) : id(id), length(length), tags(tags) {}
    Watchable::Watchable(const Watchable& other) : id(other.id), length(other.length), tags(other.tags) {}

	Watchable::~Watchable() {
	}
	void Episode::set_Next_Episode(int i){
		nextEpisodeId = i;
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

	Watchable* Watchable::clone() {
		return nullptr;
	}


	Movie::Movie(long id, const std::string& name, int length,  std::vector<std::string>& tags) : Watchable(id, length, tags), name(name) {}

	Watchable* Movie::clone() {
		Watchable *w = new Movie(*this);
		return w;
	}

	string Movie::toString(bool shortValue) const {
		string tags = "";
		for (size_t i = 0; i < get_tags().size(); i++) {
			tags = tags + get_tags()[i]  ;
			if (i != (get_tags().size()-1)){
			tags = tags + ",";
			}
		}


		string res = name;
		if(shortValue){
			return res;
		} else {
			return res + " "  + to_string(get_length())  + " minutes " +"[" + tags + "]";
		}

	}
	

	Watchable* Movie::getNextWatchable(Session &s) const {
		return (*s.getActiveUser()).getRecommendation(s);
	}  


	Episode::Episode(long id, const std::string& seriesName, int length, int season, int episode, std::vector<std::string>& tags) : Watchable(id, length, tags), seriesName(seriesName), season(season), episode(episode), nextEpisodeId(-1) {}


	Watchable* Episode::clone() {
		Watchable *w = new Episode(*this);
		return w;
	}

	std::string Episode::toString(bool shortValue) const{
		string episodeN;
		string seasonN;
		string tags = "";
		for (size_t i = 0; i < get_tags().size(); i++) {
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
		string res =  seriesName + " " + "S" + seasonN + "E" + episodeN;
		if(shortValue){
			return res;
		} else {
			return res + " " + to_string(get_length()) + " minutes " + "[" + tags + "]";
		}


	}



	Watchable* Episode::getNextWatchable(Session &s) const {
		if (nextEpisodeId == -1)
			return (*s.getActiveUser()).getRecommendation(s);
		for (size_t i = 0; i <= s.get_content().size(); i++) {
			if ((*s.get_content()[i]).get_id() == nextEpisodeId)  {
				return s.get_content()[i];
			}
		}
		return (*s.getActiveUser()).getRecommendation(s);
	}
