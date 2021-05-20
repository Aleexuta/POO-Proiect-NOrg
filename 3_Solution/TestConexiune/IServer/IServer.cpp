// IServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include "IServer.h"
IServer* IServer::instance = nullptr;
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
		std::cout << "Register Case\t";
		std::string str(msg.body.begin(),msg.body.end());
		//se coneccteaza la functia pt baza de date
		if (RegisterUser(str))
		{
			std::cout << "Adaugare User Succes\n";
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
			std::cout << "Adaugare User Error\n";
		}
	}
	break;
	case CustomMsgTypes::ServerLogin:
	{
		std::cout << "Login Case\t";
		std::string str(msg.body.begin(), msg.body.end());
		std::string rasp = LoginUser(str);
		if (rasp!="")
		{
			std::cout << "Login User Succes\n";
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
			std::cout << "Login User Error\n";
		}
		//se coneccteaza la functia pt baza de date
	}
	break;
	case CustomMsgTypes::NewNode:
	{
		std::cout << "New Node Case\t";
		std::string str(msg.body.begin(), msg.body.end());
		if (InsertNewNode(str))
		{
			std::cout << "Insert New Node Succes\n";
			//trimite mesaj cu id ul nodului inapoi

		}
		else
		{
			std::cout << "Insert New Node Error\n";

		}
	}
	break;
	case CustomMsgTypes::RemoveNode:
	{
		std::cout << "Remove node case\n";
		std::string str(msg.body.begin(), msg.body.end());
		if (removeNode(str))
		{
			std::cout << "Delete Node Succes\n";
			//trimite mesaj cu id ul nodului inapoi
		}
		else
		{
			std::cout << "Delete Node Error\n";
		}
	}
	break;
	case CustomMsgTypes::SaveNode:
	{
		std::string str(msg.body.begin(), msg.body.end());
		if (saveNode(str))
		{
			std::cout << "Save Node succes \n";
		}
		std::cout << "Save Node error\n";
	}
	break;
	case CustomMsgTypes::LoadAllNodes:
	{
		std::string str(msg.body.begin(), msg.body.end());
		std::string rasp = loadAllNodes(str);
		if (rasp != "")
		{
			std::cout << "Load All Nodes Succes\n";
			olc::net::message<CustomMsgTypes> msg2;
			msg2.header.id = CustomMsgTypes::LoadAllNodesAccept;
			for (int i = 0; i < rasp.size(); i++)
				msg2 << rasp[i];

			client->Send(msg2);
		}
	}
	break;
	case CustomMsgTypes::MoveToTrashNode:
	{
		std::cout << "Move node case\n";
		std::string str(msg.body.begin(), msg.body.end());
		if (moveNode(str))
		{
			std::cout << "Move Node Succes\n";
			//trimite mesaj cu id ul nodului inapoi
		}
		else
		{
			std::cout << "Move Node Error\n";
		}
	}
	break;
	case CustomMsgTypes::MoveFromTrashNode:
	{
		std::cout << "Move node case\n";
		std::string str(msg.body.begin(), msg.body.end());
		if (recoverNode(str))
		{
			std::cout << "Move Node Succes\n";
			//trimite mesaj cu id ul nodului inapoi
		}
		else
		{
			std::cout << "Move Node Error\n";
		}
	}
	break;
	case CustomMsgTypes::ChangePassword:
	{
		std::cout << "Change pass case\n";
		std::string str(msg.body.begin(), msg.body.end());
		if (changePass(str))
		{
			std::cout << "Change Password Succes\n";
			olc::net::message<CustomMsgTypes> msg2;
			msg2.header.id = CustomMsgTypes::ChangePasswordSucces;
			client->Send(msg2);
		}
		else
		{
			std::cout << "Change Password Error\n";
			olc::net::message<CustomMsgTypes> msg2;
			msg2.header.id = CustomMsgTypes::ChangePasswordError;
			client->Send(msg2);
		}
	}
	break;
	case CustomMsgTypes::DeleteAccount:
	{
		std::cout << "Delete account case\n";
		std::string str(msg.body.begin(), msg.body.end());
		if (deleteAcc(str))
		{
			std::cout << "Delete account Succes\n";
			olc::net::message<CustomMsgTypes> msg2;
			msg2.header.id = CustomMsgTypes::DeleteAccountSucces;
			client->Send(msg2);
		}
		else
		{
			std::cout << "Delete Account Error\n";
			olc::net::message<CustomMsgTypes> msg2;
			msg2.header.id = CustomMsgTypes::DeleteAccountError;
			client->Send(msg2);
		}
	}
	break;
	}
}

bool IServer::RegisterUser(std::string j)
{
	try
	{
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
			std::cout << rasp << "\n";
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
		return "";
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
		std::string color = js["color"];
		std::string font = js["font"];
		std::string date = js["date"];
		return DB.insertNewNode(iduser, idparent, name, photoname,idnode,color,font,date);

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
		return "";
		std::cout << "\n Eroare la LoadAllNodes(IServer)";
	}
}

bool IServer::removeNode(std::string j)
{
	try 
	{
		auto js = nlohmann::json::parse(j);//array cu json
		for (int i = 0; i < js.size(); i++)
		{
			std::string id = js[i]["idnode"];
			std::string iduser = js[i]["iduser"];
			if (!DB.removeNode(id, iduser))
				return false;
		}
		return true;
	}
	catch (...)
	{
		std::cout << "\nEroare la removeNode(IServer)";
	}
}

bool IServer::saveNode(std::string j)
{
	try
	{
		auto js = nlohmann::json::parse(j);
		std::string iduser = js["iduser"];
		std::string idnode = js["idnode"];
		std::string text = js["text"];
		return DB.newVersionText(text,iduser,idnode);
	}
	catch (...)
	{
		std::cout << "Eroare la saveNode(Iserver)\n";
	}
}

bool IServer::moveNode(std::string j)
{
	try
	{
		auto js = nlohmann::json::parse(j);//array cu json
		std::string id = js["idnode"];
		std::string iduser = js["iduser"];
		std::string idparent = js["idparent"];
		return DB.moveToTrashNode(iduser, id,idparent);
	}
	catch (...)
	{
		std::cout << "\nEroare la moveNode(IServer)";
	}
}

bool IServer::recoverNode(std::string j)
{
	try
	{
		auto js = nlohmann::json::parse(j);//array cu json
		std::string id = js["idnode"];
		std::string iduser = js["iduser"];
		std::string idoldparent = js["idoldparent"];
		return DB.moveFromTrashNode(iduser, id,idoldparent);
	}
	catch (...)
	{
		std::cout << "\nEroare la moveNode(IServer)";
		return false;
	}
}

bool IServer::changePass(std::string j)
{
	try
	{
		auto js = nlohmann::json::parse(j);
		std::string iduser = js["iduser"];
		std::string pass = js["newpass"];
		std::string old = js["oldpass"];
		return DB.updatePasswordUser(iduser, old, pass);
	}
	catch (...)
	{
		std::cout << "\nEroare la changePass(IServer)";
		return false;
	}
}

bool IServer::deleteAcc(std::string j)
{
	try
	{
		auto js = nlohmann::json::parse(j);
		std::string iduser = js["iduser"];
		return DB.deleteUser(iduser);
	}
	catch (...)
	{
		std::cout << "\nEroare la deleteAcc(IServer)";
		return false;
	}
}


IServer::~IServer()
{
	DB.closeDataBase();
}

IServer* IServer::getInstance(uint16_t nPort)
{
	if (!instance)
		instance = new IServer(nPort);
	return instance;
}

IServer* IServer::getInstance()
{
	return instance;
}

void IServer::deleteInstance()
{
	if (instance)
		delete instance;
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

DataBase IServer::getDatabase()
{
	return DB;
}
