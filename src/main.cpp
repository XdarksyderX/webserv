/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 20:41:20 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/11 22:57:51 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "Logger.hpp"

#include "ServerManager.hpp"

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
	ServerManager serverManager;
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
						+ Logger::to_string(static_cast<int>(serverConfigs[i].getPort()))
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
