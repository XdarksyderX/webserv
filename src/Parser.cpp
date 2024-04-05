/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 18:12:48 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/05 21:12:59 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

Parser::Parser(const std::string &configFilePath) : configFilePath(configFilePath)
{
	configFile.open(configFilePath.c_str());
	if (!configFile.is_open())
		throw Parser::FileNotFoundException();
}

Parser::Parser(const Parser &other)
	: configFilePath(other.configFilePath), configFile(), serverConfigs(other.serverConfigs)
{
	if (!configFilePath.empty())
		configFile.open(configFilePath.c_str());
	if (!configFile.is_open())
		throw Parser::FileNotFoundException();
}

Parser::~Parser()
{
	configFile.close();
}

Parser &Parser::operator=(const Parser &other)
{
	if (this != &other)
	{
		configFilePath = other.configFilePath;
		serverConfigs = other.serverConfigs;
		if (!configFilePath.empty())
		{
			configFile.close();
			configFile.open(configFilePath.c_str());
			if (!configFile.is_open())
				throw Parser::FileNotFoundException();
		}
	}
	return *this;
}

void Parser::parseLine(std::string &line)
{
	size_t commentPos = line.find('#');
	if (commentPos != std::string::npos)
		line.erase(commentPos);
	size_t semicolonPos = line.find(';');
	if (semicolonPos != std::string::npos)
	{
		line.erase(semicolonPos);
	}
	size_t start = line.find_first_not_of(" \t\n\v\f\r");
	if (start != std::string::npos)
		line = line.substr(start);

	else
	{
		line.clear();
		return;
	}
	size_t end = line.find_last_not_of(" \t\n\v\f\r");
	if (end != std::string::npos)
		line = line.substr(0, end + 1);
	else
		line.clear();
}

std::vector<ServerConfig> &Parser::parse()
{
	std::string line;
	ServerConfig serverConfig;

	while (std::getline(configFile, line))
	{
		std::istringstream iss(line);
		std::string word;
		iss >> word;
		if (word == "server")
			serverConfigs.push_back(parseServerBlock());
		else
			throw Parser::InvalidDirectiveException();
	}
	return serverConfigs;
}

ServerConfig Parser::parseServerBlock()
{
	ServerConfig serverConfig;
	std::string line;

	while (std::getline(configFile, line))
	{
		parseLine(line);
		std::istringstream iss(line);
		std::string word;
		if (!(iss >> word))
			continue;
		if (word == "}")
		{
			if (!serverConfig.getPort() || serverConfig.getHost().empty() || serverConfig.getServerName().empty() || serverConfig.getRoot().empty())
				throw Parser::MissingMandatoryException();
			return serverConfig;
		}
		else if (word == "listen")
		{
			int port;
			iss >> port;
			serverConfig.setPort(port);
		}
		else if (word == "host")
		{
			std::string host;
			iss >> host;
			serverConfig.setHost(host);
		}
		else if (word == "server_name")
		{
			std::string serverName;
			iss >> serverName;
			serverConfig.setServerName(serverName);
		}
		else if (word == "root")
		{
			std::string root;
			iss >> root;
			serverConfig.setRoot(root);
		}
		else if (word == "index")
		{
			std::string index;
			iss >> index;
			serverConfig.setIndex(index);
		}
		else if (word == "error_page")
		{
			int code;
			std::string path;
			iss >> code >> path;
			serverConfig.addErrorPage(code, path);
		}
		else if (word == "client_max_body_size")
		{
			size_t size;
			iss >> size;
			serverConfig.setClientMaxBodySize(size);
		}
		else if (word == "location")
		{
			std::string path;
			iss >> path;
			LocationConfig locationConfig = parseLocationBlock(serverConfig);
			serverConfig.addLocation(path, locationConfig);
		}
		else
			throw Parser::InvalidDirectiveException();
	}
	throw Parser::SyntaxErrorException();
}

LocationConfig Parser::parseLocationBlock(ServerConfig &serverConfig)
{
	LocationConfig locationConfig(serverConfig.getRoot());
	std::string line;

	while (std::getline(configFile, line))
	{
		parseLine(line);
		std::istringstream iss(line);
		std::string word;
		if (!(iss >> word))
			continue;
		if (word == "}")
			return locationConfig;
		else if (word == "root")
		{
			std::string root;
			iss >> root;
			locationConfig.setRoot(root);
		}
		else if (word == "index")
		{
			std::string index;
			iss >> index;
			locationConfig.setIndex(index);
		}
		else if (word == "allow_methods")
		{
			std::string methods;
			while (iss >> methods)
			{
				locationConfig.addAllowMethod(methods);
			}
		}
		else if (word == "alias")
		{
			std::string alias;
			iss >> alias;
			locationConfig.setAlias(alias);
		}
		else if (word == "autoindex")
		{
			std::string autoindex;
			iss >> autoindex;
			locationConfig.setAutoindex(autoindex == "on");
		}
		else if (word == "return")
		{
			std::string path;
			iss >> path;
			locationConfig.setReturnPath(path);
		}
		else
		{
			std::cout << "DIRECTIVA QUE FALLA: " << word << std::endl;
			throw Parser::InvalidDirectiveException();
		}
	}
	throw Parser::SyntaxErrorException();
}

const char *Parser::FileNotFoundException::what() const throw()
{
	return "Configuration file not found.";
}

const char *Parser::SyntaxErrorException::what() const throw()
{
	return "Syntax error in configuration file.";
}

const char *Parser::InvalidDirectiveException::what() const throw()
{
	return "Invalid directive found in configuration file.";
}

const char *Parser::MissingMandatoryException::what() const throw()
{
	return "Missing mandatory configuration setting.";
}
