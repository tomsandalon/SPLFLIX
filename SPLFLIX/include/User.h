#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include "Watchable.h"
#include "Session.h"
#include <unordered_set>
#include <unordered_map>
#include <tuple>
#include <algorithm>
class Watchable;
class Session;

class User {
public:
	User(const std::string& name); //constructor
	virtual ~User();
	virtual Watchable* getRecommendation(Session& s) = 0;
	std::string getName() const;
	void setName(string newName);
	std::vector<Watchable*> get_history() const;
	bool already_watched(Watchable*) const;
	virtual void addWatched(Watchable*) = 0;
	virtual std::string algoType() const = 0; //return the type of algo
	virtual User* clone(const Session&) = 0;
protected:
	std::vector<Watchable*> history;
private:
	 std::string name;
};


class LengthRecommenderUser : public User {
public:
	LengthRecommenderUser(const std::string& name);//constructor
	virtual Watchable* getRecommendation(Session& s);
	virtual void addWatched(Watchable*);
	virtual std::string algoType() const;
	virtual User* clone(const Session&);
private:
	double averageWatchtime() const;
};

class RerunRecommenderUser : public User {
public:
	RerunRecommenderUser(const std::string& name); //constructor
	virtual Watchable* getRecommendation(Session& s);
	virtual void addWatched(Watchable*);
	virtual std::string algoType() const;
	void setCurrentMovie(int);
	int getCurrentMovie();
	virtual User* clone(const Session&);
private:
	int currentWatchable;
};

class GenreRecommenderUser : public User {
public:
	GenreRecommenderUser(const std::string& name); //constructor
	virtual Watchable* getRecommendation(Session& s);
	virtual std::string algoType() const;
	virtual User* clone(const Session&);
private:
	std::vector<std::tuple<int, std::string>> tagCounter;
	virtual void addWatched(Watchable*);
	virtual void sortTags();
};

#endif