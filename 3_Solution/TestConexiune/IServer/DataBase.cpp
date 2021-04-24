#include "DataBase.h"
#include <stdio.h>
#include <sqlite3.h>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include "IServer.h"
//DataBase* DataBase::instance = 0;
std::string final;
std::string finalText;
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
int DataBase::callbackString(void* data, int argc, char** argv, char** azColName)
{
	int i;
	fprintf(stderr, "%s: ", (const char*)data);
	std::string rasp("");

	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		std::string col(azColName[i]);
		std::string sol(argv[i] ? argv[i] : "NULL");
		rasp += "\"" + col + "\":" + "\"" + sol + "\"";

	}
	finalText = rasp;
	return 0;
}
int DataBase::callbackMore(void* data, int argc, char** argv, char** azColName)
{
	int i;
	fprintf(stderr, "%s: ", (const char*)data);
	std::string rasp = "{";
	std::string iduser, idnode;
	for (i = 0; i < argc ; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		std::string col(azColName[i]);
		std::string sol(argv[i] ? argv[i] : "NULL");
		rasp += "\"" + col + "\":" + "\"" + sol + "\",";
		if (i == 0)
			iduser = sol;
		if (i == 1)
			idnode = sol;
	}
	// acceseaza incarcarea notitei pt idnode ul primit
	IServer* sv=IServer::getInstance();
	std::string x=sv->getDatabase().getTextForNode(iduser, idnode);
	rasp += x;
	rasp += "},";
	final+=rasp;
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
		createNodeRoot(email);
		return true;
	}
	//creare nod radacina

}

std::string DataBase::loginUser(std::string email, std::string password)
{
	char* messaggeError;
	std::string sql("SELECT USER.iduser, USER.username, USER.password, MAX(NODE.idnode) as nrnodes FROM USER inner join NODE on USER.iduser =NODE.iduser where email like '" + email + "' GROUP BY USER.iduser, USER. username, USER.password");
	std::string data("CALLBACK FUNCTION");
	int exit = sqlite3_exec(DB, sql.c_str(), callback, (void*)data.c_str(), NULL);
	//adauga in final si nr de noduri pe care le are userul
	if (exit != SQLITE_OK)
	{
		std::cerr << "Error Select" << std::endl;
		return "";
	}
	else
	{
		std::string rasp = final;

		auto js = nlohmann::json::parse(final);
		std::string id = js["iduser"];
		final = "[" + rasp+",";
		selectAllNodes(std::stoi(id));
		return final;
	}
}


void DataBase::createTrigger()
{
	char* messaggeError;
	std::string data("CALLBACK FUNCTION");
	std::string sql(//"drop trigger stergere_nod;"
		"CREATE TRIGGER stergere_nod "
		"BEFORE DELETE ON NODE "
		"BEGIN "
		"DELETE FROM NODE WHERE idparent = old.idnode and iduser=old.iduser; DELETE FROM NODE WHERE iduser=old.iduser and idnode=old.idnode;  END;");
	//insert in notes varianta 0 a nodului
	int exit = sqlite3_exec(DB, sql.c_str(), callback, (void*)data.c_str(), NULL);
	if (exit != SQLITE_OK)
	{
		std::cerr << "Error <CreateTRIGGER> \n" << std::endl;
	}
	else
	{
		std::cout << "succes  <CreateTRIGGER> \n" << std::endl;
		//return selectIdForLastNode(iduser);
	}
}

void DataBase::createTable()
{

	//creare tabel si apoi stergere cod
	{
		std::string sql =//"DROP TABLE USER;"
			"CREATE TABLE USER("
			"iduser				integer NOT NULL, "
			"username           TEXT    NOT NULL, "
			"firstname          TEXT    NOT NULL, "
			"lastname			TEXT    NOT NULL, "
			"email			    TEXT	NOT NULL unique, "
			"password			TEXT    NOT NULL,"
			"PRIMARY KEY(iduser));";
		char* messaggeError;
		int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);

		if (exit != SQLITE_OK) {
			std::cerr << "Error Create Table USER" << std::endl;
			sqlite3_free(messaggeError);
		}
		else
			std::cout << "Table created USER Successfully" << std::endl;
	}

	{
		std::string sql =//"DROP TABLE NODE;"
			"CREATE TABLE NODE("	//CREAZA TABEL NODE, cheie primara dubla(idnod si iduser)
			"iduser		integer not null,"
			"idnode		integer not null,"
			"idparent	integer	not null,"
			"name		TEXT	not null,"
			"photoname	text	not null,"
			"Primary key(iduser,idnode),"
			"Foreign key(iduser) REFERENCES USER(iduser),"
			"Foreign key(idparent) REFERENCES NODE(idnode));";
			
		char* messaggeError;
		int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);

		if (exit != SQLITE_OK) {
			std::cerr << "Error Create Table NODE" << std::endl;
			sqlite3_free(messaggeError);
		}
		else
			std::cout << "Table created NODE Successfully" << std::endl;

	}
	{
		std::string sql = //  "drop table NOTES;"
			"CREATE TABLE NOTES ("
			"iduser		integer not null,"
			"idnode		integer not null,"
			"text		text,"
			"versiune	int,"
			"Primary key(iduser,idnode,versiune),"
			"Foreign key(iduser,idnode) REFERENCES NODE(iduser,idnode));";

		char* messaggeError;
		int exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);

		if (exit != SQLITE_OK) {
			std::cerr << "Error Create Table NOTES" << std::endl;
			sqlite3_free(messaggeError);
		}
		else
			std::cout << "Table created NOTES Successfully" << std::endl;

	}
}

void DataBase::createNodeRoot(std::string email)
{
	char* messaggeError;
	std::string sql("SELECT iduser FROM USER where email like '" + email + "'");
	std::string data("CALLBACK FUNCTION");
	int exit = sqlite3_exec(DB, sql.c_str(), callback, (void*)data.c_str(), NULL);
	//nu ii trebe ca si text nimic pt ca e un nod invizibil
	if (exit != SQLITE_OK)
	{
		std::cerr << "Error <CreateNodeRoot> Select" << std::endl;
	}
	else
	{
		std::cout << "Select  <CreateNodeRoot> successfully!" << std::endl;
		auto js = nlohmann::json::parse(final);
		std::string id = js["iduser"];
		std::string sql("INSERT INTO NODE(iduser,idnode,idparent,name,photoname) VALUES(" + id+",0,0"+",'root','-')");
		//adauga insertia in notes versiunea 0
		exit = sqlite3_exec(DB, sql.c_str(), callback, (void*)data.c_str(), NULL);
		if (exit != SQLITE_OK)
		{
			std::cerr << "Error <CreateNodeRoot> Insert" << std::endl;
		}
		else
		{
			std::cout << "Insert  <CreateNodeRoot> successfully!" << std::endl;
		}
	}
}

bool DataBase::insertNewNode(std::string iduser, std::string idparent, std::string name, std::string photo,std::string idnode)
{
	char* messaggeError;
	std::string data("CALLBACK FUNCTION");
	std::string sql("INSERT INTO NODE(iduser,idnode,idparent,name,photoname) VALUES(" + iduser + "," + idnode + "," + idparent + ",'" + name + "','" + photo + "');"
		+ "INSERT INTO NOTES(idnode, iduser,text,versiune) VALUES ("+idnode+","+iduser+",'Bine ai venit',0);");
	//insert in notes varianta 0 a nodului
	int exit = sqlite3_exec(DB, sql.c_str(), callback, (void*)data.c_str(), NULL);
	if (exit != SQLITE_OK)
	{
		std::cerr << "Error <CreateNode> Insert" << std::endl;
		return false;
	}
	else
	{
		std::cout << "Insert  <CreateNode> successfully!" << std::endl;
		return true;
		//return selectIdForLastNode(iduser);
	}

}

std::string DataBase::selectIdForLastNode(std::string iduser)
{
	char* messaggeError;
	std::string sql("SELECT max(NODE.idnode) FROM NODE WHERE NODE.iduser like '"+iduser+"'");
	std::string data("CALLBACK FUNCTION");
	int exit = sqlite3_exec(DB, sql.c_str(), callback, (void*)data.c_str(), NULL);
	//adauga in final si nr de noduri pe care le are userul
	if (exit != SQLITE_OK)
	{
		std::cerr << "Error Select" << std::endl;
		return "";
	}
	else
	{
		return final;
	}
}

std::string DataBase::selectAllNodes(int iduser)
{
	char* messaggeError;
	std::string sql("SELECT NODE.iduser, NODE.idnode, NODE.idparent, NODE.name, NODE.photoname FROM NODE where NODE.iduser like " + std::to_string(iduser) + " and NODE.idnode is not 0 ORDER BY NODE.idparent asc , NODE.idnode desc");
	std::string data("CALLBACK FUNCTION");
	//final = "[";
	int exit = sqlite3_exec(DB, sql.c_str(), callbackMore, (void*)data.c_str(), NULL);
	final[final.size()-1] = ']';
	//final[final.size() + 1] = '\n';
	std::cout << final << std::endl;
	//adauga in final si nr de noduri pe care le are userul
	if (exit != SQLITE_OK)
	{
		std::cerr << "Error Select<SELECT ALL NODES> \n" << std::endl;
		return "";
	}
	else
	{
		return final;
	}
}

bool DataBase::newVersionText(std::string text, std::string iduser, std::string idnode)
{
	char* messaggeError;
	std::string data("CALLBACK FUNCTION");
	std::string sql("INSERT INTO NOTES(iduser,idnode,text) VALUES "+ iduser+","+idnode+",'"+text+"'");
	//insert in notes varianta 0 a nodului
	int exit = sqlite3_exec(DB, sql.c_str(), callback, (void*)data.c_str(), NULL);
	if (exit != SQLITE_OK)
	{
		std::cerr << "Error <createVerion> Insert" << std::endl;
		return false;
	}
	else
	{
		std::cout << "Insert  <createVersion> successfully!" << std::endl;
		return true;
	}
}

std::string DataBase::getTextForNode(std::string iduser, std::string idnode)
{
	char* messaggeError;
	std::string sql("SELECT NOTES.text FROM NOTES where NOTES.iduser = " + iduser + " and NOTES.idnode = "+idnode+" order by NOTES.versiune desc limit 1");
	std::string data("CALLBACK FUNCTION");
	int exit = sqlite3_exec(DB, sql.c_str(), callbackString, (void*)data.c_str(), NULL);
	//adauga in final si nr de noduri pe care le are userul
	if (exit != SQLITE_OK)
	{
		std::cerr << "Error Select<SELECT TEXT NOTES> \n" << std::endl;
		return "";
	}
	else
	{
		return finalText;
	}
}

bool DataBase::removeNode(std::string id,std::string iduser)
{
	char* messaggeError;
	std::string sql("DELETE FROM NODE WHERE NODE.idnode = "+ id+" and NODE.iduser="+ iduser);
	std::string data("CALLBACK FUNCTION");
	int exit = sqlite3_exec(DB, sql.c_str(), NULL, NULL, &messaggeError);

	if (exit != SQLITE_OK)
	{
		std::cerr << "Error Delete<DELETE> \n" << std::endl;
		std::cout << "\n\n" << messaggeError << "\n\n";
		return false;
	}
	else
	{
		return true;
	}
	//stergere si din versiune textele de la toate versiunile
}
