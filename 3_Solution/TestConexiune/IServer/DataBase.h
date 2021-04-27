#pragma once
#include <sqlite3.h>
#include <olc_net.h>
#include <nlohmann/json.hpp>
class DataBase
{
private:
	//static DataBase* instance;

	//~DataBase();

	sqlite3* DB;
	static int callback(void* data, int argc, char** argv, char** azColName);
	static int callbackString(void* data, int argc, char** argv, char** azColName);
	static int callbackMore(void* data, int argc, char** argv, char** azColName);

public:
	DataBase();
	//	DataBase* getInstance();
	//	void deleteInstance();
	//	DataBase(DataBase&) = delete;
	bool connectToDataBase();
	void closeDataBase();


	bool insertUser(/* date de inregistrare sub forma de stringuri*/ std::string username, std::string firstname, std::string lastname, std::string email, std::string password);
	std::string loginUser(std::string email, std::string password);

	void createTrigger();
	void createTable();
	void createNodeRoot(std::string email);
	bool insertNewNode(std::string iduser, std::string idparinte, std::string name, std::string photo, std::string idnode);
	std::string selectIdForLastNode(std::string iduser);
	bool removeNode(std::string id, std::string iduser);
	std::string selectAllNodes(int iduser);
	bool newVersionText(std::string text, std::string iduser, std::string idnode);


private:
	std::string getTextForNode(std::string iduser, std::string idnode);
	std::string getLastVersionText(std::string iduser, std::string idnode);
};

