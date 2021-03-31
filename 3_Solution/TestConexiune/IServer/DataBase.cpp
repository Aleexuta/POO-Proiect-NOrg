#include "DataBase.h"
#include <stdio.h>
#include <sqlite3.h>
#include <iostream>
//DataBase* DataBase::instance = 0;

int DataBase::callback(void* data, int argc, char** argv, char** azColName)
{
	int i;
	fprintf(stderr, "%s: ", (const char*)data);

	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}

	printf("\n");
	return 0;
}

DataBase::DataBase()
{

}

bool DataBase::connectToDataBase()
{

	int exit = 0;
	exit = sqlite3_open("databasePOO.db", &DB);

	if (exit) {
		std::cerr << "Error open DB " << sqlite3_errmsg(DB) << std::endl;
		return false;
	}
	else
		std::cout << "Opened Database Successfully!" << std::endl;
	return true;

	
}

void DataBase::closeDataBase()
{
	//sqlite3_close(DB);
}

bool DataBase::insertUser(std::string username, std::string firstname, std::string lastname, std::string email, std::string password)
{
	//verifica mai intai sa nu mai existe username si email
	char* messaggeError;
	std::string sql ("INSERT INTO USER VALUES('" + username + "','" + firstname + "','" + lastname + "','" + email + "','" + password+"')");
	int exit = sqlite3_exec(DB, sql.c_str(), NULL, NULL,&messaggeError);
	if (exit != SQLITE_OK)
	{
		std::cerr << "Error Insert" << std::endl;
		sqlite3_free(messaggeError);
		return false;
	}
	else
	{ 
		std::cout << "Record inserted successfully!" << std::endl;
		return true;
	}
}

bool DataBase::loginUser(std::string username, std::string password)
{
	return false;
}


void DataBase::createTable()
{

	//creare tabel si apoi stergere cod
	std::string sql = "CREATE TABLE USER("
		"username           TEXT    NOT NULL, "
		"firstname          TEXT    NOT NULL, "
		"lastname			TEXT    NOT NULL, "
		"email			    TEXT	NOT NULL, "
		"password			TEXT    NOT NULL,"
		"UNIQUE(username, email)	);";
	char* messaggeError;
	int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);

	if (exit != SQLITE_OK) {
		std::cerr << "Error Create Table" << std::endl;
		sqlite3_free(messaggeError);
	}
	else
		std::cout << "Table created Successfully" << std::endl;

}