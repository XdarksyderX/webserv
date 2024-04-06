/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 20:41:20 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/06 12:02:02 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "Logger.hpp"

int main(int argc, char **argv)
{
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

	return 0;
}
