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
	virtual bool canNewNode()=0;
	virtual void incrementNrNodes()=0;
	virtual void decrementNrNodes() = 0;
	virtual void setNumberOfNodes(int) = 0;
	virtual void setUsername(std::string user)=0;
	virtual void setID(int id) = 0;
	virtual int getID() = 0;
	virtual int getType() = 0;
};

class User :public IUser
{
private:
	std::string m_username;
	int m_id;
	int numberOfNodes; //maximul idnode, nu nr de noduri
public:
	User() {
		numberOfNodes = 0;
	}
	~User() {};
	std::string getUsername() {
		return m_username;
	}
	int getNumberOfNodes() {
		return numberOfNodes;
	}
	bool canNewNode()
	{
		return true;
	}
	void incrementNrNodes()
	{
		numberOfNodes++;
	}
	void decrementNrNodes() { numberOfNodes--; }
	void setNumberOfNodes(int nr)
	{
		numberOfNodes = nr;
	}
	void setUsername(std::string user)
	{
		m_username = user;
	}
	void setID(int id)
	{
		m_id = id;
	}
	int getID()
	{
		return m_id;
	}
	int getType()
	{
		return 1;
	}
};

class Guest :public IUser
{
private:
	int numberOfNodes;
	int maxNumberOfNodes = MAXNUMBER;
public:
	Guest() { numberOfNodes = 0; }
	~Guest() {};
	std::string getUsername() {
		return "guest";
	}
	int getNumberOfNodes() {
		return numberOfNodes;
	}
	bool canNewNode()
	{
		if (numberOfNodes < maxNumberOfNodes)
			return true;
		return false;
	}
	void incrementNrNodes()
	{
		numberOfNodes++;
	}
	void decrementNrNodes() { numberOfNodes--; }
	void setNumberOfNodes(int nr)
	{
		numberOfNodes = 0;
	}
	void setUsername(std::string user)
	{
		
	}
	void setID(int id)
	{
		
	}
	int getID()
	{
		return 0;
	}
	int getType()
	{
		return 0;
	}
};