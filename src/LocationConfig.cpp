/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 17:32:54 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/05 20:56:53 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationConfig.hpp"

LocationConfig::LocationConfig()
{
    autoindex = true;
	allowMethods.push_back("GET");
	index = "index.html";
	root = "";
}

LocationConfig::LocationConfig(std::string serverRoot)
{
	autoindex = true;
	allowMethods.push_back("GET");
	index = "index.html";
	root = serverRoot;
}

LocationConfig::LocationConfig(const LocationConfig &other)
{
    *this = other;
}

LocationConfig::~LocationConfig() {}

LocationConfig &LocationConfig::operator=(const LocationConfig &other)
{
    if (this != &other)
    {
        root = other.root;
        allowMethods = other.allowMethods;
        index = other.index;
        autoindex = other.autoindex;
        returnPath = other.returnPath;
        alias = other.alias;
        cgiInfo = other.cgiInfo;
    }
    return *this;
}

std::string LocationConfig::getRoot() const
{
    return root;
}

std::vector<std::string> LocationConfig::getAllowMethods() const
{
    return allowMethods;
}

std::string LocationConfig::getIndex() const
{
    return index;
}

bool LocationConfig::getAutoindex() const
{
    return autoindex;
}

std::string LocationConfig::getReturnPath() const
{
    return returnPath;
}

std::string LocationConfig::getAlias() const
{
    return alias;
}

std::map<std::string, std::string> LocationConfig::getCgiInfo() const
{
    return cgiInfo;
}

void LocationConfig::setRoot(const std::string &r)
{
    root = r;
}

void LocationConfig::setAllowMethods(const std::vector<std::string> &methods)
{
    allowMethods = methods;
}

void LocationConfig::addAllowMethod(const std::string &method)
{
    allowMethods.push_back(method);
}

void LocationConfig::setIndex(const std::string &i)
{
    index = i;
}

void LocationConfig::setAutoindex(bool ai)
{
    autoindex = ai;
}

void LocationConfig::setReturnPath(const std::string &rp)
{
    returnPath = rp;
}

void LocationConfig::setAlias(const std::string &a)
{
    alias = a;
}

void LocationConfig::setCgiInfo(const std::map<std::string, std::string> &ci)
{
    cgiInfo = ci;
}

void LocationConfig::addCgiMapping(const std::string &extension, const std::string &interpreterPath)
{
    cgiInfo[extension] = interpreterPath;
}

std::ostream& operator<<(std::ostream& os, const LocationConfig& config)
{
    os << "Location Configuration:" << std::endl;
    os << "Root: " << config.root << std::endl;
    os << "Index: " << config.index << std::endl;
    return os;
}
