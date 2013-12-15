#include <iostream>
#include "Application/WServer.h"
#include "Application/Application.h"

int main(int argc, char **argv)
{
	/* *************************************************************************
	 * *************************  Start Web Server *****************************
	 * *************************************************************************/
	Wt::WLogger Logger;

	try
	{
		//Initialize Server
		WServer Server(Logger, argv[0], "wt_config.xml");

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
		Logger.entry("fatal") << Wt::WLogger::timestamp << Wt::WLogger::sep << "[Fatal]" << Wt::WLogger::sep << "Error starting the server: " << e.what();
	}
	catch(std::exception &e)
	{
		Logger.entry("fatal") << Wt::WLogger::timestamp << Wt::WLogger::sep << "[Fatal]" << Wt::WLogger::sep << "Server exception error: " << e.what();
	}
}