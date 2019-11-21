#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include "Watchable.h"
#include <unordered_set>
#include <unordered_map>
#include <tuple>
class Watchable;
class Session;

class User {
public:
	User(const std::string& name);
	virtual Watchable* getRecommendation(Session& s) = 0;
	std::string getName() const;
	std::vector<Watchable*> get_history() const;
	bool already_watched(Watchable*) const;
	virtual void addWatched(Watchable*) = 0;
protected:
	std::vector<Watchable*> history;
private:
	const std::string name;
};


class LengthRecommenderUser : public User {
public:
	LengthRecommenderUser(const std::string& name);
	virtual Watchable* getRecommendation(Session& s);
	virtual void addWatched(Watchable*);
private:
	double averageWatchtime() const;
};

class RerunRecommenderUser : public User {
public:
	RerunRecommenderUser(const std::string& name);
	virtual Watchable* getRecommendation(Session& s);
	virtual void addWatched(Watchable*);
private:
	int currentMovie;
};

class GenreRecommenderUser : public User {
public:
	GenreRecommenderUser(const std::string& name);
	virtual Watchable* getRecommendation(Session& s);
private:
	std::vector<std::tuple<int, std::string>> tagCounter;
	virtual void addWatched(Watchable*);
	bool genreSort(const std::tuple<int, std::string>, const std::tuple<int, std::string>);
};

#endif#pragma once
