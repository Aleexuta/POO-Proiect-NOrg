#pragma once
#include <sqlite3.h>
#include <olc_net.h>
class DataBase
{
private:
	//static DataBase* instance;

	//~DataBase();

	sqlite3* DB;
	static int callback(void* data, int argc, char** argv, char** azColName);

public:
	DataBase();
//	DataBase* getInstance();
//	void deleteInstance();
//	DataBase(DataBase&) = delete;
	bool connectToDataBase();
	void closeDataBase();
	

	bool insertUser(/* date de inregistrare sub forma de stringuri*/ std::string username,std::string firstname, std::string lastname,std::string email, std::string password);
	std::string loginUser(std::string email, std::string password);
	
	void createTable();

	std::string rasp;
};

