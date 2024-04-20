/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/05 17:32:51 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/20 17:44:04 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

ServerConfig::ServerConfig()
{
    port = 0;
    clientMaxBodySize = 1024;
    host = "127.0.0.1";
    serverName = "default";
    index = "index.html";
    root = "";
}

ServerConfig::ServerConfig(const ServerConfig &other)
{
    *this = other;
}

ServerConfig::~ServerConfig() {}

ServerConfig &ServerConfig::operator=(const ServerConfig &other)
{
    if (this != &other)
    {
        port = other.port;
        host = other.host;
        serverName = other.serverName;
        root = other.root;
        index = other.index;
        errorPages = other.errorPages;
        clientMaxBodySize = other.clientMaxBodySize;
        locations = other.locations;
        uploadsDirectory = other.uploadsDirectory;
    }
    return *this;
}

unsigned short ServerConfig::getPort() const
{
    return port;
}

std::string ServerConfig::getHost() const
{
    return host;
}

std::string ServerConfig::getServerName() const
{
    return serverName;
}

std::string ServerConfig::getRoot() const
{
    return root;
}

std::string ServerConfig::getIndex() const
{
    return index;
}

std::map<int, std::string> ServerConfig::getErrorPages() const
{
    return errorPages;
}

size_t ServerConfig::getClientMaxBodySize() const
{
    return clientMaxBodySize;
}

std::map<std::string, LocationConfig> ServerConfig::getLocations() const
{
    return locations;
}

std::string ServerConfig::getUploadsDirectory() const
{
    return uploadsDirectory;
}

void ServerConfig::setPort(unsigned short p)
{
    port = p;
}

void ServerConfig::setHost(const std::string &h)
{
    host = h;
}

void ServerConfig::setServerName(const std::string &name)
{
    serverName = name;
}

void ServerConfig::setRoot(const std::string &r)
{
    root = r;
}

void ServerConfig::setIndex(const std::string &i)
{
    index = i;
}

void ServerConfig::setErrorPages(const std::map<int, std::string> &pages)
{
    errorPages = pages;
}

void ServerConfig::setClientMaxBodySize(size_t size)
{
    clientMaxBodySize = size;
}

void ServerConfig::setLocations(const std::map<std::string, LocationConfig> &locs)
{
    locations = locs;
}

void ServerConfig::addErrorPage(int errorCode, const std::string &path)
{
    errorPages[errorCode] = path;
}

void ServerConfig::addLocation(const std::string &path, const LocationConfig &config)
{
   
    locations[path] = config;
}

void ServerConfig::setUploadsDirectory(const std::string &uD)
{
    uploadsDirectory = uD;
}

std::ostream &operator<<(std::ostream &os, const ServerConfig &config)
{
    os << "Server Configuration:" << std::endl;
    os << "Port: " << config.port << std::endl;
    os << "Host: " << config.host << std::endl;
    os << "Server Name: " << config.serverName << std::endl;
    os << "Root: " << config.root << std::endl;
    os << "Index: " << config.index << std::endl;
    os << "Client Max Body Size: " << config.clientMaxBodySize << std::endl;

    os << "Error Pages:" << std::endl;
    for (std::map<int, std::string>::const_iterator it = config.errorPages.begin(); it != config.errorPages.end(); ++it)
        os << "  Code " << it->first << ": " << it->second << std::endl;

    os << "Locations:" << std::endl;
    for (std::map<std::string, LocationConfig>::const_iterator it = config.locations.begin(); it != config.locations.end(); ++it)
        os << "\tPath: " << it->first << it->second << std::endl;

    return os;
}