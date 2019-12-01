#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>
using namespace std;

class Session;

class Watchable {
public:
	Watchable(long id, int length, const std::vector<std::string>& tags);
	Watchable(const Watchable& other);
	virtual ~Watchable();
	virtual std::string toString(bool shortValue) const = 0; //
	virtual Watchable* getNextWatchable(Session&) const = 0;//
	int const get_length() const;
	long const get_id() const;
	std::vector<std::string> get_tags() const;
	virtual Watchable* clone();
private:
	const long id;
	int length;
	std::vector<std::string> tags;
};

class Movie : public Watchable {
public:
	Movie(long id, const std::string& name, int length,  std::vector<std::string>& tags);
	virtual std::string toString(bool shortValue) const;
	virtual Watchable* getNextWatchable(Session&) const;
	virtual Watchable* clone();
private:
	std::string name;
};


class Episode : public Watchable {
public:
	Episode(long id, const std::string& seriesName, int length, int season, int episode,  std::vector<std::string>& tags);
	virtual std::string toString(bool shortValue) const;
	virtual Watchable* getNextWatchable(Session&) const;
	virtual void set_Next_Episode(int i);// new
	virtual Watchable* clone();
private:
	std::string seriesName;
	int season;
	int episode;
	long nextEpisodeId;
};

#endif
