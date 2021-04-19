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
			olc::net::message<CustomMsgTypes> msg2;
			msg2.header.id = CustomMsgTypes::ServerAcceptRegister;
			client->Send(msg2);
			//trimite un mesaj catre client cu s a efectuat cu succes
		}
		else
		{
			olc::net::message<CustomMsgTypes> msg2;
			msg2.header.id = CustomMsgTypes::ServerDenyRegister;
			client->Send(msg2);
			std::cout << "\nAdaugare User Error";
		}
	}
	break;
	case CustomMsgTypes::ServerLogin:
	{
		std::cout << "Login Case";
		std::string str(msg.body.begin(), msg.body.end());
		std::string rasp = LoginUser(str);
		if (rasp!="")
		{
			std::cout << "Login User Succes";
			olc::net::message<CustomMsgTypes> msg2;
			msg2.header.id = CustomMsgTypes::ServerAcceptLogin;
			for (int i = 0; i < rasp.size(); i++)
				msg2 << rasp[i];
			
			client->Send(msg2);
		}
		else
		{
			olc::net::message<CustomMsgTypes> msg2;
			msg2.header.id = CustomMsgTypes::ServerDenyLogin;
			client->Send(msg2);
			std::cout << "\nLogin User Error";
		}
		//se coneccteaza la functia pt baza de date
	}
	break;
	case CustomMsgTypes::NewNode:
	{
		std::cout << "New Node Case";
		std::string str(msg.body.begin(), msg.body.end());
		if (InsertNewNode(str))
		{
			std::cout << "Insert New Node Succes";
			//trimite mesaj cu id ul nodului inapoi

		}
		else
		{
			std::cout << "\nInsert New Node Error";

		}
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

	case CustomMsgTypes::OpenNode:
	{

	}
	break;
	case CustomMsgTypes::SaveNode:
	{

	}
	break;
	case CustomMsgTypes::LoadAllNodes:
	{
		std::string str(msg.body.begin(), msg.body.end());
		std::string rasp = loadAllNodes(str);
		if (rasp != "")
		{
			std::cout << "Load All Nodes Succes";
			olc::net::message<CustomMsgTypes> msg2;
			msg2.header.id = CustomMsgTypes::LoadAllNodesAccept;
			for (int i = 0; i < rasp.size(); i++)
				msg2 << rasp[i];

			client->Send(msg2);
		}
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
		if (DB.insertUser(username, firstname, lastname, email, password))
		{			
			return true;
		}
		return false;
	}
	catch (...)
	{
		std::cout << "\nEroare la RegisterUser(ISERVER)";
		return false;
	}
	
}

std::string IServer::LoginUser(std::string j)
{
	try
	{
		auto js = nlohmann::json::parse(j);
		std::string email = js["email"];
		std::string password = js["password"];
		std::string rasp = DB.loginUser(email, password);

		if (rasp!="")
		{
			auto js2 = nlohmann::json::parse(rasp);
			std::string passCorect = js2[0]["password"];
			if (passCorect!= password)
			{
				return "";
			}
			return rasp;
		}
		return "";
	}
	catch (...)
	{
		std::cout << "\n Eroare la LoginUser(IServer)";
	}
}

bool IServer::InsertNewNode(std::string j)
{

	try
	{
		auto js = nlohmann::json::parse(j);
		std::string iduser = js["iduser"];
		std::string idparent = js["idparent"];
		std::string name = js["name"];
		std::string photoname = js["photoname"];
		std::string idnode = js["idnode"];
		return DB.insertNewNode(iduser, idparent, name, photoname,idnode);

	}
	catch (...)
	{
		std::cout << "\n Eroare la InsertNewNode(IServer)";
	}
}

std::string IServer::loadAllNodes(std::string j)
{
	try
	{

		return DB.selectAllNodes(std::stoi(j));

	}
		catch (...)
	{
		std::cout << "\n Eroare la LoadAllNodes(IServer)";
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
