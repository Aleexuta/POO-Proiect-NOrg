#include "DataBase.h"
#include <stdio.h>
#include <sqlite3.h>
#include <iostream>
#include <string>
//DataBase* DataBase::instance = 0;
std::string final;
int DataBase::callback(void* data, int argc, char** argv, char** azColName)
{
	int i;
	fprintf(stderr, "%s: ", (const char*)data);
	std::string rasp="{";
	
	for (i = 0; i < argc-1; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		std::string col(azColName[i]);
		std::string sol(argv[i] ? argv[i] : "NULL");
		rasp += "\"" + col + "\":" + "\"" + sol + "\",";
	
	}
	i = argc - 1;
	printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	std::string col(azColName[i]);
	std::string sol(argv[i] ? argv[i] : "NULL");
	rasp += "\"" + col + "\":" + "\"" + sol + "\"}";
	std::cout << rasp<<std::endl;
	printf("\n");
	final = rasp;
	return 0;
}

DataBase::DataBase()
{

}

bool DataBase::connectToDataBase()
{

	int exit = 0;
	exit = sqlite3_open("DBPOO.db", &DB);

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
	std::string sql ("INSERT INTO USER (username,firstname,lastname,email,password) VALUES('" + username + "','" + firstname + "','" + lastname + "','" + email + "','" + password+"')");
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

std::string DataBase::loginUser(std::string email, std::string password)
{
	char* messaggeError;
	std::string sql("SELECT iduser, username, password FROM USER where email like '"+email+"'");
	std::string data("CALLBACK FUNCTION");
	int exit = sqlite3_exec(DB, sql.c_str(), callback, (void*)data.c_str(), NULL);
	if (exit != SQLITE_OK)
	{
		std::cerr << "Error Select" << std::endl;
		return "";
	}
	else 
	{
		std::cout << "Select successfully!" << std::endl;
		return final;
	}
}


void DataBase::createTable()
{

	//creare tabel si apoi stergere cod
	std::string sql = "CREATE TABLE USER("
		"iduser				integer	primary key, "	
		"username           TEXT    NOT NULL, "
		"firstname          TEXT    NOT NULL, "
		"lastname			TEXT    NOT NULL, "
		"email			    TEXT	NOT NULL unique, "
		"password			TEXT    NOT NULL)";
	char* messaggeError;
	int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);

	if (exit != SQLITE_OK) {
		std::cerr << "Error Create Table" << std::endl;
		sqlite3_free(messaggeError);
	}
	else
		std::cout << "Table created Successfully" << std::endl;

}