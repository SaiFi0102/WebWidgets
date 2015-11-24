#include <iostream>
#include "Application/WServer.h"
#include "Application/Application.h"

int main(int argc, char **argv)
{
	/* *************************************************************************
	 * *************************  Start Web Server *****************************
	 * *************************************************************************/
// 	try
// 	{
		//Initialize Server
		WServer Server = WServer(argv[0], "wt_config.xml");
		Server.Initialize();

		//Configuration
		Server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);
		Server.addEntryPoint(Wt::Application, Application::CreateApplication);
		
		//Start Server
		if(Server.Start())
		{
			//And wait till a shutdown signal is given
			int sig = WServer::waitForShutdown(argv[0]);
			Server.stop();
			Wt::log("info") << "Shutdown (Signal = " << sig << ")";
		}
// 	}
// 	catch(Wt::WServer::Exception &e)
// 	{
// 		Wt::log("fatal") << "Error starting the server: " << e.what();
// 		exit(EXIT_FAILURE);
// 	}
// 	catch(std::exception &e)
// 	{
// 		Wt::log("fatal") << "Server exception error: " << e.what();
// 		exit(EXIT_FAILURE);
// 	}
}