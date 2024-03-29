#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "User.h"
#include "Session.h"

class Session;

enum ActionStatus {
	PENDING, COMPLETED, ERROR
};


class BaseAction {
public:
    virtual BaseAction* clone();
	BaseAction();
	virtual ~BaseAction();
	ActionStatus getStatus() const;
	virtual void act(Session& sess) = 0;
	virtual std::string toString() const = 0;
protected:
	void complete();
	void error(const std::string& errorMsg);
	std::string getErrorMsg() const;
	std::string getWord(int, std::string) const;
	int countWords(std::string) const;
private:
	std::string errorMsg;
	ActionStatus status;
};

class CreateUser : public BaseAction {
public:
    virtual BaseAction* clone();
	virtual void act(Session& sess);
	virtual std::string toString() const;
};

class ChangeActiveUser : public BaseAction {
public:
    virtual BaseAction* clone();
	virtual void act(Session& sess);
	virtual std::string toString() const;
};

class DeleteUser : public BaseAction {
public:
    virtual BaseAction* clone();
	virtual void act(Session& sess);
	virtual std::string toString() const;
};

class DuplicateUser : public BaseAction {
public:
    virtual BaseAction* clone();
	virtual void act(Session& sess);
	virtual std::string toString() const;
};

class PrintContentList : public BaseAction {
public:
    virtual BaseAction* clone();
	virtual void act(Session& sess);
	virtual std::string toString() const;
private:
	static std::string tagsToString(std::vector<std::string>);
};

class PrintWatchHistory : public BaseAction {
public:
    virtual BaseAction* clone();
	virtual void act(Session& sess);
	virtual std::string toString() const;
};


class Watch : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
	virtual BaseAction* clone();
private:
	static int isInteger(std::string);
	static char isChar();
};


class PrintActionsLog : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
	virtual BaseAction* clone();
};

class Exit : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
	virtual BaseAction* clone();
};

#endif