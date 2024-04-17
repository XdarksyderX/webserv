/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponseBuilder.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/06 15:41:13 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/17 17:41:28 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPResponseBuilder.hpp"

const std::map<std::string, std::string> HTTPResponseBuilder::MIME_TYPES = HTTPResponseBuilder::createMIMEMap();
const std::map<int, std::string> HTTPResponseBuilder::STATUS_CODES = HTTPResponseBuilder::createStatusCodesMap();

void	HTTPResponseBuilder::assembleStatusLine(std::string &assembleResponse, const HTTPResponse &response)
{
	assembleResponse += "HTTP/";
	assembleResponse += response.getHttpVersion();
	assembleResponse += " ";
	assembleResponse += Logger::to_string(response.getStatusCode());
	assembleResponse += " ";
	assembleResponse += response.getStatusMessage();
	assembleResponse += "\r\n";
}

void HTTPResponseBuilder::assembleHeaders(std::string &assembleResponse, const HTTPResponse &response)
{
    const std::map<std::string, std::string> &headers = response.getHeaders();

    for (std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); ++it)
	{
        if (!it->first.empty() && !it->second.empty())
		{
			assembleResponse += it->first;
			assembleResponse += ": ";
			assembleResponse += it->second;
			assembleResponse += "\r\n";
		}
    }
}

std::string HTTPResponseBuilder::assembleResponse(const HTTPResponse &response)
{
	std::string assembleResponse;

	assembleStatusLine(assembleResponse, response);
	assembleHeaders(assembleResponse, response);
	assembleResponse += "\r\n";
	assembleResponse += response.getBody();
	return assembleResponse;
}

HTTPResponseBuilder::HTTPResponseBuilder(const ServerConfig &sC, const HTTPRequest &r) : serverConfig(sC), request(r)
{
	
}

std::string HTTPResponseBuilder::findMatchingLocation()
{
    std::string bestMatchKey = "";

    const std::map<std::string, LocationConfig>& locations = serverConfig.getLocations();
    if (locations.empty())
        return NULL;
    std::map<std::string, LocationConfig>::const_iterator it = locations.find("/");
    for (it = locations.begin(); it != locations.end(); ++it)
	{
        if (request.getUri().find(it->first) == 0 && it->first.length() > bestMatchKey.length())
            bestMatchKey = it->first;
    }

    return bestMatchKey;
}

std::string		HTTPResponseBuilder::simpleErrorPage(int errorCode)
{
	std::string response = "<!DOCTYPE html>\n<html>\n<head>\n<title>";
	response += Logger::to_string(errorCode);
	response += " ";
	response += STATUS_CODES.at(errorCode);
	response += "</title>\n</head>\n<body>\n<h1>";
	response += Logger::to_string(errorCode);
	response += " ";
	response += STATUS_CODES.at(errorCode);
	response += "</h1>\n</body>\n</html>\n";
	return response;
}

HTTPResponse	HTTPResponseBuilder::handleErrorPage(int errorCode)
{
	HTTPResponse response;
	response.setStatusCode(errorCode);
	response.setStatusMessage(STATUS_CODES.at(errorCode));
	std::map<int, std::string>::const_iterator it = serverConfig.getErrorPages().find(errorCode);
	std::map<int, std::string>::const_iterator itEnd = serverConfig.getErrorPages().end();
	if (it != itEnd)
	{
		std::string filePath = Utils::joinPaths(serverConfig.getRoot(),
								serverConfig.getErrorPages().at(errorCode));
		response.setBody(Utils::getFileContent(filePath));
		response.addHeader("Content-Type", MIME_TYPES.at(Utils::getExtensionFromFile(filePath)));
	}
	else
	{
		response.setBody(simpleErrorPage(errorCode));
		response.addHeader("Content-Type", "text/html");
	}
	response.addHeader("Content-Length", Logger::to_string(response.getBody().length()));
	response.addHeader("Date", Time::getHTTPFormatCurrentTime());
	response.addHeader("Server", "webserv");
	return response;
}

HTTPResponse	HTTPResponseBuilder::handleGetRequest(const LocationConfig *location)
{
	HTTPResponse response;
	std::string root = serverConfig.getRoot();
	std::string resource = Utils::joinPaths(root, request.getUri());
	std::string directory;
	if (Utils::directoryExists(resource))
	{
		directory = resource;
		resource = Utils::joinPaths(resource, location->getIndex());
	}
	resource = Utils::preventFileTraversal(resource);
	if (!Utils::fileExists(resource))
	{
		if (location->getAutoindex() && !directory.empty())
		{
			response.setBody(Utils::createHTMLDirectoryListing(directory));
			response.addHeader("Content-Type", "text/html");
		}
		else
			return handleErrorPage(404);
	}
	else
	{
		response.setBody(Utils::getFileContent(resource));
		response.addHeader("Content-Type", MIME_TYPES.at(Utils::getExtensionFromFile(resource)));
	}
	response.setStatusCode(200);
	response.addHeader("Content-Length", Logger::to_string(response.getBody().length()));
	response.addHeader("Date", Time::getHTTPFormatCurrentTime());
	response.addHeader("Server", "webserv");
	return response;
}

HTTPResponse HTTPResponseBuilder::handlePostRequest(const LocationConfig *location)
{
    HTTPResponse response;
    std::string root = serverConfig.getUploadsDirectory();
    std::string uploadPath = location->getUploadPath();
    std::string resource = Utils::joinPaths(root, uploadPath);
    std::string fileName = Utils::getNodeName(request.getUri());
    std::string fullPath = Utils::joinPaths(resource, fileName);

    fullPath = Utils::preventFileTraversal(fullPath);

    if (Utils::createFile(fullPath, request.getBody()))
    {
        response.setStatusCode(201);
        response.setBody("File created successfully at " + fullPath);
        response.addHeader("Content-Type", "text/plain");
        response.addHeader("Location", fullPath);
    }
    else
        return handleErrorPage(500);

    response.addHeader("Content-Length", Logger::to_string(response.getBody().length()));
    response.addHeader("Date", Time::getHTTPFormatCurrentTime());
    response.addHeader("Server", "webserv");

    return response;
}

HTTPResponse	HTTPResponseBuilder::buildResponse()
{
	if (request.getHeader("Content-Length") != "")
	{
		size_t contentLength = static_cast<size_t>(std::atoi(request.getHeader("Content-Length").c_str()));
		if (contentLength > serverConfig.getClientMaxBodySize() 
			|| contentLength != request.getBody().length())
			return handleErrorPage(413);
	}
	
	LocationConfig location;
	std::string locationName = findMatchingLocation();
	if (locationName.empty())
	{
		const std::map<std::string, LocationConfig>& locations = serverConfig.getLocations();
		std::map<std::string, LocationConfig>::const_iterator it = locations.find("/");
		std::map<std::string, LocationConfig>::const_iterator itEnd = locations.end();
		if (it == itEnd)
			return handleErrorPage(404);
		else
			location = it->second;
    }
	else
	{
		location = serverConfig.getLocations().at(locationName);
	}

	if (!Utils::hasElement(location.getAllowMethods(), request.getMethod()))
		return handleErrorPage(405);

	if (request.getMethod() == GET)
		return handleGetRequest(&location);
	if ((request.getMethod() == POST || request.getMethod() == PUT) &&
		location.getUploadPath() != "" && serverConfig.getUploadsDirectory() != "")
		return handlePostRequest(&location);
	return handleErrorPage(405);
}
