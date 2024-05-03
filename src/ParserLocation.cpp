/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParserLocation.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erivero- <erivero-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 23:28:14 by migarci2          #+#    #+#             */
/*   Updated: 2024/05/01 14:50:41 by erivero-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

LocationConfig Parser::parseLocationBlock(ServerConfig &serverConfig, const std::string &name)
{
    LocationConfig locationConfig(serverConfig.getRoot(), name);
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
            processLocationLine(line, locationConfig, serverConfig);
    }
    if (!bracesStack.empty())
        throw Parser::SyntaxErrorException();
    return locationConfig;
}

void Parser::processLocationLine(const std::string &line, LocationConfig &locationConfig,
    ServerConfig &serverConfig)
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
    else if (word == "autoindex")
        processAutoindexDirective(iss, locationConfig);
    else if (word == "cgi_path")
        processCgiPathDirective(iss, locationConfig);
    else if (word == "cgi_ext")
        processCgiExtDirective(iss, locationConfig);
    else if (word == "upload_path")
        processUploadPathDirective(iss, locationConfig, serverConfig);
    else
	{
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
    std::vector<HTTPMethod> methods;
    while (iss >> method)
    {
        if (method == "GET")
            methods.push_back(GET);
        else if (method == "POST")
            methods.push_back(POST);
        else if (method == "DELETE")
            methods.push_back(DELETE);
        else if (method == "PUT")
            methods.push_back(PUT);
        else
            throw Parser::InvalidDirectiveException();
    }
    if (methods.empty())
        throw Parser::InvalidDirectiveException();
    locationConfig.setAllowMethods(methods);
}

void Parser::processAutoindexDirective(std::istringstream &iss, LocationConfig &locationConfig)
{
    std::string autoindex;
    if (!(iss >> autoindex))
        throw Parser::InvalidDirectiveException();
    locationConfig.setAutoindex(autoindex == "on");
}

void Parser::processCgiPathDirective(std::istringstream &iss, LocationConfig &locationConfig)
{
    std::string cgiPath;
    if (!(iss >> cgiPath))
        throw Parser::InvalidDirectiveException();
    if (!Utils::fileExists(cgiPath))
        throw Parser::ResourceNotFoundException();
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

void Parser::processUploadPathDirective(std::istringstream &iss, LocationConfig &locationConfig, const ServerConfig &serverConfig)
{
    std::string uploadPath;
    if (!(iss >> uploadPath))
        throw Parser::InvalidDirectiveException();
    if (!Utils::directoryExists(Utils::joinPaths(serverConfig.getUploadsDirectory(), uploadPath)))
        throw Parser::ResourceNotFoundException();
    locationConfig.setUploadPath(uploadPath);
}
