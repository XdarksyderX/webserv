/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 20:41:20 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/05 20:42:52 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "Logger.hpp"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		Logger::log("Usage: ./webserv <config_file>", Logger::ERROR);
		return 1;
	}

	try
	{
		Parser parser(argv[1]);
		std::vector<ServerConfig> serverConfigs = parser.parse();
		for (size_t i = 0; i < serverConfigs.size(); i++)
			std::cout << serverConfigs[i] << std::endl;
	}
	catch (const std::exception &e)
	{
		Logger::log(e.what(), Logger::ERROR);
		return 1;
	}

	return 0;
}