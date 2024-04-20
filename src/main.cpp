/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 20:41:20 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/20 22:46:09 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <csignal>

#include "Parser.hpp"
#include "Logger.hpp"

#include "ServerManager.hpp"

ServerManager serverManager;

void	signalHandler(int signal)
{
	if (signal == SIGINT)
	{
		std::cout << "\033[?1049h\033[H";

		std::string input;
		Logger::log("Are you sure you want to stop the servers? (y/N): ", Logger::WARNING, false, false);
		std::getline(std::cin, input);
		std::cout << "\033[?1049l";
		if (input.empty() || (input[0] != 'y' && input[0] != 'Y'))
		{
            Logger::log("Servers will continue running...", Logger::INFO, false);
            return;
        }
		std::cout << std::endl;
		Logger::log("Stopping servers...", Logger::INFO);
		serverManager.stopServers();
	}
}

int main(int argc, char **argv)
{
	Logger::logHeader();
	std::string configFile = "config/webserv.conf";
	if (argc == 2)
		configFile = argv[1];
	else if (argc > 2)
	{
		Logger::log("Usage: ./webserv [config_file]", Logger::ERROR);
		return 1;
	}

	signal(SIGINT, signalHandler);

	std::vector<ServerConfig> serverConfigs;
	try
	{
		Parser parser(configFile);
		serverConfigs = parser.parse();
	}
	catch (const std::exception &e)
	{
		Logger::log(e.what(), Logger::ERROR);
		return 1;
	}

	int serverCount = 0;
	for (size_t i = 0; i < serverConfigs.size(); i++)
	{
		try
		{
			serverManager.addServer(new HTTPServer(serverConfigs[i]));
			std::string serverName = (serverConfigs[i].getServerName() == "default") ? "" : serverConfigs[i].getServerName() + " ";
			Logger::log("Server " + serverName
						+ "started on "
						+ serverConfigs[i].getHost()
						+ ":"
						+ Utils::to_string(static_cast<int>(serverConfigs[i].getPort()))
						+ "",
					Logger::INFO);
			serverCount++;
		}
		catch (const std::exception &e)
		{
			Logger::log(e.what(), Logger::ERROR);
			return 1;
		}
	}
	std::cout << std::endl;
	try {
		serverManager.startServers();
	}
	catch (const std::exception &e)
	{
		Logger::log(e.what(), Logger::ERROR);
		return 1;
	}

	for (size_t i = 0; i < serverManager.getServers().size(); i++)
		delete serverManager.getServers()[i];

	return 0;
}
