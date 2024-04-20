/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 23:27:44 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/20 17:42:56 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

ServerConfig Parser::parseServerBlock()
{
	ServerConfig serverConfig;
	std::string line;
	std::stack<char> bracesStack;
	bracesStack.push('{');

	while (std::getline(configFile, line))
	{
		parseLine(line);
		if (line.empty() || line == "{")
		{
			if (line == "{")
			{
				bracesStack.push('{');
			}
			continue;
		}
		if (line == "}")
		{
			if (bracesStack.empty())
			{
				throw Parser::SyntaxErrorException();
			}
			bracesStack.pop();
			if (bracesStack.empty())
			{
				break;
			}
			continue;
		}
		if (!bracesStack.empty())
			processServerLine(line, serverConfig);
	}
	if (!bracesStack.empty())
		throw Parser::SyntaxErrorException();
	const std::map<std::string, LocationConfig>& locations = serverConfig.getLocations();
	std::map<std::string, LocationConfig>::const_iterator it = locations.find("/");
	std::map<std::string, LocationConfig>::const_iterator itEnd = locations.end();
	if (it == itEnd)
	{
		LocationConfig defaultLocation = LocationConfig::createDefaultLocation(serverConfig);
		serverConfig.addLocation("/", defaultLocation);
	}
	return serverConfig;
}

void Parser::processServerLine(const std::string &line, ServerConfig &serverConfig)
{
	std::istringstream iss(line);
	std::string word;
	if (!(iss >> word))
		return;
	if (word == "listen")
		processListenDirective(iss, serverConfig);
	else if (word == "host")
		processHostDirective(iss, serverConfig);
	else if (word == "server_name")
		processServerNameDirective(iss, serverConfig);
	else if (word == "root")
		processRootDirective(iss, serverConfig);
	else if (word == "index")
		processIndexDirective(iss, serverConfig);
	else if (word == "error_page")
		processErrorPageDirective(iss, serverConfig);
	else if (word == "client_max_body_size")
		processClientMaxBodySizeDirective(iss, serverConfig);
	else if (word == "location")
		processLocationDirective(iss, serverConfig);
	else if (word == "upload_dir")
		processUploadsDirectoryDirective(iss, serverConfig);
	else
	{
		throw Parser::InvalidDirectiveException();
	}
}

void Parser::processListenDirective(std::istringstream &iss, ServerConfig &serverConfig)
{
	int port;
	if (!(iss >> port))
		throw Parser::InvalidDirectiveException();
	serverConfig.setPort(port);
}

void Parser::processHostDirective(std::istringstream &iss, ServerConfig &serverConfig)
{
	std::string host;
	if (!(iss >> host))
		throw Parser::InvalidDirectiveException();
	serverConfig.setHost(host);
}

void Parser::processServerNameDirective(std::istringstream &iss, ServerConfig &serverConfig)
{
	std::string serverName;
	if (!(iss >> serverName))
		throw Parser::InvalidDirectiveException();
	serverConfig.setServerName(serverName);
}

void Parser::processRootDirective(std::istringstream &iss, ServerConfig &serverConfig)
{
	std::string root;
	if (!(iss >> root))
		throw Parser::InvalidDirectiveException();
	if (!Utils::directoryExists(root))
		throw Parser::ResourceNotFoundException();
	serverConfig.setRoot(root);
}

void Parser::processIndexDirective(std::istringstream &iss, ServerConfig &serverConfig)
{
	std::string index;
	if (!(iss >> index))
		throw Parser::InvalidDirectiveException();
	serverConfig.setIndex(index);
}

void Parser::processErrorPageDirective(std::istringstream &iss, ServerConfig &serverConfig)
{
	int code;
	std::string path;
	if (!(iss >> code >> path))
		throw Parser::InvalidDirectiveException();
	if (!Utils::fileExists(Utils::joinPaths(serverConfig.getRoot(), path)))
		throw Parser::ResourceNotFoundException();
	serverConfig.addErrorPage(code, path);
}

void Parser::processClientMaxBodySizeDirective(std::istringstream &iss, ServerConfig &serverConfig)
{
	size_t size;
	if (!(iss >> size))
		throw Parser::InvalidDirectiveException();
	serverConfig.setClientMaxBodySize(size);
}

void Parser::processLocationDirective(std::istringstream &iss, ServerConfig &serverConfig)
{
    std::string path;
    if (!(iss >> path) || path.empty())
        throw Parser::InvalidDirectiveException();

    std::string name = (path[0] == '/') ? path.substr(1) : path;
    LocationConfig locationConfig = parseLocationBlock(serverConfig, name);
    locationConfig.setName(name);
    serverConfig.addLocation(path, locationConfig);
}
void Parser::processUploadsDirectoryDirective(std::istringstream &iss, ServerConfig &serverConfig)
{
	std::string path;
	if (!(iss >> path))
		throw Parser::InvalidDirectiveException();
	if (!Utils::directoryExists(path))
		throw Parser::ResourceNotFoundException();
	serverConfig.setUploadsDirectory(path);
}