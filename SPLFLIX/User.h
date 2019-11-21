#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include "Watchable.h"
#include <unordered_set>
#include <unordered_map>
#include <tuple>
#include <algorithm>
class Watchable;
class Session;

class User {
public:
	User(const std::string& name); //constructor
	virtual ~User(); //destructor
	virtual User(const User&& other); //copy constructor
	virtual User(User&& other); //Move constructor
	virtual User& operator=(const User& other);// Copy Assignment
	virtual User& operator=(User&& other); //Move Assignment
	virtual Watchable* getRecommendation(Session& s);
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
	LengthRecommenderUser(const std::string& name);//constructor
	virtual Watchable* getRecommendation(Session& s);
	virtual void addWatched(Watchable*)
private:
	double averageWatchtime() const;
};

class RerunRecommenderUser : public User {
public:
	RerunRecommenderUser(const std::string& name); //constructor
	virtual ~User(); //destructor
	virtual User(const RerunRecommenderUser&& other); //copy constructor
	virtual User(RerunRecommenderUser&& other); //Move constructor
	virtual User& operator=(const RerunRecommenderUser& other);// Copy Assignment
	virtual User& operator=(RerunRecommenderUser&& other); //Move Assignment
	virtual Watchable* getRecommendation(Session& s);
	virtual void addWatched(Watchable*);
private:
	int currentMovie;
};

class GenreRecommenderUser : public User {
public:
	GenreRecommenderUser(const std::string& name); //constructor
	virtual ~User(); //destructor
	virtual User(const GenreRecommenderUser&& other); //copy constructor
	virtual User(GenreRecommenderUser&& other); //Move constructor
	virtual User& operator=(const GenreRecommenderUser& other);// Copy Assignment
	virtual User& operator=(GenreRecommenderUser&& other); //Move Assignment
	virtual Watchable* getRecommendation(Session& s);
private:
	std::vector<std::tuple<int, std::string>*> tagCounter;
	virtual void addWatched(Watchable*);
	bool genreSort(const std::tuple<int, std::string> a, const std::tuple<int, std::string> b);
};

#endif
