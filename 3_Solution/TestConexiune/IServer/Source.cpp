
#include "IServer.h"


int main()
{
	IServer server(50000);
	server.Start();
	try
	{
		server.connectDataBase();
		server.createTable();
	}
	catch (const char*mess)
	{
		std::cout << mess;
	}
	while (1)
	{
		server.Update(-1, true);
	}

	return 0;
}