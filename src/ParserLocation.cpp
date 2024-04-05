/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserLocation.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 23:28:14 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/05 23:45:32 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

LocationConfig Parser::parseLocationBlock(ServerConfig &serverConfig)
{
    LocationConfig locationConfig(serverConfig.getRoot());
    std::string line;
    std::stack<char> bracesStack;
    bracesStack.push('{');

    while (std::getline(configFile, line))
    {
        parseLine(line);
        if (line.find("}") != std::string::npos)
        {
            bracesStack.pop();
            if (bracesStack.empty())
                break;
        }
        if (!bracesStack.empty())
            processLocationLine(line, locationConfig);
    }
    if (!bracesStack.empty())
        throw Parser::SyntaxErrorException();
    return locationConfig;
}

void Parser::processLocationLine(const std::string &line, LocationConfig &locationConfig)
{
    std::istringstream iss(line);
    std::string word;
    if (!(iss >> word))
        return;
    if (word == "root")
        processLocationRootDirective(iss, locationConfig);
    else if (word == "index")
        processLocationIndexDirective(iss, locationConfig);
    else if (word == "allow_methods")
        processAllowMethodsDirective(iss, locationConfig);
    else if (word == "alias")
        processAliasDirective(iss, locationConfig);
    else if (word == "autoindex")
        processAutoindexDirective(iss, locationConfig);
    else if (word == "return")
        processReturnDirective(iss, locationConfig);
    else if (word == "cgi_path")
        processCgiPathDirective(iss, locationConfig);
    else if (word == "cgi_ext")
        processCgiExtDirective(iss, locationConfig);
    else
	{
		std::cout << "Invalid directive LOCATION: " << word << std::endl;
        throw Parser::InvalidDirectiveException();
	}
}

void Parser::processLocationRootDirective(std::istringstream &iss, LocationConfig &locationConfig)
{
    std::string root;
    if (!(iss >> root))
        throw Parser::InvalidDirectiveException();
    locationConfig.setRoot(root);
}

void Parser::processLocationIndexDirective(std::istringstream &iss, LocationConfig &locationConfig)
{
    std::string index;
    if (!(iss >> index))
        throw Parser::InvalidDirectiveException();
    locationConfig.setIndex(index);
}

void Parser::processAllowMethodsDirective(std::istringstream &iss, LocationConfig &locationConfig)
{
    std::string method;
    std::vector<std::string> methods;
    while (iss >> method)
    {
        methods.push_back(method);
    }
    if (methods.empty())
        throw Parser::InvalidDirectiveException();
    locationConfig.setAllowMethods(methods);
}

void Parser::processAliasDirective(std::istringstream &iss, LocationConfig &locationConfig)
{
    std::string alias;
    if (!(iss >> alias))
        throw Parser::InvalidDirectiveException();
    locationConfig.setAlias(alias);
}

void Parser::processAutoindexDirective(std::istringstream &iss, LocationConfig &locationConfig)
{
    std::string autoindex;
    if (!(iss >> autoindex))
        throw Parser::InvalidDirectiveException();
    locationConfig.setAutoindex(autoindex == "on");
}

void Parser::processReturnDirective(std::istringstream &iss, LocationConfig &locationConfig)
{
    std::string returnPath;
    if (!(iss >> returnPath))
        throw Parser::InvalidDirectiveException();
    locationConfig.setReturnPath(returnPath);
}

void Parser::processCgiPathDirective(std::istringstream &iss, LocationConfig &locationConfig)
{
    std::string cgiPath;
    if (!(iss >> cgiPath))
        throw Parser::InvalidDirectiveException();
    locationConfig.addCgiPath(cgiPath);
}

void Parser::processCgiExtDirective(std::istringstream &iss, LocationConfig &locationConfig)
{
    std::string extension;
    std::vector<std::string> extensions;
    while (iss >> extension)
        locationConfig.addCgiExtension(extension);
    if (locationConfig.getCgiExtensions().empty())
        throw Parser::InvalidDirectiveException();
}
