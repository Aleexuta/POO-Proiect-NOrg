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
    CreateNode,
    OpenNode,
    SaveNode,
    LoadNodes
};
class IServer : public olc::net::server_interface<CustomMsgTypes>
{
public:
    IServer(uint16_t nPort): 
        olc::net::server_interface<CustomMsgTypes>(nPort)
    {
    }
    ~IServer();
    void connectDataBase();
    void createTable();
protected:
	virtual bool OnClientConnect(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client);
	// Called when a client appears to have disconnected
	virtual void OnClientDisconnect(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client)
	{
		std::cout << "Removing client [" << client->GetID() << "]\n";
	}
	virtual void OnMessage(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client, olc::net::message<CustomMsgTypes>& msg);
    
    bool RegisterUser(std::string j);


private:
    DataBase DB;
};