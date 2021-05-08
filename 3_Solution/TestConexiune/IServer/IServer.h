#pragma once
#include <olc_net.h>
#include <nlohmann/json.hpp>
#include "DataBase.h"
enum class CustomMsgTypes : uint32_t
{
	ServerAccept,
	ServerAcceptRegister,
	ServerDenyRegister,
	ServerAcceptLogin,
	ServerDenyLogin,
	ServerDeny,
	ServerRegister,
	ServerLogin,
	NewNode,
	NewNodeAccept,
	NewNodeDeny,
	LoadAllNodes,
	LoadAllNodesAccept,
	RemoveNode,
	MoveToTrashNode,
	MoveFromTrashNode,
	SaveNode,
	DeleteAccount,
	DeleteAccountSucces,
	DeleteAccountError,
	ChangePassword,
	ChangePasswordSucces,
	ChangePasswordError
};
class DataBase;
class IServer : public olc::net::server_interface<CustomMsgTypes>
{
public:
	static IServer* getInstance(uint16_t nPort);
	static IServer* getInstance();
	void deleteInstance();
	void connectDataBase();
	void createTable();
	DataBase getDatabase();
protected:

	virtual bool OnClientConnect(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client);
	// Called when a client appears to have disconnected
	virtual void OnClientDisconnect(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client)
	{
		std::cout << "Removing client [" << client->GetID() << "]\n";
	}
	virtual void OnMessage(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client, olc::net::message<CustomMsgTypes>& msg);
private:   
	bool RegisterUser(std::string j);
	std::string LoginUser(std::string j);
	bool InsertNewNode(std::string j);
	std::string loadAllNodes(std::string j);
	bool removeNode(std::string j);
	bool saveNode(std::string j);
	bool moveNode(std::string j);
	bool recoverNode(std::string j);
	bool changePass(std::string j);
	bool deleteAcc(std::string j);
 private:
	static IServer* instance;
	IServer(uint16_t nPort) :
		olc::net::server_interface<CustomMsgTypes>(nPort)
	{
	}
	~IServer();
	DataBase DB;
};