#pragma once
#define MAXNUMBER 15
#include <string>
class IUser
{
private:

public:
	virtual ~IUser() {};
	virtual std::string getUsername() = 0;
	virtual int getNumberOfNodes() =0;

};

class User :public IUser
{
private:
	std::string m_username;
	int numberOfNodes;
public:
	User(std::string username) : m_username(username) {
		numberOfNodes = 0;
	}
	~User() {};
	std::string getUsername() {
		return m_username;
	}
	int getNumberOfNodes() {
		return numberOfNodes;
	}
};

class Guest :public IUser
{
private:
	int numberOfNodes;
	int maxNumberOfNodes = MAXNUMBER;
public:
	Guest() { numberOfNodes = 0; }
	std::string getUsername() {
		return "guest";
	}
	int getNumberOfNodes() {
		return numberOfNodes;
	}
};