/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 17:32:54 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/05 22:49:20 by migarci2         ###   ########.fr       */
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

std::vector<std::string> LocationConfig::getCgiPaths() const
{
    return cgiPaths;
}

std::vector<std::string> LocationConfig::getCgiExtensions() const
{
    return cgiExtensions;
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

void LocationConfig::addCgiPath(const std::string& interpreterPath)
{
    cgiPaths.push_back(interpreterPath);
}

void LocationConfig::addCgiExtension(const std::string& extension)
{
    cgiExtensions.push_back(extension);

}

std::ostream& operator<<(std::ostream& os, const LocationConfig& config)
{
    os << "\tLocation Configuration:" << std::endl;
    os << "\tRoot: " << config.root << std::endl;
    os << "\tIndex: " << config.index << std::endl;
    return os;
}
