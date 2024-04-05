/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 18:12:48 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/05 23:28:09 by migarci2         ###   ########.fr       */
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
    {
        configFile.open(configFilePath.c_str());
        if (!configFile.is_open())
            throw Parser::FileNotFoundException();
    }
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
        line.erase(semicolonPos);
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

    while (std::getline(configFile, line))
    {
        parseLine(line);
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string word;
        iss >> word;
        if (word == "server")
            serverConfigs.push_back(parseServerBlock());
        else if (!word.empty())
            throw Parser::InvalidDirectiveException();
    }
    return serverConfigs;
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
