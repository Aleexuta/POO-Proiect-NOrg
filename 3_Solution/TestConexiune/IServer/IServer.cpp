// IServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include "IServer.h"
bool IServer::OnClientConnect(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client)
{
	olc::net::message<CustomMsgTypes> msg;
	msg.header.id = CustomMsgTypes::ServerAccept;
	client->Send(msg);
	return true;
}

void IServer::OnMessage(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client, olc::net::message<CustomMsgTypes>& msg)
{
	switch (msg.header.id)
	{
	case CustomMsgTypes::ServerRegister:
	{
		std::cout << "Register Case";
		std::string str(msg.body.begin(),msg.body.end());
		//se coneccteaza la functia pt baza de date
		if (RegisterUser(str))
		{
			std::cout << "\n Adaugare User Succes";
		}
		else
		{
			std::cout << "\nAdaugare User Error";
		}
	}
	break;
	case CustomMsgTypes::ServerLogin:
	{

	}
	break;
	case CustomMsgTypes::ServerAccept:
	{

	}
	break;
	case CustomMsgTypes::ServerDeny:
	{

	}
	break;
	case CustomMsgTypes::CreateNode:
	{

	}
	break;
	case CustomMsgTypes::OpenNode:
	{

	}
	break;
	case CustomMsgTypes::SaveNode:
	{

	}
	break;
	case CustomMsgTypes::LoadNodes:
	{

	}
	break;
	}
}

bool IServer::RegisterUser(std::string j)
{
	try
	{//nu face ceva bine
		//auto src= j.find("username");
		auto js = nlohmann::json::parse(j);
		std::cout << j<<"\n\n\n\n";
		std::string username=js["username"];
		std::string firstname=js["firstname"];
		std::string lastname=js["lastname"];
		std::string email=js["email"];
		std::string password=js["password"];
		//aici ^^^
		if (DB.insertUser(username, firstname,lastname,email, password))
			return true;
		return false;
	}
	catch (...)
	{
		std::cout << "Eroare la RegisterUser(ISERVER)";
	}
}


IServer::~IServer()
{
	DB.closeDataBase();
}

void IServer::connectDataBase()
{
	if (!DB.connectToDataBase())
		throw "EROARE LA DESCHIDEREA BAZEI DE DATE";

}

void IServer::createTable()
{
	DB.createTable();
}
