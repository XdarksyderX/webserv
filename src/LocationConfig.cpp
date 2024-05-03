/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationConfig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erivero- <erivero-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 17:32:54 by migarci2          #+#    #+#             */
/*   Updated: 2024/05/03 13:00:12 by erivero-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationConfig.hpp"

LocationConfig::LocationConfig()
{
    autoindex = true;
	allowMethods.push_back(GET);
	index = "index.html";
	root = "";
    name = "";
}

LocationConfig::LocationConfig(const std::string &serverRoot, const std::string &locationName)
{
	autoindex = true;
	allowMethods.push_back(GET);
	index = "index.html";
	root = serverRoot;
    name = locationName;
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
        name = other.name;
        root = other.root;
        allowMethods = other.allowMethods;
        index = other.index;
        autoindex = other.autoindex;
        uploadPath = other.uploadPath;
        cgiPaths = other.cgiPaths;
        cgiExtensions = other.cgiExtensions;
    }
    return *this;
}

std::string LocationConfig::getName() const
{
    return name;
}

std::string LocationConfig::getRoot() const
{
    return root;
}

std::vector<HTTPMethod> LocationConfig::getAllowMethods() const
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

std::string LocationConfig::getUploadPath() const
{
    return uploadPath;
}

std::vector<std::string> LocationConfig::getCgiPaths() const
{
    return cgiPaths;
}

std::vector<std::string> LocationConfig::getCgiExtensions() const
{
    return cgiExtensions;
}

void LocationConfig::setName(const std::string &n)
{
    name = n;
}

void LocationConfig::setRoot(const std::string &r)
{
    root = r;
}

void LocationConfig::setAllowMethods(const std::vector<HTTPMethod> &methods)
{
    allowMethods = methods;
}

void LocationConfig::addAllowMethod(HTTPMethod method)
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

void LocationConfig::setUploadPath(const std::string &uP)
{
    uploadPath = uP;
}

void LocationConfig::addCgiPath(const std::string& interpreterPath)
{
    cgiPaths.push_back(interpreterPath);
}

void LocationConfig::addCgiExtension(const std::string& extension)
{
    cgiExtensions.push_back(extension);

}

LocationConfig	LocationConfig::createDefaultLocation(const ServerConfig &serverConfig)
{
    LocationConfig location(serverConfig.getRoot(), "");
    location.setIndex(serverConfig.getIndex());
    std::vector<HTTPMethod> methods;
    methods.push_back(GET);
    location.setUploadPath(serverConfig.getUploadsDirectory());
    location.setAllowMethods(methods);
    return location;
}

std::ostream& operator<<(std::ostream& os, const LocationConfig& config)
{
    os << "\tLocation Configuration:" << std::endl;
    os << "\tRoot: " << config.root << std::endl;
    os << "\tIndex: " << config.index << std::endl;
    return os;
}

std::map<std::string, std::string> LocationConfig::getCgiInfo() const
{
    std::map<std::string, std::string> cgiInfo;

    for (size_t i = 0; i < cgiPaths.size() && i < cgiExtensions.size(); ++i)
    {
        cgiInfo[cgiExtensions[i]] = cgiPaths[i];
    }
    return (cgiInfo);
}
