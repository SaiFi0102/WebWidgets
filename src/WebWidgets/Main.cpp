#include <iostream>
#include "Application/WServer.h"
#include "Application/Application.h"

int main(int argc, char **argv)
{
	/* *************************************************************************
	 * *************************  Start Web Server *****************************
	 * *************************************************************************/
	WServer Server = WServer(argv[0], "wt_config.xml");
	try
	{
		//Initialize Server
		Server.Initialize();

		//Configuration
		Server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);
		Server.addEntryPoint(Wt::Application, Application::CreateApplication);
		
		//Start Server
		if(Server.Start())
		{
			//And wait till a shutdown signal is given
			Wt::WServer::waitForShutdown();
			Server.stop();
		}
	}
	catch(Wt::WServer::Exception &e)
	{
		Server.log("fatal") << "Error starting the server: " << e.what();
	}
	catch(std::exception &e)
	{
		Server.log("fatal") << "Server exception error: " << e.what();
	}
}